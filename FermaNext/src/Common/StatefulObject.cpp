
#include "StatefulObject.h"

#include <algorithm>

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
            iter = states.erase(iter);
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
    QString emptyName;
    return createState( emptyName );
}

ObjectState& StatefulObject::createState ( const QString& stateName )
{
    // Remove already disabled states
    removeDisabledStates();
    // Create new one
    ObjectState* state = new ObjectState(this, stateName);
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

size_t StatefulObject::countStates () const
{
    return states.size();
}

size_t StatefulObject::countEnabledStates () const
{
    size_t statesNum = 0;
    StateListConstIter iter = states.begin();
    for ( ; iter != states.end(); ++iter )
        if ( ! (*iter)->isDisabled() )
            ++statesNum;
    return statesNum;
}

void StatefulObject::desist ()
{
    if ( ! isAlive() )
        return;
    emit onBeforeDesist(*this);
    isDesisted = true;
    emit onAfterDesist(*this);
}

void StatefulObject::revive ()
{
    if ( isAlive() )
        return;
    emit onBeforeRevive(*this);
    isDesisted = false;
    emit onAfterRevive(*this);
}

bool StatefulObject::isAlive () const
{
    return !isDesisted;
}

/*****************************************************************************/
