
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <qobject.h>

class ObjectState;

class ObjectStateManager : public QObject
{
    Q_OBJECT
public:
    // Some state manager exceptions
    class UndoException {};
    class RedoException {};
    class StateBlockIsNotEnded {};
    class UnknownException {};

protected:
    class StateBlock
    {   
    public:
        StateBlock ( ObjectStateManager& );
        ~StateBlock ();
        
        // Manage states
        void addState ( ObjectState& );
        bool removeState ( ObjectState& );
        // Checks if vector contains state
        bool contains ( const ObjectState& ) const;
        bool isEmpty () const;
        size_t countStates () const;

        // Undoes/Redoes all states of block
        void undo ();
        void redo ();

    protected:
        // Clear state vector
        void clear ();
        
    private:
        typedef std::vector<ObjectState*> StateList;
        typedef StateList::iterator StateListIter;
        typedef StateList::const_iterator StateListConstIter;
        typedef StateList::reverse_iterator StateListRevIter;
        
        StateList states;
        ObjectStateManager& stateMng;
        bool blockIsClosed;
    };

    // Returns block, which contains state or 0 otherwise
    virtual StateBlock* findBlock ( ObjectState& );

    // Just removes first block if states number of all blocks
    // are > possible stack size
    virtual bool tryToShiftStack ();

    // Tries to remove top of the stack, if current block is pointing
    // not to the top
    virtual bool tryToRemoveStackTop () throw (UnknownException);

    // Tries to create new block if it was not created,
    // otherwise returns old block
    virtual StateBlock& tryToCreateStateBlock ();

protected slots:
    // Removes all block and its states. Finds block by the state.
    // Returns true if succeed
    virtual bool removeBlockByState (  ObjectState& );

public:
    ObjectStateManager ( size_t stackSize = 256 );
    virtual ~ObjectStateManager ();

    //Manages state blocks
    virtual void startStateBlock ();
    virtual void endStateBlock ();

    // Saves object state. If startStateBlock was called first, state
    // saves in this block.
    virtual void saveState ( ObjectState& );

    // Manages the undo/redo cookery
    virtual void undo () throw (UnknownException, UndoException, 
                                StateBlockIsNotEnded);
    virtual void redo () throw (UnknownException, RedoException,
                                StateBlockIsNotEnded);

    // Returns number of states in all blocks
    virtual size_t countStates () const;
    // Returns number of state blocks
    virtual size_t countStateBlocks () const;

    // Returns number of states to redo
    virtual size_t countStatesToRedo () const;
    // Returns number of state blocks to redo
    virtual size_t countStateBlocksToRedo () const;

    // Returns number of states to undo
    virtual size_t countStatesToUndo () const;
    // Returns number of state blocks to undo
    virtual size_t countStateBlocksToUndo () const;

    // When state of object changes in some method, object should
    // create state to undo changes in future. When undo works, method
    // can be called again to restore the state. So we can get into
    // recursion. isStateCall returns true when the state restores
    // itself from undo or redo call.
    virtual bool isStateCall () const;

protected:
    // Simply frees all allocated memory
    virtual void clear ();

    // Sets state call flag
    virtual void stateCall ( bool );

signals:
    void beforeUndo ( ObjectStateManager& );
    void afterUndo ( ObjectStateManager& );
    void beforeRedo ( ObjectStateManager& );
    void afterRedo ( ObjectStateManager& );
    void onSaveState ( ObjectStateManager&, ObjectState& );
    void onRemoveState ( ObjectStateManager&, ObjectState& );

private:
    typedef std::vector<StateBlock*> BlockList;
    typedef BlockList::iterator BlockListIter;
    typedef BlockList::const_iterator BlockListConstIter;

    BlockList stateBlocks;
    StateBlock* currentBlock;
    size_t possibleStackSize;
    size_t startedBlocks;
    bool isStateCallFlag;
    bool newlyCreatedBlock;
};

#endif //STATEMANAGER_H
