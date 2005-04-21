
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>

class ObjectState;

const size_t DEF_STACK_SIZE = 256;

class ObjectStateManager 
{
public:
    // Some state manager exceptions
    class UndoException {};
    class RedoException {};
    class UnknownException {};

    typedef std::vector<ObjectState*> States;
    typedef States::iterator StatesIter;

    ObjectStateManager ();
    virtual ~ObjectStateManager ();

    virtual void saveState ( ObjectState* st ) throw (UnknownException);
    virtual void undo () throw (UnknownException, UndoException);
    virtual void redo () throw (UnknownException, RedoException);

private:
    States states;
    ObjectState* currentState;
};

#endif //STATEMANAGER_H
