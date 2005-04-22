
#ifndef STATEFULOBJECT_H
#define STATEFULOBJECT_H

#include <qobject.h>
#include "ObjectStateManager.h"

// State of any object. 
class ObjectState;
template <class Obj> class ConcreteState;

// Object, that can be saved and loaded by state.
class StatefulObject;
template <class Obj> class ConcreteStatefulObject;

class ObjectState : public QObject
{
    Q_OBJECT
public:
    ObjectState () : disabled(false) {}
    virtual void submitState () const = 0;
    virtual ~ObjectState () { };
    bool isDisabled () const 
    { return disabled; }
public slots:
    void disable ()
    { disabled = false; emit onDisable(this); }
signals:
    void onDisable ( ObjectState* );
private:
    mutable bool disabled;
};

template <class Obj>
class ConcreteObjectState : virtual public ObjectState
{
    Obj* obj;
    Obj* state;    
public:
    ConcreteObjectState ( Obj* obj_, Obj* state_ ) : 
        obj(obj_), state(state_)
    { connect(obj, SIGNAL(onStateDestroy(StatefulObject*)), SLOT(disable())); }
       
    ~ConcreteObjectState ()
    { delete state; }

    void submitState () const
    { if ( !isDisabled() ) obj->loadState(*state); }
};

class StatefulObject : public QObject 
{
    Q_OBJECT
public: 
    virtual ~StatefulObject () 
    { emit onStateDestroy(this); } 
signals:
    void onStateDestroy ( StatefulObject* );
};

template <class Obj>
class ConcreteStatefulObject : public StatefulObject
{
protected:
    virtual Obj* self () = 0;
public:
    virtual ~ConcreteStatefulObject () {}
    virtual void loadState ( const Obj& node ) = 0;

    void saveState ( ObjectStateManager& sm )
    {        
        sm.saveState( new ConcreteObjectState<Obj>(self(), new Obj(*self())) );
    }
};


#endif //STATEFULOBJECT_H
