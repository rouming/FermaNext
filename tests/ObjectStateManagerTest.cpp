
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
            state.save();
        }
        
        val = val_; 
    }
private:
    int val;
};


void start_end_state_block_test ()
{    
    std::cout << std::endl << ">>>> start_end_state_block_test" << std::endl;

    ObjectStateManager* mng = new ObjectStateManager;
    ObjectStateManager& m = *mng;
    StatefulObject o(&m);

    my_assert( m.countStateBlocks() == 0, "After init m.countStateBlocks() == 0" );

    m.startStateBlock();
    my_assert( m.countStateBlocks() == 1, "m.startStateBlock(): should be 1 block");
    m.endStateBlock();
    my_assert( m.countStateBlocks() == 1, "m.endStateBlock(): should be 1 block");

    // Can't start new block because previous one is empty
    m.startStateBlock();
    my_assert(m.countStateBlocks() == 1, "m.startStateBlock(): should be 1 block");

    ObjectState& st1 = o.createState();
    m.saveState(st1);
    my_assert(m.countStates() == 1, "m.countStates(): should be 1 state");
    m.endStateBlock();

    // New block
    m.startStateBlock();
    my_assert( m.countStateBlocks() == 2, "m.endStateBlock(): should be 2 block");
    ObjectState& st2 = o.createState();
    m.saveState(st2);
    // Doesn't create new block because previous one is not closed
    m.startStateBlock();
    my_assert( m.countStateBlocks() == 2, "Should be 2 blocks" );
    ObjectState& st3 = o.createState();
    m.saveState(st3);
    my_assert( m.countStates() == 3, "Should be 3 states" );

    // Close inner block.
    m.endStateBlock();
    my_assert( m.countStateBlocks() == 2, "Should be 2 blocks" );
    // Checks that outer block is not closed
    m.startStateBlock();
    my_assert( m.countStateBlocks() == 2, "Should be 2 again blocks" );
    ObjectState& st4 = o.createState();
    m.saveState(st4);
    my_assert( m.countStates() == 4, "Should be 4 states" );
    m.endStateBlock();

    // Really close.
    m.endStateBlock();
    my_assert( m.countStateBlocks() == 2, "Should be 2 blocks" );

    // New third block
    m.startStateBlock();
    my_assert( m.countStateBlocks() == 3, "Should be 3 blocks" );
    ObjectState& st5 = o.createState();
    m.saveState(st5);
    my_assert( m.countStates() == 5, "Should be 5 states" );
    m.endStateBlock();    

    delete mng;
    
    my_assert( o.getStateManager() == 0, "State manager should be NULL" );

    std::cout << std::endl;
}


void stack_shifting_test ()
{
    std::cout << std::endl << ">>>> stack_shifting_test" << std::endl;

    ObjectStateManager m;
    SomeObject obj(666, m);

    const size_t STACK_SIZE = 256;
    size_t i = 0;

    // Fill all stack by states 
    for ( i = 1; i <= STACK_SIZE; ++i ) 
        obj.value(i);

    // Some more (+ quarter)
    for ( i = 1; i <= STACK_SIZE/4; ++i ) 
        obj.value(i);

    // Full undo
    for ( i = 1; i <= STACK_SIZE; ++i ) 
        m.undo();

    // Full redo and check
    size_t ii = 0;
    bool shifting_res = true;
    for ( i = 0; i < STACK_SIZE; ++i ) {
        int val = obj.value();
        bool res;
        if ( i <= STACK_SIZE/4 * 3 )
            res = ((size_t)val == STACK_SIZE/4 + i );
        else 
            res = ((size_t)val == ++ii);
        if ( !res ) {
            my_assert( false, "Shifting" );
            shifting_res = res;
        }
        m.redo();
    }
    my_assert( shifting_res, "All shifting" );

    std::cout << std::endl;
}

