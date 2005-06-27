
#include "StatefulObject.h"
#include "ObjectStateManager.h"

#include <iostream>

int objects = 0;
int PASSED = 0, FAILED = 0;

void my_assert( bool val, const std::string& str ) 
{
    if ( val ) ++PASSED;
    else ++FAILED;
    
    std::cout << str << ": " << (val ? "OK\n" : "NOT OK\n");
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

int main ()
{
    const int STACK_SIZE = 10;
    int i = 0;

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

