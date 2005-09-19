
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

    DoublePoint coord1, coord2;
    QPoint pos1, pos2;
    coord1 = getFirstNode().getPoint();
    coord2 = getLastNode().getPoint();
    pos1.setX ( coord1.x() * scaleMultX + leftWindowIndent );
    pos1.setY ( flipY ? ( trussAreaHeight - coord1.y() ) * scaleMultY + topWindowIndent :
              coord1.y() * scaleMultY + topWindowIndent );
    pos2.setX ( coord2.x() * scaleMultX + leftWindowIndent );
    pos2.setY ( flipY ? ( trussAreaHeight - coord2.y() ) * scaleMultY + topWindowIndent :
             coord2.y() * scaleMultY + topWindowIndent );
    if ( isHighlighted () )
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth + 5, 
                  agg::rgba(200, 135, 15, 0.45) );
    if ( drawingStatus )
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth, agg::rgba(0, 0, 0) );
    else 
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth - 1, agg::rgba(0, 0, 0) );
}

/****************************************************************************/
