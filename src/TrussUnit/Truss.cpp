
#include "Truss.h"
#include <algorithm>

/*****************************************************************************
 * Node
 *****************************************************************************/

Truss::Node::Node () : 
    x(0), y(0), fix(FixationLack)
{}

Truss::Node::Node ( int x_, int y_ ) :
    x(x_), y(y_), fix(FixationLack)
{}

Truss::Node::Node ( int x_, int y_, Fixation fix_ ) :
    x(x_), y(y_), fix(fix_)
{}

void Truss::Node::setFixation ( Truss::Node::Fixation fix_ )
{
    fix = fix_;
}

Truss::Node::Fixation Truss::Node::getFixation () const
{
    return fix;
}


/*****************************************************************************
 * Pivot
 *****************************************************************************/

Truss::Pivot::Pivot () :
    first(NULL),
    last(NULL)
{}

Truss::Pivot::Pivot ( const Node& first_, const Node& last_ ) :
    first(&first_),
    last(&last_)
{}

void Truss::Pivot::setFirstNode ( const Truss::Node& first_ )
{
    first = &first_;
}

const Truss::Node& Truss::Pivot::getFirstNode () const
{
    return *first;
}

void Truss::Pivot::setLastNode ( const Truss::Node& last_ )
{
    last = &last_;
}

const Truss::Node& Truss::Pivot::getLastNode () const
{
    return *last;
}

/*****************************************************************************
 * Truss
 *****************************************************************************/

Truss::Truss ()
{}

Truss::~Truss ()
{
    NodeList::iterator itNodes;
    PivotList::iterator itPivots;
    for ( itNodes = nodes.begin(); itNodes != nodes.end();  ++itNodes )
        delete *itNodes;
    for ( itPivots = pivots.begin(); itPivots != pivots.end();  ++itPivots )
        delete *itPivots;
}

Truss::Node& Truss::createNode ()
{
    Node* node = new Node;
    nodes.push_back(node);
    return *node;
}

Truss::Pivot& Truss::createPivot ()
{
    Pivot* pivot = new Pivot;
    pivots.push_back(pivot);
    return *pivot;
}

Truss::Pivot& Truss::createPivot ( const Node& first, 
                                   const Node& last )
{
    Pivot* pivot = new Pivot( first, last );
    pivots.push_back(pivot);
    return *pivot;    
}

bool Truss::removePivot ( const Pivot& pivot )
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

bool Truss::removeNode ( const Node& node )
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

/****************************************************************************/