void block_removing_test ()
{
    std::cout << std::endl << ">>>> stack_shifting_test" << std::endl;

    ObjectStateManager m;

    SomeObject* obj1 = new SomeObject(0, m);
    SomeObject* obj2 = new SomeObject(10, m);
    SomeObject* obj3 = new SomeObject(100, m);

    int i =0;

    // Next nearest test
    for ( i = 1; i <= 10; ++i ) {
        obj1->value( i );
        obj2->value( 10 + i );
        obj3->value( 100 + i );
    }
    my_assert( m.countStateBlocks() == 10 * 3, "1. Init count state blocks" );
    my_assert( m.countStates() == 10 * 3, "1. Init count states" );
    delete obj3;
    my_assert( m.countStateBlocks() == 2, "1. Obj3 removed: state blocks" );
    my_assert( m.countStates() == 2, "1. Obj3 removed: states" );
    delete obj2;
    my_assert( m.countStateBlocks() == 1, "1. Obj2 removed: state blocks" );
    my_assert( m.countStates() == 1, "1. Obj2 removed: states" );
    delete obj1;
    my_assert( m.countStateBlocks() == 0, "1. Obj1 removed: none state blocks" );
    my_assert( m.countStates() == 0, "1. Obj1 removed: none states" );


    // Group test
    obj1 = new SomeObject(0, m);
    obj2 = new SomeObject(10, m);
    obj3 = new SomeObject(100, m);
    for ( i = 1; i <= 10; ++i ) {
        obj1->value( i );
    }
    for ( i = 1; i <= 10; ++i ) {
        obj2->value( 10 + i );
    }
    for ( i = 1; i <= 10; ++i ) {
        obj3->value( 100 + i );
    }
    my_assert( m.countStateBlocks() == 10 * 3, "2. Init count state blocks" );
    my_assert( m.countStates() == 10 * 3, "2. Init count states" );
    delete obj2;
    my_assert( m.countStateBlocks() == 10, "2. Obj2 removed: state blocks" );
    my_assert( m.countStates() == 10, "2. Obj2 removed: states" );
    delete obj3;
    my_assert( m.countStateBlocks() == 10, "2. Obj3 removed: state blocks" );
    my_assert( m.countStates() == 10, "2. Obj3 removed: states" );
    delete obj1;
    my_assert( m.countStateBlocks() == 0, "2. Obj1 removed: none state blocks" );
    my_assert( m.countStates() == 0, "2. Obj1 removed: none states" );

    
    // Undo/Redo after block removing test
    obj1 = new SomeObject(0, m);
    obj2 = new SomeObject(666, m);
    obj3 = new SomeObject(100, m);
    for ( i = 1; i <= 10; ++i ) {
        obj1->value( i );
    }

    obj2->value( 1 );

    for ( i = 1; i <= 10; ++i ) {
        obj3->value( 100 + i );
    }

    for ( i = 1; i <= 10; ++i ) {
        m.undo();
    }
    // Undo obj2
    m.undo();
    my_assert( obj2->value() == 666, "3. Undo to obj2 init value" );

    // Half redo. Nevermind what is the number.
    for ( i = 1; i <= 10/2; ++i ) {
        m.redo();
    }
    my_assert( obj2->value() != 666, "3. Redo" );

    delete obj3;
    m.undo();
    my_assert( obj2->value() == 666, "3. Undoed current block to obj2 init value" );

    // Half undo. Nevermind what is the number.
    for ( i = 1; i <= 10/2; ++i ) {
        m.undo();
    }
    int obj1_value = obj1->value();
    delete obj2;
    my_assert( obj1->value() == obj1_value, "3. obj1 value has not been changed" );    

    delete obj1;
    my_assert( m.countStateBlocks() == 0, "3. Obj1 removed: none state blocks" );
    my_assert( m.countStates() == 0, "3. Obj1 removed: none states" );    

    bool excp = false;
    try { m.undo(); }
    catch (...) { excp = true; }
    my_assert( excp, "3. Nothing to undo" );    

    excp = false;
    try { m.redo(); }
    catch (...) { excp = true; }
    my_assert( excp, "3. Nothing to redo" );

    std::cout << std::endl;
}



int main ()
{
    const int STACK_SIZE = 10;
    int i = 0;

    start_end_state_block_test();

    ObjectStateManager m;

    {// Scope
        std::cout << ">>>> main undo/redo test" << std::endl;

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
    my_assert( m.countStateBlocks() == 0, "Block vector should be empty" );


    stack_shifting_test();
    block_removing_test();

    std::cout << "\n" << "Passed: " << PASSED << "\nFailed: " << FAILED << "\n";
    return 0;    
}
