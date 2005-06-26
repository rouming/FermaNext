
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H

#include "AbstractObjectAction.h"

#include <vector>

class StatefulObject;

// Such statefule object, that can keep one or many actions.
class ObjectState : public QObject
{
    Q_OBJECT
public:
    ObjectState ( StatefulObject* );
    virtual ~ObjectState ();

public:
    // Add action to main vector.
    // Be aware: added actions are removed by 
    // object state in destructor.
    virtual void addAction ( AbstractObjectAction* );
    virtual bool removeAction ( AbstractObjectAction* );

    // Main state operations. Commit should execute 
    // every stored action, rollback -- unexecute 
    // actions in reverse order.
    virtual void commitState ();
    virtual void rollbackState ();

    // Save this state to the state manager which we get 
    // from the stateful object which manages this state
    virtual void save ();

    // Returns the stateful object which manages this state
    virtual StatefulObject* getStatefulObject ();
    
signals:
    void onStateDestroy ( ObjectState& );
    void beforeCommit ( ObjectState& );
    void afterCommit ( ObjectState& );
    void beforeRollback ( ObjectState& );
    void afterRollback ( ObjectState& );

private:
    typedef std::vector<AbstractObjectAction*> ActionList;
    typedef std::vector<AbstractObjectAction*>::iterator ActionListIter;        
    typedef std::vector<AbstractObjectAction*>::reverse_iterator ActionListRevIter;
    
    ActionList actions;
    StatefulObject* statefulObject;
};

#endif //OBJECTSTATE_H
