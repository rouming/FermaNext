
#include "ObjectStateManager.h"
#include "StatefulObject.h"

#include <algorithm>

/*****************************************************************************
 * State Block
 *****************************************************************************/

ObjectStateManager::StateBlock::StateBlock ( ObjectState* start_, 
                                            ObjectState* end_ ) :
    start(start_),
    end(end_)
{}

/*****************************************************************************
 * Object State Manager
 *****************************************************************************/

ObjectStateManager::ObjectStateManager () :
    currentBlock(0),    
    possibleStackSize(DEF_STACK_SIZE)    
{
    states.reserve(possibleStackSize);
}

ObjectStateManager::~ObjectStateManager ()
{}

bool ObjectStateManager::isValid ( StateBlock& block )
    throw (UnknownException)
{
    // Block is not started
    if ( block.start == 0 )
        return false;

    StateListIter startIter = std::find( states.begin(), states.end(), block.start );
    // Never should happen
    if ( startIter == states.end() )
        throw UnknownException();

    // Block is not finished. That's ok.
    if ( block.end == 0 )
        return true;

    StateListIter endIter = std::find( states.begin(), states.end(), block.end );

    // Never should happen
    if ( endIter == states.end() )
        throw UnknownException();
    
    return (startIter <= endIter);
}

void ObjectStateManager::startStateBlock ()
{
    // First start
    if ( currentBlock == 0 ) {
        currentBlock = new StateBlock();
        stateBlocks.push_back(currentBlock);
    }
    // Begins new block if previous block was filled
    else if ( stateBlockIsStarted() && stateBlockIsEnded() ) {
        currentBlock = new StateBlock();
        stateBlocks.push_back(currentBlock);
    }
    // Does nothing if block was not ended or 
    // not started yet
}

void ObjectStateManager::endStateBlock ()
{
    // Finishes block if it was already started 
    // (so states vector is not empty)
    if ( stateBlockIsStarted() && !stateBlockIsEnded() ) {
        currentBlock->end = states.back();        
    }
    // Does nothing if block was already finished or not started
}

bool ObjectStateManager::stateBlockIsStarted ()
{
    return (currentBlock != 0 && currentBlock->start != 0);
}

bool ObjectStateManager::stateBlockIsEnded ()
{
    return (currentBlock != 0 && currentBlock->end != 0);
}

ObjectStateManager::StateBlock* ObjectStateManager::findBlock ( ObjectState& state )
    throw (BlockIsInvalid)
{
    
    BlockListIter iter = stateBlocks.begin();
    for ( ; iter != stateBlocks.end(); ++iter ) {
        StateBlock* block = *iter;
        // Is not inited, strange but possible
        if ( block->start == 0 )
            continue;
        if ( !isValid(*block) )
            throw BlockIsInvalid();

        StateListIter startIter = std::find( states.begin(), states.end(), 
                                             block->start );
        StateListIter endIter;

        // Block is not finished. That's ok. Should be the last block
        if ( block->end == 0 )
            endIter = states.end();
        else {
            endIter = std::find( states.begin(), states.end(), block->end );
            // Take following element
            ++endIter;
        }

        for ( ; startIter != endIter; ++startIter )
            if ( *startIter == &state )
                return block;
    }
    return 0;
}

bool ObjectStateManager::removeStatesFromBlock ( StateBlock& block ) 
    throw (UnknownException)
{
    // Nothing to remove if block was not inited
    if ( block.start == 0 )
        return false;

    StateListIter startIter = std::find( states.begin(), 
                                         states.end(), 
                                         block.start );
    // Never should happen
    if ( startIter == states.end() )
        throw UnknownException();

    StateListIter endIter;

    // We take the end of the stack if block is not finished
    if ( block.end == 0 )
        endIter = states.end();
    else {
        endIter = std::find( states.begin(), states.end(), block.end );
        // Never should happen
        if ( endIter == states.end() )
            throw UnknownException();
        // Take following element for correct erase
        ++endIter;
    }
    // Strange if start > end. Never should happen
    if ( startIter > endIter )
        throw UnknownException();

    states.erase( startIter, endIter );
    return true;    
}

