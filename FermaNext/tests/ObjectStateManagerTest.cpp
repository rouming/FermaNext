
#include "StatefulObject.h"
#include "ObjectStateManager.h"

#include <iostream>

int objects = 0;

class SomeObject : public StatefulObject<SomeObject>
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
    const size_t STACK_SIZE = 10;
    size_t i = 0;
    ObjectStateManager m;

    SomeObject obj(666);
    obj.saveState(m);

    //Changes
    for ( i=0; i < STACK_SIZE; ++i ) {
        obj.value(i+1);
        obj.saveState(m);
    }
    std::cout << "Last change: " << obj.value() << "\n";

    //Full Undo
    for ( i=0; i < STACK_SIZE-1; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << obj.value() << "\n";
    }

    //Rollback to initial
    m.undo();

    //Check bounds
    try {
        m.undo();    
    } catch ( ObjectStateManager::UndoException& ) { }

    std::cout << "Initial: " << obj.value() << "\n";

    //Full Redo
    for (  i=0; i < STACK_SIZE; ++i ) {
        m.redo();
        std::cout << "Redo" << i+1 << ": " << obj.value() << "\n";
    }

    //Check bounds
    try {
        m.redo(); 
    } catch ( ObjectStateManager::RedoException& ) { }

    //Half Undo
    for (  i=0; i < STACK_SIZE/2; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << obj.value() << "\n";
    }    

    //Half Redo
    for (  i=0; i < STACK_SIZE/2; ++i ) {
        m.redo();
        std::cout << "Redo" << i+1 << ": " << obj.value() << "\n";
    }

    //Half Undo
    for (  i=0; i < STACK_SIZE/2; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << obj.value() << "\n";
    }

    //New Changes
    for (  i=0; i < STACK_SIZE/2; ++i ) {
        obj.value((i+1)*10);
        obj.saveState(m);
    }
    std::cout << "Last new change: " << obj.value() << "\n";


    //Full Undo
    for (  i=0; i < STACK_SIZE-1; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << obj.value() << "\n";
    }

    //Rollback to initial
    m.undo();
    std::cout << "Initial: " << obj.value() << "\n";


    return 0;    
}

