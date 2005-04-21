
#include "ObjectStateManager.h"
#include "StatefulObject.h"

#include <algorithm>


ObjectStateManager::ObjectStateManager () :    
    currentState(0)
{
    states.reserve(DEF_STACK_SIZE);
}

ObjectStateManager::~ObjectStateManager ()
{
    StatesIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )
        delete *iter;    
}

void ObjectStateManager::saveState ( ObjectState* st ) throw (UnknownException)
{
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
    states.push_back(st);
    currentState = st;    
}

void ObjectStateManager::undo () throw (UnknownException, UndoException) 
{    
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
}

void ObjectStateManager::redo () throw (UnknownException, RedoException)
{    
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
}
