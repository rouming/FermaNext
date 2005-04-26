
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Pivot
 *****************************************************************************/

TrussPivot::TrussPivot ()
{}

TrussPivot::TrussPivot ( TrussNode& first, TrussNode& last ) :
    Pivot<TrussNode>(first, last)
{}

TrussPivot::TrussPivot ( const TrussPivot& p ) :
    Pivot<TrussNode>(p)
{}

void TrussPivot::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}

/****************************************************************************/
