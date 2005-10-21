
#include "ObjectStateManager.h"
#include "StatefulObject.h"

#include <algorithm>

/*****************************************************************************
 * State Block
 *****************************************************************************/

ObjectStateManager::StateBlock::StateBlock ( ObjectStateManager& mng ) : 
    stateMng(mng)
{}

ObjectStateManager::StateBlock::~StateBlock ()
{
    clear();
}

void ObjectStateManager::StateBlock::addState ( ObjectState& state )
{
    states.push_back(&state);
}

bool ObjectStateManager::StateBlock::removeState ( ObjectState& state )
{
    StateListIter iter = std::find( states.begin(), states.end(), &state );
    if ( iter == states.end() )
        return false;    
    (*iter)->disable();
    states.erase(iter);
    return true;
}

bool ObjectStateManager::StateBlock::contains ( const ObjectState& st ) const
{
    StateListConstIter iter = std::find( states.begin(), states.end(), &st );
    // Found nothing
    if ( iter == states.end() )
        return false;
    return true;
}
bool ObjectStateManager::StateBlock::isEmpty () const
{
    return states.size() == 0;
}

size_t ObjectStateManager::StateBlock::countStates () const
{
    return states.size();
}

QString ObjectStateManager::StateBlock::concatStateNames () const
{
    QString names;
    StateListConstIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )
        names += (*iter)->getStateName() + '\t';
    return names;
}

void ObjectStateManager::StateBlock::undo ()
{
    StateListRevIter iter = states.rbegin();
    for ( ; iter != states.rend(); ++iter )
        (*iter)->rollbackState();
}

void ObjectStateManager::StateBlock::redo ()
{
    StateListIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )
        (*iter)->commitState();
}

void ObjectStateManager::StateBlock::clear ()
{
    StateListIter iter = states.begin();
    for ( ; iter != states.end(); ++iter ) {
        ObjectState* state = *iter;
        // Disconnect all states
        QObject::disconnect( state, 0, &stateMng, 0 );
        state->disable();
    }
    states.clear();
}

/*****************************************************************************
 * Object State Manager
 *****************************************************************************/

ObjectStateManager::ObjectStateManager ( size_t stackSize ) :
    currentBlock(0),
    possibleStackSize(stackSize),
    startedBlocks(0),
    isStateCallFlag(false),
    newlyCreatedBlock(false)
{
    stateBlocks.reserve(possibleStackSize);
}

ObjectStateManager::~ObjectStateManager ()
{
    clear();
}

void ObjectStateManager::clear ()
{
    BlockListIter iter = stateBlocks.begin();
    for ( ; iter != stateBlocks.end(); ++iter )
        delete *iter;
    stateBlocks.clear();
}

void ObjectStateManager::startStateBlock ()
{
    // We should increment inner blocks
    ++startedBlocks;
}

void ObjectStateManager::endStateBlock ()
{
    if ( startedBlocks == 0 )
        return;

    // We should decrement inner blocks
    --startedBlocks;

    // Outer last block is ended and something was saved in it
    if ( startedBlocks == 0 && newlyCreatedBlock ) {
        newlyCreatedBlock = false;
        emit onStateBlockIsEnded(*this);
    }
}

bool ObjectStateManager::stateBlockisNotEnded () const
{
    return (startedBlocks != 0);
}

ObjectStateManager::StateBlock* ObjectStateManager::findBlock ( 
                                                          ObjectState& state )
{
    BlockListIter iter = stateBlocks.begin();
    for ( ; iter != stateBlocks.end(); ++iter ) {
        StateBlock* block = *iter;
        if ( block->contains(state) )
            return block;
    }
    return 0;
}

bool ObjectStateManager::removeBlockByState (  ObjectState& st )
{
    StateBlock* block = findBlock(st);
    if ( block == 0 )
        return false;

    BlockListIter iter = std::find( stateBlocks.begin(), 
                                    stateBlocks.end(), 
                                    block );
    if ( iter == stateBlocks.end() ) {
        // Unreachable line, or is not thread-safe
        //throw IsNotThreadSafe();
        return false;
    }

    // If current block exists we should be sure it is located
    // below the block is going to be removed.
    if ( currentBlock != 0 ) {
        BlockListIter currIter = std::find( stateBlocks.begin(), 
                                            stateBlocks.end(), 
                                            currentBlock );
        // Strange. Should never reach this line. 
        if ( currIter == stateBlocks.end() ) {
            return false;
        }
        // Current block must be located below the block
        // which is going to be removed.
        if ( currIter >= iter )
            currentBlock = ( iter != stateBlocks.begin() ? *(iter - 1) : 0 );
    }

    // Remove all blocks wich are located above the block
    // which is going to be removed.
    // =============================
    // Remember: We can't just remove block by dying state. 
    //           Actually it is not save. Some states, which 
    //           were saved after dying state, can depend on 
    //           it. So all states located after dying state 
    //           should be removed.
    // =============================
    BlockListIter eraseIter = iter;
    for ( ; iter != stateBlocks.end(); ++iter )
        delete *iter;
    stateBlocks.erase( eraseIter, stateBlocks.end() );

    emit onRemoveState( *this, st );

    return true;
}

