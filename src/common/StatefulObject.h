
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
    virtual ObjectState& createState ( const QString& stateName );
    virtual bool removeState ( ObjectState& );

    // Count all states
    virtual size_t countStates () const;
    // Count only enabled states
    virtual size_t countEnabledStates () const;

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
    void onBeforeDesist ( StatefulObject& );
    void onBeforeRevive ( StatefulObject& );
    void onAfterDesist ( StatefulObject& );
    void onAfterRevive ( StatefulObject& );

private:
    typedef std::vector<ObjectState*> StateList;
    typedef std::vector<ObjectState*>::iterator StateListIter;
    typedef std::vector<ObjectState*>::const_iterator StateListConstIter;

    StateList states;
    ObjectStateManager* stateManager;
    bool isDesisted;   
};

#endif //STATEFULOBJECT_H
