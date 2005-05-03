
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ()
{}

TrussNode::TrussNode ( const TrussNode& n ) :
    Node(n)
{}

void TrussNode::paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const
{
    // implementation goes here!
}

/****************************************************************************/
