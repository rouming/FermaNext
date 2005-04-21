
#include "ObjectStateManager.h"
#include "StatefulObject.h"

#include <algorithm>

ObjectStateManager::ObjectStateManager ( size_t stackSize ) :
    states(stackSize),
    currentState(0)
{}

void ObjectStateManager::saveState ( ObjectState* st )
{
    if ( currentState != 0 ) {
        StatesIter iter = std::find(states.begin(), states.end(), currentState);
        // We should remove top of the stack if current state exists in vector
        // and it is not the last element.
        if ( iter != states.end() && ++iter != states.end() )
            //FIXME: memory leak
            states.erase(iter, states.end());
    }
    states.push_back(st);
    currentState = st;
}

void ObjectStateManager::undo () 
{    
    if ( currentState == 0 ) 
        //FIXME: throw smth, no return
        return;
    StatesIter iter = std::find( states.begin(), states.end(), currentState );
    if ( iter == states.end() )
        return;//FIXME: throw smth in future to determine the error
    if ( iter == states.begin() )
        return;//FIXME:throw smth to notice that only one state is in the stack
    --iter;
    currentState = *iter;
    currentState->submitState();
}
void ObjectStateManager::redo () 
{    
    if ( currentState == 0 ) 
        //FIXME: throw smth, not return
        return;
    StatesIter iter = std::find( states.begin(), states.end(), currentState );
    if ( iter == states.end() )
        return;//FIXME: throw smth in future to determine the error
    ++iter;
    if ( iter == states.end() )
        //FIXME: throw smth to notice that there is nothing to redo
        return;
    currentState = *iter;
    currentState->submitState();
}
