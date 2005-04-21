
#ifndef STATEFULOBJECT_H
#define STATEFULOBJECT_H

#include "ObjectStateManager.h"

template <class Obj> class StatefulObject;
template <class Obj> class ConcreteState;

class ObjectState
{
public:
    virtual void submitState () const = 0;
    virtual ~ObjectState () { };
};

template <class Obj>
class ConcreteObjectState : virtual public ObjectState
{
    Obj* obj;
    Obj* state;    
public:
    ConcreteObjectState ( Obj* obj_, Obj* state_ ) : 
        obj(obj_), state(state_) 
    {}

    ~ConcreteObjectState () 
    { delete state; }

    void submitState () const
    { obj->loadState(*state); }
};

template <class Obj>
class StatefulObject
{
protected:
    virtual Obj* self () = 0;
public:
    virtual void loadState ( const Obj& node ) = 0;

    void saveState ( ObjectStateManager& sm )
    {        
        sm.saveState( new ConcreteObjectState<Obj>(self(), new Obj(*self())) );
    }
};


#endif //STATEFULOBJECT_H
