
#include "StatefulObject.h"
#include "ObjectStateManager.h"

#include <iostream>

int objects = 0;
int PASSED = 0, FAILED = 0;

void my_assert( bool val, const std::string& str ) 
{
    if ( val ) ++PASSED;
    else ++FAILED;
    
    std::cout << (val ? "    OK: " : "NOT OK: ") << str << "\n";
}

class SomeObject : public StatefulObject
{
public:
    SomeObject (int val_, ObjectStateManager& mng) :
        StatefulObject(&mng),
        val(val_)
    { ++objects; }
    ~SomeObject () { --objects;  }
public:
    int value () const { return val; }
    void value ( int val_ ) 
    { 
        if ( !getStateManager()->isStateCall() ) {       
            ObjectState& state = createState(); 
            
            ConcreteObjectAction<SomeObject, int>* action = 
                new ConcreteObjectAction<SomeObject, int>(*this, 
                                                          &SomeObject::value,
                                                          &SomeObject::value,
                                                          val_,
                                                          val);
            state.addAction( action );
            getStateManager()->saveState(&state);
        }
        
        val = val_; 
    }
private:
    int val;
};


void start_end_state_block_test ()
{
    ObjectStateManager m;
    StatefulObject o(&m);
    ObjectState& st = o.createState();
    
    my_assert( m.countBlocks() == 0, "After init m.countBlocks() == 0" );

    m.startStateBlock();
    my_assert( m.countBlocks() == 1, "m.startStateBlock(): should be 1 block");
    m.endStateBlock();
    my_assert( m.countBlocks() == 1, "m.endStateBlock(): should be 1 block");

    // Can't start new block because previous one is empty
    m.startStateBlock();
    my_assert(m.countBlocks() == 1, "m.startStateBlock(): should be 1 block");

    m.saveState(&st);
    my_assert(m.countStates() == 1, "m.countStates(): should be 1 state");
    m.endStateBlock();

    // New block
    m.startStateBlock();
    my_assert( m.countBlocks() == 2, "m.endStateBlock(): should be 2 block");
    m.saveState(&st);
    // Doesn't create new block because previous one is not closed
    m.startStateBlock();
    my_assert( m.countBlocks() == 2, "Should be 2 blocks" );
    m.saveState(&st);
    my_assert( m.countStates() == 3, "Should be 3 states" );

    // Close inner block.
    m.endStateBlock();
    my_assert( m.countBlocks() == 2, "Should be 2 blocks" );
    // Checks that outer block is not closed
    m.startStateBlock();
    my_assert( m.countBlocks() == 2, "Should be 2 again blocks" );
    m.saveState(&st);
    my_assert( m.countStates() == 4, "Should be 4 states" );
    m.endStateBlock();

    // Really close.
    m.endStateBlock();
    my_assert( m.countBlocks() == 2, "Should be 2 blocks" );

    // New third block
    m.startStateBlock();
    my_assert( m.countBlocks() == 3, "Should be 3 blocks" );
    m.saveState(&st);
    my_assert( m.countStates() == 5, "Should be 5 states" );
    m.endStateBlock();    
}



int main ()
{
    const int STACK_SIZE = 10;
    int i = 0;

    start_end_state_block_test();

    ObjectStateManager m;
   
    {// Scope

        SomeObject obj(666, m);

        //Changes
        for ( i=0; i < STACK_SIZE; ++i ) {
            obj.value(i+1);
        }
        my_assert( obj.value() == STACK_SIZE, "Last change" );

        //Full Undo
        for ( i=STACK_SIZE-1; i > 0; --i ) {
            m.undo();
            my_assert( obj.value() == i, "Undo" );
        }

        //Rollback to initial
        m.undo();
        my_assert( obj.value() == 666, "Initial" );

        //Check bounds
        bool excp = false;
        try { m.undo();  } 
        catch ( ObjectStateManager::UndoException& ) { excp = true; }
        my_assert( excp, "UndoException catched" );

        //Full Redo
        for (  i=1; i <= STACK_SIZE; ++i ) {
            m.redo();
            my_assert( obj.value() == i, "Redo" );
        }

        //Check bounds
        excp = false;
        try { m.redo();  } 
        catch ( ObjectStateManager::RedoException& ) { excp = true; }
        my_assert( excp, "RedoException catched" );

        //Half Undo
        for (  i=STACK_SIZE-1; i >= STACK_SIZE/2; --i ) {
            m.undo();
            my_assert( obj.value() == i, "Half undo" );
        }    

        //Half Redo
        for (  i=STACK_SIZE/2+1; i <= STACK_SIZE; ++i ) {
            m.redo();
            my_assert( obj.value() == i, "Half redo" );
        }
        
        //Half Undo
        for (  i=STACK_SIZE-1; i >= STACK_SIZE/2; --i ) {
            m.undo();
            my_assert( obj.value() == i, "Half undo2" );
        }    
        
        //New Changes
        for (  i=STACK_SIZE/2; i <= STACK_SIZE; ++i ) {
            obj.value((i)*10);
        }
        my_assert( obj.value() == STACK_SIZE*10, "Last new change" );

        //Full Undo
        int j =0;
        for ( i=STACK_SIZE; i > 0; --i ) {
            m.undo();
            if ( i > STACK_SIZE/2 ) 
                j = (i-1)*10;
            else 
                j = i;
            my_assert( obj.value() == j, "Undo3" );
        }

        //Rollback to initial
        m.undo();
        my_assert( obj.value() == 666, "Initial" );

        SomeObject* anotherObj = new SomeObject(101010, m);
        m.startStateBlock();
        anotherObj->value(1);
        anotherObj->value(2);
        anotherObj->value(3);
        anotherObj->value(4);

        //Check Undo/Redo incorrectness, because of unclosed block
        excp = false;
        try { m.undo();  } 
        catch ( ObjectStateManager::StateBlockIsNotEnded& ) { excp = true; }
        my_assert( excp, "BlockIsNotClosed should be catched" );

        excp = false;
        try { m.redo();  } 
        catch ( ObjectStateManager::StateBlockIsNotEnded& ) { excp = true; }
        my_assert( excp, "BlockIsNotClosed should be catched" );

        m.endStateBlock();

        m.undo();
        my_assert( anotherObj->value() == 101010, "Undo state block" );
        m.redo();
        my_assert( anotherObj->value() == 4, "Redo state block" );

        delete anotherObj;

    }//Scope ends
    my_assert( objects == 0, "All objects should be destroyed" );
    my_assert( m.countStates() == 0, "State vector should be empty" );
    my_assert( m.countBlocks() == 0, "Block vector should be empty" );


		std::cout << "\n" << "Passed: " << PASSED << "\nFailed: " << FAILED << "\n";
    return 0;    
}

