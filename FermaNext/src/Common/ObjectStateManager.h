
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <QObject>
#include <QStringList>

class ObjectState;

class ObjectStateManager : public QObject
{
    Q_OBJECT
public:
    // Some state manager exceptions
    class OutOfBoundsException {};
    class StepException {};
    class UndoException {};
    class RedoException {};
    class StateBlockIsNotEnded {};

protected:
    class StateBlock
    {   
    public:
        StateBlock ( ObjectStateManager&, 
                     const QString& blockName = QString() );
        ~StateBlock ();
        
        // Manage states
        void addState ( ObjectState& );
        bool removeState ( ObjectState& );
        // Checks if vector contains state
        bool contains ( const ObjectState& ) const;
        bool isEmpty () const;
        size_t countStates () const;

        // Returns block name
        const QString&  getBlockName () const;
        // Sets block name
        void setBlockName ( const QString& );

        // Returns concatenated string of state names
        QString concatStateNames () const;

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
        
        QString blockName;
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
    virtual bool tryToRemoveStackTop ();

protected slots:
    // Removes all block and its states. Finds block by the state.
    // Returns true if succeed
    virtual bool removeBlockByState (  ObjectState& );

public:
    ObjectStateManager ( size_t stackSize = 256 );
    virtual ~ObjectStateManager ();

    //Manages state blocks
    virtual void startStateBlock ();
    virtual void endStateBlock ( const QString& blockName = QString() );
    // Returns true if newly created block is not ended.
    virtual bool stateBlockIsNotEnded () const;

    // Saves object state. If startStateBlock was called first, state
    // saves in this block.
    virtual void saveState ( ObjectState& );

    // Manages the undo/redo cookery
    virtual void undo () /*throw (UndoException, StateBlockIsNotEnded)*/;
    virtual void redo () /*throw (RedoException, StateBlockIsNotEnded)*/;

    // Rollbacks and removes all states of started block, which 
    // was started but has not been ended yet.
    virtual void rollbackNotEndedBlock ();

    // Momentary step to saved state block by index
    // (undo or redo from the current position to defined direction) 
    // if indx is 0 -- try to undo all states
    virtual void step ( uint indx ) 
        /*throw (OutOfBoundsException, StepException, RedoException, 
                 UndoException, StateBlockIsNotEnded)*/;

    // Step to the begin of the stack
    // same as 'step(0)' call
    virtual void stepToBegin () 
        /*throw (OutOfBoundsException, StepException, RedoException, 
                 UndoException, StateBlockIsNotEnded)*/;

    // Step to the end of the stack (see step)
    // same as 'step( countStateBlocks() )' call
    virtual void stepToEnd () 
        /*throw (OutOfBoundsException, StepException, RedoException, 
                 UndoException, StateBlockIsNotEnded)*/;

    // Returns names of all blocks
    virtual QStringList stateBlockNames () const;

    // Returns current position of redoed state block.
    // 0 is returned if all stack has been undoed or if it is empty
    virtual uint currentPos () const;

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
    void beforeStep ( ObjectStateManager& );
    void afterStep ( ObjectStateManager& );
    void onSaveState ( ObjectStateManager&, ObjectState& );
    void onRemoveState ( ObjectStateManager&, ObjectState& );
    void onStateBlockIsEnded ( ObjectStateManager& );

private:
    typedef std::vector<StateBlock*> BlockList;
    typedef BlockList::iterator BlockListIter;
    typedef BlockList::const_iterator BlockListConstIter;

    BlockList stateBlocks;
    StateBlock* currentBlock;
    StateBlock* newlyCreatedBlock;
    size_t possibleStackSize;
    size_t startedBlocks;
    bool isStateCallFlag;
};

#endif //STATEMANAGER_H