bool ObjectStateManager::tryToRemoveStackTop () throw (UnknownException)
{
    // Is empty
    if ( countStateBlocks() == 0 )
        return false;

    BlockListIter iter;

    // If current block is not empty remove all blocks 
    // which are placed after it
    if ( currentBlock != 0 ) {
        iter = std::find(stateBlocks.begin(), stateBlocks.end(), currentBlock);
 
       // Strange.
        if ( iter == stateBlocks.end() )
            throw UnknownException();

        // Nothing to do if current block is at the top of the stack
        if ( ++iter == stateBlocks.end() )
            return false;
    }
    // Remove all stack
    else 
        iter = stateBlocks.begin();

    // Remove useless blocks
    BlockListIter eraseIter = iter;
    for ( ; iter != stateBlocks.end(); ++iter )
        delete *iter;
    stateBlocks.erase( eraseIter, stateBlocks.end() );

    return true;
}

bool ObjectStateManager::tryToShiftStack ()
{
    // At first we should remove top of the stack
    bool topIsRemoved = tryToRemoveStackTop();
    // If top is removed, nothing to push -- we have enough free space
    if ( topIsRemoved )
        return true;

    // Should we shift?
    size_t stateBlockNum = countStateBlocks();
    if ( stateBlockNum < possibleStackSize )
        return false;

    // Remove first block
    BlockListIter iterBegin = stateBlocks.begin();
    delete *iterBegin;
    stateBlocks.erase( iterBegin );

    return true;
}

ObjectStateManager::StateBlock& ObjectStateManager::tryToCreateStateBlock ()
{
    if ( ! newlyCreatedBlock ) {
        tryToShiftStack();
        currentBlock = new StateBlock(*this);
        stateBlocks.push_back(currentBlock);
        newlyCreatedBlock = true;
    }   
    return *currentBlock;
}

void ObjectStateManager::saveState ( ObjectState& st )
{
    startStateBlock();
    // Save state
    tryToCreateStateBlock().addState(st);
    endStateBlock();

    // Connect to know when state is ready to be destroyed.
    // Beware! All state signals are disconnected in destructor of state block.
    QObject::connect(&st, SIGNAL(onStateDestroy(ObjectState&)), 
                          SLOT(removeBlockByState(ObjectState&)));
    
    emit onSaveState(*this, st);
}

void ObjectStateManager::undo () throw (UnknownException, UndoException, 
                                        StateBlockIsNotEnded) 
{    
    // Nothing to undo
    if ( countStateBlocks() == 0 ) 
        throw UndoException();

    // Nothing to undo
    if ( currentBlock == 0 )
        throw UndoException();

    // Can't undo if block is not ended
    if ( startedBlocks )
        throw StateBlockIsNotEnded();

    BlockListIter iter = std::find( stateBlocks.begin(), 
                                    stateBlocks.end(), 
                                    currentBlock);
    // Hm. Strange.
    if ( iter == stateBlocks.end() )                
        throw UnknownException();

    // Safe undo
    emit beforeUndo(*this);
    stateCall(true);

    currentBlock->undo();
    
    // Should zero current block if we undoed the first block
    if ( iter == stateBlocks.begin() )
        currentBlock = 0;
    else {
        --iter;
        currentBlock = *iter;
    }

    stateCall(false);
    emit afterUndo(*this);
}

void ObjectStateManager::redo () throw (UnknownException, RedoException,
                                        StateBlockIsNotEnded)
{
    // Nothing to redo
    if ( countStateBlocks() == 0 ) 
        throw RedoException();

    // Can't redo if block is not ended
    if ( startedBlocks )
        throw StateBlockIsNotEnded();

    BlockListIter iter;

    if ( currentBlock == 0 )
        iter = stateBlocks.begin();
    else {
        iter = std::find(stateBlocks.begin(), stateBlocks.end(), currentBlock);

        // Hm. Strange.
        if ( iter == stateBlocks.end() )
            throw UnknownException();

        ++iter;

        // Nothing to redo
        if ( iter == stateBlocks.end() )
            throw RedoException();

    }

    // Safe redo
    emit beforeRedo(*this);
    stateCall(true);

    currentBlock = *iter;
    currentBlock->redo();

    stateCall(false);
    emit afterRedo(*this);
}

