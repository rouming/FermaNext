
#include "StatefulObject.h"
#include "ObjectStateManager.h"

#include <iostream>

int objects = 0;

void my_assert( bool val, const std::string& str ) 
{
    std::cout << str << ": " << (val ? "OK\n" : "NOT OK\n");
}

class SomeObject : public ConcreteStatefulObject<SomeObject>
{
protected:
    SomeObject* self ()  { return this; }
public:
    SomeObject (int val_): val(val_) { ++objects; }
    ~SomeObject () { --objects; }
    SomeObject ( const SomeObject& obj ) 
    { 
        ++objects;
        val = obj.val;
    } 
    void loadState ( const SomeObject& obj )
    {
        val = obj.val;
    }
    int value () const { return val; }
    void value ( int val_ ) { val = val_; }    
private:
    int val;
};

int main ()
{
    const int STACK_SIZE = 10;
    int i = 0;

    
    {// Scope
        ObjectStateManager m;
        
        SomeObject obj(666);
        obj.saveState(m);

        //Changes
        for ( i=0; i < STACK_SIZE; ++i ) {
            obj.value(i+1);
            obj.saveState(m);
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
            obj.saveState(m);
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

        SomeObject* anotherObj = new SomeObject(101010);
        anotherObj->saveState(m);
        anotherObj->saveState(m);
        anotherObj->saveState(m);
        anotherObj->saveState(m);
        delete anotherObj;
        

        
    }//Scope end
    my_assert( objects == 0, "All objects should be destroyed" );

    return 0;    
}

