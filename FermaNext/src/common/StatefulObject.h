
#ifndef STATEFULOBJECT_H
#define STATEFULOBJECT_H

#include "ObjectState.h"
#include "ObjectStateManager.h"

#include <qobject.h>


// Basic class in stateful hierarchy.
// This class is used to provide Qt subsidiary signals. 
class StatefulObject : public QObject 
{
    Q_OBJECT
public:        
    StatefulObject ( ObjectStateManager* );
    virtual ~StatefulObject ();

    // Returns state manager of this stateful object.
    // If manager died, returns 0.
    virtual ObjectStateManager* getStateManager ();

    // Manages states.
    virtual ObjectState& createState ();
    virtual bool removeState ( ObjectState& );

    virtual size_t countStates () const;

    // Marks this stateful object as desisted
    virtual void desist ();
    // Revives this stateful object 
    virtual void revive ();
    // Checks if it is alive
    virtual bool isAlive () const;

protected:
    // Frees all allocated states
    virtual void clear ();
    // Frees only disabled states
    virtual void removeDisabledStates ();

protected slots:
    // Marks manager as destroyed
    virtual void stateManagerDestroyed ();

signals:
    void onDesist ( StatefulObject& );
    void onRevive ( StatefulObject& );

private:
    typedef std::vector<ObjectState*> StateList;
    typedef std::vector<ObjectState*>::iterator StateListIter;

    StateList states;
    ObjectStateManager* stateManager;
    bool isDesisted;   
};

#endif //STATEFULOBJECT_H
