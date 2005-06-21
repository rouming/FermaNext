
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Pivot
 *****************************************************************************/

TrussPivot::TrussPivot () :
    width(4)
{}

TrussPivot::TrussPivot ( TrussNode& first, TrussNode& last ) :
    Pivot<TrussNode>(first, last)
{}

struct line
{
    double x1, y1, x2, y2;
    int f;

    line(double x1_, double y1_, double x2_, double y2_) : 
        x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
        if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};

int TrussPivot::getPivotWidth () const
{
    return width;
}

void TrussPivot::setPivotWidth ( int w )
{
    width = w;
}

void TrussPivot::drawLine ( scanline_rasterizer& ras, solid_renderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2,
                          int width_, color_type color ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( width_ ); 
    ras.add_path ( stroke );
    solidRend.color ( color );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussPivot::paint ( base_renderer& baseRend ) const
{
    solid_renderer solidRend ( baseRend );
    glyph_gen glyph(0);
    text_renderer textRend ( baseRend, glyph );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;
    glyph.font ( agg::verdana17_bold );

    QPoint p1, p2;
    p1 = getFirstNode().getNodeWidgetPosition ();
    p2 = getLastNode().getNodeWidgetPosition ();
    if ( isHighlighted () )
        drawLine ( ras, solidRend, sl, p1, p2, width + 5, agg::rgba(200, 135, 15, 0.45) ); 
    drawLine ( ras, solidRend, sl, p1, p2, width, agg::rgba(0, 0, 0) ); 
}

/****************************************************************************/
