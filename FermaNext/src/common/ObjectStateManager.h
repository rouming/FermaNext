
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <qobject.h>

class ObjectState;

const size_t POSSIBLE_STACK_SIZE = 256;

class ObjectStateManager : public QObject
{
    Q_OBJECT
public:
    // Some state manager exceptions
    class UndoException {};
    class RedoException {};
    class UnknownException {};

protected:
    class StateBlock
    {   
    public:
        ~StateBlock ();
        
        // Adds the state to vector
        void addState ( ObjectState& );
        // Checks if vector contains state
        bool contains ( ObjectState& );
        bool isEmpty ();
        size_t countStates ();

        // Undoes/Redoes all states of block
        void undo ();
        void redo ();

    protected:
        // Clear state vector
        void clear ();
        
    private:
        typedef std::vector<ObjectState*> StateList;
        typedef StateList::iterator StateListIter;
        typedef StateList::reverse_iterator StateListRevIter;
        
        StateList states;
        bool blockIsClosed;
    };

    // Returns block, which contains state or 0 otherwise
    virtual StateBlock* findBlock ( ObjectState& );

    // Just removes first block if states number of all blocks
    // are > possible stack size
    bool tryToShiftStack ();

    // Tries to remove top of the stack, if current block is pointing
    // not to the top
    bool tryToRemoveStackTop () throw (UnknownException);

protected slots:
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

    // Returns number of states in all blocks
    virtual size_t countStates ();
    // Returns number of blocks
    virtual size_t countBlocks ();

    // When state of object changes in some method, object should
    // create state to undo changes in future. When undo works, method
    // can be called again to restore the state. So we can get into
    // recursion. isStateCall returns true when the state restores
    // itself from undo or redo call.
    virtual bool isStateCall ();

protected:
    // Simply frees all allocated memory
    void clear ();

    // Sets state call flag
    void stateCall ( bool );

private:
    typedef std::vector<StateBlock*> BlockList;
    typedef BlockList::iterator BlockListIter;

    BlockList stateBlocks;
    StateBlock* currentBlock;
    size_t possibleStackSize;
    bool currentBlockIsEnded;
    bool isStateCallFlag;
};

#endif //STATEMANAGER_H
