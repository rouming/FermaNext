
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Pivot
 *****************************************************************************/

TrussPivot::TrussPivot ( ObjectStateManager* mng ) :
    Pivot<TrussNode>(mng), drawingStatus(true)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removePivotHighlight()) );   
}

TrussPivot::TrussPivot ( TrussNode& first, TrussNode& last,
                         ObjectStateManager* mng) :
    Pivot<TrussNode>(first, last, mng), drawingStatus(true)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removePivotHighlight()) );   
}

bool TrussPivot::getDrawingStatus () const
{
    return drawingStatus;
}

void TrussPivot::setDrawingStatus ( bool status )
{
    drawingStatus = status;
}

void TrussPivot::setHighlighted ( bool h )
{
    getFirstNode().setHighlighted ( h );
    getLastNode().setHighlighted ( h );
    PaintableTrussElement::setHighlighted ( h );
}

void TrussPivot::removePivotHighlight ()
{
    setHighlighted ( false );
}

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

void TrussPivot::drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2,
                          int width, color_type color ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( width ); 
    ras.add_path ( stroke );
    solidRend.color ( color );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussPivot::paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                        int trussAreaHeight) const
{
    if ( !isVisible() )
        return;

    bool flipY = true;
    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;

    QPoint p1, p2;
    p1 = getFirstNode().getPoint ();
    p2 = getLastNode().getPoint ();
    p1.setX ( int(p1.x() * scaleMultX) + leftWindowIndent );
    p1.setY ( flipY ? int(( trussAreaHeight - p1.y() ) * scaleMultY) + topWindowIndent :
             int(p1.y() * scaleMultY) + topWindowIndent );
    p2.setX ( int(p2.x() * scaleMultX) + leftWindowIndent );
    p2.setY ( flipY ? int(( trussAreaHeight - p2.y() ) * scaleMultY) + topWindowIndent :
             int(p2.y() * scaleMultY) + topWindowIndent );
    if ( isHighlighted () )
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth + 5, 
                  agg::rgba(200, 135, 15, 0.45) );
    if ( drawingStatus )
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth, agg::rgba(0, 0, 0) );
    else 
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth - 1, agg::rgba(0, 0, 0) );
}

/****************************************************************************/
