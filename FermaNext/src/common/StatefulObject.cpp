
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
    clear();  
}

void StatefulObject::removeDisabledStates ()
{
    StateListIter iter = states.begin();
    while ( iter != states.end() ) {
        ObjectState* state = *iter;
        if ( state->isDisabled() ) {
            delete state;
            states.erase(iter);
        }
        else 
            ++iter;
    }
}

void StatefulObject::clear ()
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
    // Removes states
    clear();
}

ObjectState& StatefulObject::createState ()
{
    // Remove already disabled states
    removeDisabledStates();
    // Create new one
    ObjectState* state = new ObjectState(this);
    states.push_back(state);
    return *state;
}

bool StatefulObject::removeState ( ObjectState& state )
{
    StateListIter iter = std::find( states.begin(), states.end(), &state );
    if ( iter == states.end() )
        return false;    
    delete *iter;
    states.erase(iter);
    return true;
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
