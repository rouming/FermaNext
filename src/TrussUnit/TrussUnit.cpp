
#include "TrussUnit.h"
#include <algorithm>

/*****************************************************************************
 * Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(false), 
    enabled(false)
{}

PaintableTrussElement::PaintableTrussElement ( bool e_, bool v_ ) :
    visible(v_), 
    enabled(e_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    visible = v_;
}

void PaintableTrussElement::setEnabled ( bool e_ )
{
    enabled = e_;
    if ( !enabled ) visible = false;
}

bool PaintableTrussElement::isVisible ()
{
    return visible;
}

bool PaintableTrussElement::isEnabled ()
{
    return enabled;
}

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussUnit::Node::Node () : 
    x(0), y(0), fix(FixationLack)
{}

TrussUnit::Node::Node ( int x_, int y_ ) :
    x(x_), y(y_), fix(FixationLack)
{}

TrussUnit::Node::Node ( int x_, int y_, Fixation fix_ ) :
    x(x_), y(y_), fix(fix_)
{}

void TrussUnit::Node::setFixation ( TrussUnit::Node::Fixation fix_ )
{
    fix = fix_;
}

TrussUnit::Node::Fixation TrussUnit::Node::getFixation () const
{
    return fix;
}

void TrussUnit::Node::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}

/*****************************************************************************
 * Truss Pivot
 *****************************************************************************/

TrussUnit::Pivot::Pivot () :
    first(NULL),
    last(NULL)
{}

TrussUnit::Pivot::Pivot ( const Node& first_, const Node& last_ ) :
    first(&first_),
    last(&last_)
{}

void TrussUnit::Pivot::setFirstNode ( const TrussUnit::Node& first_ )
{
    first = &first_;
}

const TrussUnit::Node& TrussUnit::Pivot::getFirstNode () const
{
    return *first;
}

void TrussUnit::Pivot::setLastNode ( const TrussUnit::Node& last_ )
{
    last = &last_;
}

const TrussUnit::Node& TrussUnit::Pivot::getLastNode () const
{
    return *last;
}

void TrussUnit::Pivot::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}

/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit ()
{}

TrussUnit::~TrussUnit ()
{
    NodesList::iterator itNodes;
    PivotsList::iterator itPivots;
    for ( itNodes = nodes.begin(); itNodes != nodes.end();  ++itNodes )
        delete *itNodes;
    for ( itPivots = pivots.begin(); itPivots != pivots.end();  ++itPivots )
        delete *itPivots;
}

TrussUnit::Node& TrussUnit::createNode ()
{
    Node* node = new Node;
    nodes.push_back(node);
    return *node;
}

TrussUnit::Pivot& TrussUnit::createPivot ()
{
    Pivot* pivot = new Pivot;
    pivots.push_back(pivot);
    return *pivot;
}

TrussUnit::Pivot& TrussUnit::createPivot ( const Node& first, 
                                           const Node& last )
{
    Pivot* pivot = new Pivot( first, last );
    pivots.push_back(pivot);
    return *pivot;    
}

bool TrussUnit::removePivot ( const Pivot& pivot )
{
    PivotsList::iterator iter = pivots.begin();
    for ( ; iter != pivots.end(); ++iter )
        if ( (*iter) == &pivot ) {
            delete *iter;
            pivots.erase(iter);
            return true;
        }            
    return false;    
}

bool TrussUnit::removeNode ( const Node& node )
{
    NodesList::iterator iter = nodes.begin();
    for ( ; iter != nodes.end(); ++iter )
        if ( (*iter) == &node ) {
            delete *iter;
            nodes.erase(iter);
            return true;
        }            
    return false;    
}

void TrussUnit::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}


