
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

void TrussPivot::paint ( ren_dynarow& baseRend, const DoublePoint& scaleMult,
                         double trussAreaHeight) const
{
    if ( !isVisible() )
        return;

    bool flipY = true;
    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;

    QPoint pos1, pos2;
    const DoublePoint& coord1 = getFirstNode().getPoint();
    const DoublePoint& coord2 = getLastNode().getPoint();
    pos1.setX( int(coord1.x() * scaleMult.x() + trussBufIndent) );
    pos1.setY( flipY ? int(( trussAreaHeight - coord1.y() ) * 
                             scaleMult.y() + trussBufIndent ) :
                       int( coord1.y() * scaleMult.y() + trussBufIndent ) );
    pos2.setX( int(coord2.x() * scaleMult.x() + trussBufIndent) );
    pos2.setY( flipY ? int(( trussAreaHeight - coord2.y() ) * 
                             scaleMult.y() + trussBufIndent ):
                       int(coord2.y() * scaleMult.y() + trussBufIndent ) );
    if ( isHighlighted () )
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth + 5, 
                  agg::rgba(200, 135, 15, 0.6) );
    if ( drawingStatus )
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth, agg::rgba(0, 0, 0) );
    else 
        drawLine ( ras, solidRend, sl, pos1, pos2, pivotsWidth - 1, agg::rgba(0, 0, 0) );
}

/****************************************************************************/
