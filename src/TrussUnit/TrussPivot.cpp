
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

void TrussPivot::paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const
{
    // implementation goes here!
}

/****************************************************************************/