bool ObjectStateManager::removeBlockByState (  ObjectState& st )
{
    StateBlock* block = findBlock(st);
    if ( block == 0 )
        return false;

    BlockListIter iter = std::find( stateBlocks.begin(), 
                                    stateBlocks.end(), 
                                    block );
    if ( iter == stateBlocks.end() )
        // Unreachable line, or is not thread-safe
        //throw IsNotThreadSafe();
        return false;

    // Remove all states from this block
    removeStatesFromBlock(*block);
    // Remove block
    stateBlocks.erase(iter);
    delete block;

    return true;
}

bool ObjectStateManager::tryToRemoveStackTop () throw (UnknownException)
{
    if ( currentBlock == 0 )
        return false;
    
    StateListIter iter = std::find(states.begin(), states.end(), currentBlock->start);
    // We should remove top of the stack if current state exists in vector
    // and it is not the last element.
    if ( iter == states.end() )
        throw UnknownException();

    // Nothing to do if current state is at the top of the stack
    if ( ++iter != states.end() )
        return false;

    StateListIter eraseIter = iter;
    for ( ; iter != states.end(); ++iter )
        delete *iter;            
    states.erase( eraseIter, states.end() );
    return true;
}

bool ObjectStateManager::tryToPushStack ()
{
    // At first we should remove top of the stack
    bool topIsRemoved = tryToRemoveStackTop();
    // If top is removed, nothing to push -- we have enough free space
    if ( topIsRemoved )
        return true;

    // Should we push?
    if ( states.size() != possibleStackSize )
        return false;

    
    return true;
}

void ObjectStateManager::saveState ( ObjectState* st ) throw (UnknownException)
{
    if ( currentBlock != 0 ) {
        bool noStartedBlock = stateBlockIsStarted() && stateBlockIsEnded();

        // New block has not been started yet, so start it        
        if ( noStartedBlock )
            // Starts new state block
            startStateBlock();

        
        tryToPushStack();
        


        // End newly started block
        if ( noStartedBlock )
            // Ends started block
            endStateBlock();        
    }
    // First call without started block
    else {
        // Starts new state block
        startStateBlock();
        currentBlock->start = st;        
        states.push_back(st);
        QObject::connect(st, SIGNAL(onStateDestroy(ObjectState&)), 
                             SLOT(removeBlockByState(ObjectState&)));
        // Ends started block
        endStateBlock();
    }



/*
    if ( currentState != 0 ) {
        StatesIter iter = std::find(states.begin(), states.end(), currentState);
        // We should remove top of the stack if current state exists in vector
        // and it is not the last element.
        if ( iter == states.end() )
            throw UnknownException();
        if ( iter != states.end() && ++iter != states.end() )
        {
            StatesIter eraseIter = iter;
            for ( ; iter != states.end(); ++iter )
                delete *iter;            
            states.erase( eraseIter, states.end() );
        }
    }
    QObject::connect(st, SIGNAL(onDisable(ObjectState*)), 
                         SLOT(removeState(ObjectState*)));
    states.push_back(st);
    currentState = st;    
    */
}

void ObjectStateManager::undo () throw (UnknownException, UndoException) 
{    /*
    if ( currentState == 0 ) 
        throw UnknownException();        
    StatesIter iter = std::find( states.begin(), states.end(), currentState );
    if ( iter == states.end() )                
        throw UnknownException();
    if ( iter == states.begin() )
        throw UndoException();
    --iter;
    currentState = *iter;
    currentState->submitState();
    */
}

void ObjectStateManager::redo () throw (UnknownException, RedoException)
{    /*
    if ( currentState == 0 )
        throw UnknownException();
    StatesIter iter = std::find( states.begin(), states.end(), currentState );
    if ( iter == states.end() )
        throw UnknownException();
    ++iter;
    if ( iter == states.end() )
        throw RedoException();
    currentState = *iter;
    currentState->submitState();
    */
}

size_t ObjectStateManager::countStates ()
{
    return states.size();
}

/*****************************************************************************/
