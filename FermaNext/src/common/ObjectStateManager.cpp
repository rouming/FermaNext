
#include "ObjectStateManager.h"
#include "StatefulObject.h"

#include <algorithm>

/*****************************************************************************
 * State Block
 *****************************************************************************/

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
    for ( ; iter != states.end(); ++iter )
        (*iter)->disable();
    states.clear();
}

/*****************************************************************************
 * Object State Manager
 *****************************************************************************/

ObjectStateManager::ObjectStateManager () :
    currentBlock(0),    
    possibleStackSize(POSSIBLE_STACK_SIZE),
    startedBlocks(0),
    currentBlockIsEnded(true),
    isStateCallFlag(false)
{
    // Suppose there are 4 states in one block. 
    stateBlocks.reserve(possibleStackSize/4);
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
    // First start
    if ( countStateBlocks() == 0 ) {
        currentBlock = new StateBlock();
        stateBlocks.push_back(currentBlock);
        currentBlockIsEnded = false;
        startedBlocks = 1;
    }
    // Begins new block if previous block was filled and correctly ended
    else if ( currentBlockIsEnded ) {
        currentBlock = new StateBlock();
        stateBlocks.push_back(currentBlock);
        currentBlockIsEnded = false;
        startedBlocks = 1;
    }
    // We should count inner blocks, but if they are not empty
    else if ( !currentBlock->isEmpty() )
        ++startedBlocks;

    // Does nothing if block was not ended or not started yet
}

void ObjectStateManager::endStateBlock ()
{
    // Finishes block if it is not empty and all inner blocks are ended
    if ( currentBlock != 0 &&
         !currentBlock->isEmpty() && !currentBlockIsEnded ) {
        // If only one block remains we should close it
        if ( startedBlocks == 1 )
            currentBlockIsEnded = true;
        --startedBlocks;
    }
    // Does nothing if block was already finished or not started
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

    block->removeState(st);

    // Remove block
    stateBlocks.erase(iter);    
    delete block;

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
    size_t statesNum = countStates();
    if ( statesNum < possibleStackSize )
        return false;

    // Remove first block
    BlockListIter iterBegin = stateBlocks.begin();
    delete *iterBegin;
    stateBlocks.erase( iterBegin );

    return true;
}

void ObjectStateManager::saveState ( ObjectState& st )
{
    if ( currentBlock != 0 ) {
        bool noStartedBlock = !currentBlock->isEmpty() && currentBlockIsEnded;

        // Shift the stack if possible
        tryToShiftStack();

        // New block has not been started yet, so start it
        if ( noStartedBlock )
            startStateBlock();        
        
        // Save state
        currentBlock->addState(st);

        // End newly started block
        if ( noStartedBlock )
            endStateBlock();
    }
    // First call or all blocks were undoed. Create single state block.
    else {
        // Should shift at first
        tryToShiftStack();
        startStateBlock();
        // Save state
        currentBlock->addState(st);
        endStateBlock();
    }

    // Connect to know when state is ready to be destroyed
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
    if ( !currentBlockIsEnded ) 
        throw StateBlockIsNotEnded();

    BlockListIter iter = std::find( stateBlocks.begin(), 
                                    stateBlocks.end(), 
                                    currentBlock);
    // Hm. Strange.
    if ( iter == stateBlocks.end() )                
        throw UnknownException();

    // Safe undo
    stateCall(true);
    emit beforeUndo(*this);
    currentBlock->undo();
    emit afterUndo(*this);
    stateCall(false);
    
    // Should zero current block if we undoed the first block
    if ( iter == stateBlocks.begin() )
        currentBlock = 0;
    else {
        --iter;
        currentBlock = *iter;
    }        
}

void ObjectStateManager::redo () throw (UnknownException, RedoException,
                                        StateBlockIsNotEnded)
{
    // Nothing to undo
    if ( countStateBlocks() == 0 ) 
        throw RedoException();

    // Can't redo if block is not ended
    if ( !currentBlockIsEnded ) 
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
    currentBlock = *iter;

    // Safe redo
    stateCall(true);
    emit beforeRedo(*this);
    currentBlock->redo();
    emit afterRedo(*this);
    stateCall(false);
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
