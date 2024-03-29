
#include "ObjectState.h"
#include "StatefulObject.h"

/*****************************************************************************
 * Object State 
 *****************************************************************************/

ObjectState::ObjectState ( StatefulObject* stObj ) :
    statefulObject(stObj),
    isDisabledFlag(false)
{}

ObjectState::ObjectState ( StatefulObject* stObj, const QString& name  ) :
    statefulObject(stObj),
    stateName(name),
    isDisabledFlag(false)
{}

ObjectState::~ObjectState ()
{
    emit onStateDestroy(*this);
    ActionListIter iter = actions.begin();
    for ( ; iter != actions.end(); ++iter )
        delete *iter;    
    actions.clear();
}

bool ObjectState::isDisabled ()
{
    return isDisabledFlag;
}

void ObjectState::disable ()
{
    isDisabledFlag = true;
}

void ObjectState::addAction ( AbstractObjectAction* action )
{
    actions.push_back(action);
}

bool ObjectState::removeAction ( AbstractObjectAction* action )
{
    ActionListIter iter = actions.begin();
    for ( ; iter != actions.end(); ++iter )
        if ( *iter == action ) {
            actions.erase(iter);
            return true;
        }
    return false;
}

const QString& ObjectState::getStateName () const
{
    return stateName;
}

void ObjectState::setStateName ( const QString& name )
{
    stateName = name;
}

void ObjectState::commitState ()
{
    beforeCommit(*this);
    ActionListIter iter = actions.begin();
    for ( ; iter != actions.end(); ++iter )
        (*iter)->execute();
    afterCommit(*this);
}

void ObjectState::rollbackState ()
{
    beforeRollback(*this);
    ActionListRevIter iter = actions.rbegin();
    for ( ; iter != actions.rend(); ++iter )
        (*iter)->unexecute();
    afterRollback(*this);
}

void ObjectState::save ()
{
    StatefulObject* stObj = getStatefulObject();
    ObjectStateManager* mng = stObj->getStateManager();
    // If manager is alive try to save the state
    if ( mng )
        mng->saveState(*this);
}

StatefulObject* ObjectState::getStatefulObject () const
{
    return statefulObject;
}

/*****************************************************************************/
