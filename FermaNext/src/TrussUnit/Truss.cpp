
#include "Truss.h"
#include <algorithm>

/*****************************************************************************
 * Node
 *****************************************************************************/

Node::Node () : 
    x(0), y(0), fix(FixationLack)
{}

Node::Node ( int x_, int y_ ) :
    x(x_), y(y_), fix(FixationLack)
{}

Node::Node ( int x_, int y_, Fixation fix_ ) :
    x(x_), y(y_), fix(fix_)
{}

void Node::setFixation ( Node::Fixation fix_ )
{
    fix = fix_;
}

Node::Fixation Node::getFixation () const
{
    return fix;
}

/*****************************************************************************
 * Pivot
 *****************************************************************************/
/*
Pivot<class Node>::Pivot () :
    first(NULL),
    last(NULL)
{}

Pivot<class Node>::Pivot ( const Node& first_, const Node& last_ ) :
    first(&first_),
    last(&last_)
{}

void Pivot<class Node>::setFirstNode ( const Node& first_ )
{
    first = &first_;
}

const Node& Pivot<class Node>::getFirstNode () const
{
    return *first;
}

void Pivot<class Node>::setLastNode ( const Node& last_ )
{
    last = &last_;
}

const Node& Pivot<class Node>::getLastNode () const
{
    return *last;
}
*/

/*****************************************************************************
 * Truss
 *****************************************************************************/
/*
template <class Node, class Pivot>
Truss<Node, Pivot>::Truss ()
{}

template <class Node, class Pivot>
Truss<Node, Pivot>::~Truss ()
{
    NodeList::iterator itNodes;
    PivotList::iterator itPivots;
    for ( itNodes = nodes.begin(); itNodes != nodes.end();  ++itNodes )
        delete *itNodes;
    for ( itPivots = pivots.begin(); itPivots != pivots.end();  ++itPivots )
        delete *itPivots;
}

template <class Node, class Pivot>
Node& Truss<Node, Pivot>::createNode ()
{
    Node* node = new Node;
    nodes.push_back(node);
    return *node;
}

template <class Node, class Pivot>
Pivot& Truss<Node, Pivot>::createPivot ()
{
    Pivot* pivot = new Pivot;
    pivots.push_back(pivot);
    return *pivot;
}

template <class Node, class Pivot>
Pivot& Truss<class Node, class Pivot>::createPivot ( const Node& first, 
                                                     const Node& last )
{
    Pivot* pivot = new Pivot( first, last );
    pivots.push_back(pivot);
    return *pivot;    
}

template <class Node, class Pivot>
bool Truss<class Node, class Pivot>::removePivot ( const Pivot& pivot )
{
    PivotList::iterator iter = pivots.begin();
    for ( ; iter != pivots.end(); ++iter )
        if ( (*iter) == &pivot ) {
            delete *iter;
            pivots.erase(iter);
            return true;
        }            
    return false;    
}

template <class Node, class Pivot>
bool Truss<class Node, class Pivot>::removeNode ( const Node& node )
{
    NodeList::iterator iter = nodes.begin();
    for ( ; iter != nodes.end(); ++iter )
        if ( (*iter) == &node ) {
            delete *iter;
            nodes.erase(iter);
            return true;
        }            
    return false;    
}

 */
/****************************************************************************/


