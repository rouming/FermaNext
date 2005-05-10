
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
    { disabled = true; emit onDisable(this); }
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
    { connect(obj, SIGNAL(onStatefulObjectDestroy(StatefulObject*)), SLOT(disable()));  }
       
    ~ConcreteObjectState ()
    { if ( !isDisabled() ) obj->removeObjectState(state);  }

    void submitState () const
    { if ( !isDisabled() ) obj->loadState(*state); }
};

class StatefulObject : public QObject 
{
    Q_OBJECT
public:
    virtual ~StatefulObject () 
    // Should notice everyone about forthcoming decease.
    { emit onStatefulObjectDestroy(this); }
signals:
    void onStatefulObjectDestroy ( StatefulObject* );
};

template <class Obj>
class ConcreteStatefulObject : public StatefulObject
{
protected:
    // Do not forget to clear states from the descendant 
    // destructor, or memory leaks are awaited.
    void clearStates () 
    {
        std::vector<Obj*>::iterator i = states.begin();
        for ( ; i != states.end(); ++i )
            removeState(*i);
        states.clear();
    }
public:
    Obj* takeObjectState ()
    {
        Obj* state = takeState();
        states.push_back(state);
        return state;
    }

    void removeObjectState ( Obj* state )
    {
        std::vector<Obj*>::iterator i = states.begin();
        for ( ; i != states.end(); ++i )
            if ( *i == state ) {
                Obj* st = *i;
                states.erase(i);
                removeState(st);
                return;
            }
    }
protected:
    virtual Obj* self () = 0;
    virtual Obj* takeState () = 0;
    virtual void removeState ( Obj* ) = 0;
public:
    void saveState ( ObjectStateManager& sm )
    {        
        sm.saveState( new ConcreteObjectState<Obj>(self(), takeObjectState()) );
    }

    virtual void loadState ( const Obj& node ) = 0;  
private:
    std::vector<Obj*> states;
};

#endif //STATEFULOBJECT_H
