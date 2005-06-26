
#include "StatefulObject.h"

/*****************************************************************************
 * Stateful Object
 *****************************************************************************/

StatefulObject::StatefulObject ( ObjectStateManager* mng ) :
    stateManager(mng),
    isDesisted(false)
{
    if ( stateManager )
        connect( stateManager, SIGNAL(destroyed()), 
                               SLOT(stateManagerDestroyed()) );
}

StatefulObject::~StatefulObject ()
{
    StateListIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )        
        delete *iter;
    states.clear();        
}

ObjectStateManager* StatefulObject::getStateManager ()
{
    return stateManager;
}

void StatefulObject::stateManagerDestroyed ()
{
    stateManager = 0;
}

ObjectState& StatefulObject::createState ()
{
    ObjectState* state = new ObjectState(this);
    connect( state, SIGNAL(onStateDestroy(ObjectState&)),
                    SLOT(removeState(ObjectState&)) );
    states.push_back(state);
    return *state;
}

bool StatefulObject::removeState ( ObjectState& state )
{
    StateListIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )
        if ( *iter = &state ) {
            delete *iter;
            states.erase(iter);
            return true;
        }
    return false;
}

size_t StatefulObject::countStates ()
{
    return states.size();
}

void StatefulObject::desist ()
{
    emit onDesist(*this);
    isDesisted = true;
}

void StatefulObject::revive ()
{
    emit onRevive(*this);
    isDesisted = false;    
}

bool StatefulObject::isAlive ()
{
    return !isDesisted;
}

/*****************************************************************************/
