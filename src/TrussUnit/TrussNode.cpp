
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ()
{}

TrussNode::TrussNode ( const TrussNode& n ) :
    Node(n)
{}

void TrussNode::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}

/****************************************************************************/
