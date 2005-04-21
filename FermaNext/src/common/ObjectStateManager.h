
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>

class ObjectState;

class ObjectStateManager 
{
public:
    typedef std::vector<ObjectState*> States;
    typedef States::iterator StatesIter;

    ObjectStateManager ( size_t );

    virtual void saveState ( ObjectState* st );
    virtual void undo ();
    virtual void redo ();

private:
    States states;
    ObjectState* currentState;
};

#endif //STATEMANAGER_H
