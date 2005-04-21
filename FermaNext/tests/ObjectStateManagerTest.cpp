
#include "StatefulObject.h"
#include "ObjectStateManager.h"

#include <iostream>

class Node : public StatefulObject<Node>
{
protected:
    Node* self ()  { return this; }
public:
    Node (int x_): x(x_) { }
    Node ( const Node& node ) 
    { 
        x = node.x;
    } 
    void loadState ( const Node& node )
    {
        x = node.x;
    }
    int x;
};

int main ()
{
    const size_t STACK_SIZE = 10;
    ObjectStateManager m(0);

    Node node(666);
    node.saveState(m);

    //Changes
    for ( size_t i=0; i < STACK_SIZE; ++i ) {
        node.x = i+1;
        node.saveState(m);
    }
    std::cout << "Last change: " << node.x << "\n";

    //Full Undo
    for ( size_t i=0; i < STACK_SIZE-1; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << node.x << "\n";
    }
    //Check bounds
    m.undo(); m.undo(); m.undo(); m.undo(); m.undo(); m.undo();    

    //Rollback to initial
    m.undo();
    std::cout << "Initial: " << node.x << "\n";

    //Full Redo
    for ( size_t i=0; i < STACK_SIZE; ++i ) {
        m.redo();
        std::cout << "Redo" << i+1 << ": " << node.x << "\n";
    }

    //Check bounds
    m.redo(); m.redo(); m.redo(); m.redo(); m.redo(); 

    //Half Undo
    for ( size_t i=0; i < STACK_SIZE/2; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << node.x << "\n";
    }    

    //Half Redo
    for ( size_t i=0; i < STACK_SIZE/2; ++i ) {
        m.redo();
        std::cout << "Redo" << i+1 << ": " << node.x << "\n";
    }

    //Half Undo
    for ( size_t i=0; i < STACK_SIZE/2; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << node.x << "\n";
    }

    //New Changes
    for ( size_t i=0; i < STACK_SIZE/2; ++i ) {
        node.x = (i+1)*10;
        node.saveState(m);
    }
    std::cout << "Last new change: " << node.x << "\n";


    //Full Undo
    for ( size_t i=0; i < STACK_SIZE-1; ++i ) {
        m.undo();
        std::cout << "Undo" << i+1 << ": " << node.x << "\n";
    }

    //Rollback to initial
    m.undo();
    std::cout << "Initial: " << node.x << "\n";


    
}

