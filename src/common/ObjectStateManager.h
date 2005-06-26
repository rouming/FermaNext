
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <qobject.h>

class ObjectState;

const size_t DEF_STACK_SIZE = 256;

class ObjectStateManager : public QObject
{
    Q_OBJECT
public:
    // Some state manager exceptions
    class UndoException {};
    class RedoException {};
    class UnknownException {};
    class IsNotThreadSafe {};
    class BlockIsInvalid {};

protected:
    struct StateBlock 
    {   
        StateBlock ( ObjectState* start = 0, ObjectState* end = 0 );

        ObjectState* start;
        ObjectState* end;
    };
    // Checks block validity: `start` state should be 
    // found below `end` state or `end` should be 0
    virtual bool isValid ( StateBlock& ) throw (UnknownException);
    // Returns true if new block is started
    virtual bool stateBlockIsStarted ();
    // Returns true if new block is ended
    virtual bool stateBlockIsEnded ();
    // Returns block, which contains state or 0 otherwise
    virtual StateBlock* findBlock ( ObjectState& ) throw (BlockIsInvalid);

    // Tries to push blocks according to possible stack size
    bool tryToPushStack ();

    // Tries to remove top of the stack, if current block is pointing
    // not to the first element
    bool tryToRemoveStackTop () throw (UnknownException);

protected slots:
    // Simply removes states from block, zeros it and returns true
    virtual bool removeStatesFromBlock ( StateBlock& block ) throw (UnknownException);
    // Removes all block and its states. Finds block by the state.
    // Returns true if succeed
    virtual bool removeBlockByState (  ObjectState& );

public:
    ObjectStateManager ();
    virtual ~ObjectStateManager ();

    //Manages state blocks
    virtual void startStateBlock ();
    virtual void endStateBlock ();

    // Saves object state. If startStateBlock was called first, state
    // saves in this block.
    virtual void saveState ( ObjectState* ) throw (UnknownException);

    // Manages the undo/redo cookery
    virtual void undo () throw (UnknownException, UndoException);
    virtual void redo () throw (UnknownException, RedoException);

    virtual size_t countStates ();

private:
    typedef std::vector<ObjectState*> StateList;
    typedef StateList::iterator StateListIter;
    typedef std::vector<StateBlock*> BlockList;
    typedef BlockList::iterator BlockListIter;

    StateList states;
    BlockList stateBlocks;
    StateBlock* currentBlock;    
    size_t possibleStackSize;
};

#endif //STATEMANAGER_H
