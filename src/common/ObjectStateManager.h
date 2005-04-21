
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>

class ObjectState;

class ObjectStateManager 
{
public:
    virtual void saveState ( ObjectState* st );
    virtual void undo ();
    virtual void redo ();
};

#endif //STATEMANAGER_H