void ObjectStateManager::step ( uint indx ) 
    throw (UnknownException, OutOfBoundsException, StepException,
           RedoException, UndoException, StateBlockIsNotEnded)
{
    size_t stateBlocksNum = countStateBlocks();

    // Nothing to do
    if ( stateBlocksNum == 0 )
        throw StepException();

    // Wrong index
    if ( indx > stateBlocksNum ) 
        throw OutOfBoundsException();

    // Can't step if block is not ended
    if ( startedBlocks )
        throw StateBlockIsNotEnded();

    StateBlock* stepToBlock = (indx == 0 ? 0 : stateBlocks[indx-1]);

    // Nothing to do
    if ( stepToBlock == currentBlock )
        return;

    BlockListIter stepIter;
    
    if ( stepToBlock == 0 )
        stepIter = stateBlocks.begin();
    else 
        stepIter = std::find( stateBlocks.begin(), stateBlocks.end(), 
                              stepToBlock );

    // Hm. Strange.
    if ( stepIter == stateBlocks.end() )
        throw UnknownException();

    BlockListIter currIter;
    
    if ( currentBlock == 0 )
        currIter = stateBlocks.begin();
    else
        currIter = std::find( stateBlocks.begin(), stateBlocks.end(), 
                              currentBlock);

    // Hm. Strange.
    if ( currIter == stateBlocks.end() )
        throw UnknownException();

    BlockListIter begin = currIter, end = stepIter;

    emit beforeStep( *this );

    // Choose step direction
    if ( currentBlock == 0 || stepIter > currIter ) { // redo
        // Redo first block
        if ( currentBlock == 0 )
            redo();
        for ( ; begin != end; ++begin )
            redo();
    } 
    else {    // undo: stepIter < currIter 
        for ( ; begin != end; --begin )
            undo();
        // Undo first block
        if ( stepToBlock == 0 )
            undo();
    }

    emit afterStep( *this );
}

void ObjectStateManager::stepToBegin () 
    throw (UnknownException, OutOfBoundsException, StepException,
           RedoException, UndoException, StateBlockIsNotEnded)
{
    step(0);
}

void ObjectStateManager::stepToEnd () 
    throw (UnknownException, OutOfBoundsException, StepException,
           RedoException, UndoException, StateBlockIsNotEnded)
{
    step( countStateBlocks() );
}

QStringList ObjectStateManager::stateBlockNames () const
{
    QStringList names;
    BlockListConstIter iter = stateBlocks.begin();
    for ( ; iter != stateBlocks.end(); ++iter )
        names.push_back( (*iter)->concatStateNames() );
    return names;
}

uint ObjectStateManager::currentPos () const
{
    if ( currentBlock == 0 )
        return 0;
    uint pos = 0;
    BlockListConstIter iter = stateBlocks.begin();
    for ( pos = 1 ; iter != stateBlocks.end(); ++iter, ++pos )
        if ( *iter == currentBlock )
            return pos;
    // Unreachable line
    return 0;
}

size_t ObjectStateManager::countStates () const
{
    size_t statesNum = 0;
    BlockListConstIter iter = stateBlocks.begin();
    for ( ; iter != stateBlocks.end(); ++iter )
        statesNum += (*iter)->countStates();
    return statesNum;
}

size_t ObjectStateManager::countStateBlocks () const
{
    return stateBlocks.size();
}

size_t ObjectStateManager::countStatesToRedo () const
{
    if ( currentBlock == 0 )
        return countStates();
    BlockListConstIter iter = std::find( stateBlocks.begin(),
                                         stateBlocks.end(),
                                         currentBlock );
    // Hm. Strange.
    if ( iter == stateBlocks.end() )
        //throw UnknownException();
        return 0;

    size_t statesNum = 0;
    while ( ++iter != stateBlocks.end() )
        statesNum += (*iter)->countStates();
    return statesNum;
}

size_t ObjectStateManager::countStateBlocksToRedo () const
{
    if ( currentBlock == 0 )
        return countStateBlocks();
    BlockListConstIter iter = std::find( stateBlocks.begin(),
                                         stateBlocks.end(),
                                         currentBlock );
    // Hm. Strange.
    if ( iter == stateBlocks.end() )
        //throw UnknownException();
        return 0;

    size_t statesNum = 0;
    while ( ++iter != stateBlocks.end() )
        ++statesNum;
    return statesNum;
}

size_t ObjectStateManager::countStatesToUndo () const
{
    if ( currentBlock == 0 )
        return 0;
    BlockListConstIter iter = std::find( stateBlocks.begin(),
                                         stateBlocks.end(),
                                         currentBlock );
    // Hm. Strange.
    if ( iter == stateBlocks.end() )
        //throw UnknownException();
        return 0;    

    size_t statesNum = 0;
    while ( iter-- != stateBlocks.begin() )
        statesNum += (*iter)->countStates();
    // We should count states of first state block
    statesNum += stateBlocks.front()->countStates();
    return statesNum;
}

size_t ObjectStateManager::countStateBlocksToUndo () const
{
    if ( currentBlock == 0 )
        return 0;
    BlockListConstIter iter = std::find( stateBlocks.begin(),
                                         stateBlocks.end(),
                                         currentBlock );
    // Hm. Strange.
    if ( iter == stateBlocks.end() )
        //throw UnknownException();
        return 0;    

    size_t statesNum = 0;
    while ( iter-- != stateBlocks.begin() )
        ++statesNum;
    // We should count first state block
    ++statesNum;
    return statesNum;
}

void ObjectStateManager::stateCall ( bool stateCall )
{
    isStateCallFlag = stateCall;
}

bool ObjectStateManager::isStateCall () const
{
    return isStateCallFlag;
}

/*****************************************************************************/
