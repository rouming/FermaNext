
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

void TrussPivot::paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                        double trussAreaHeight) const
{
    if ( !isVisible() )
        return;

    bool flipY = true;
    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;

    DoublePoint p1, p2;
    p1 = getFirstNode().getPoint();
    p2 = getLastNode().getPoint();
    p1.setX ( p1.x() * scaleMultX + leftWindowIndent );
    p1.setY ( flipY ? ( trussAreaHeight - p1.y() ) * scaleMultY + topWindowIndent :
              p1.y() * scaleMultY + topWindowIndent );
    p2.setX ( p2.x() * scaleMultX + leftWindowIndent );
    p2.setY ( flipY ? ( trussAreaHeight - p2.y() ) * scaleMultY + topWindowIndent :
             p2.y() * scaleMultY + topWindowIndent );
    if ( isHighlighted () )
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth + 5, 
                  agg::rgba(200, 135, 15, 0.45) );
    if ( drawingStatus )
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth, agg::rgba(0, 0, 0) );
    else 
        drawLine ( ras, solidRend, sl, p1, p2, pivotsWidth - 1, agg::rgba(0, 0, 0) );
}

/****************************************************************************/
