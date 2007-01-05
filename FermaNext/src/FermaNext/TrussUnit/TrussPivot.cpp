
#include "TrussUnit.h"

//#include "../FermaNextWorkspace.h"

/*****************************************************************************
 * Truss Pivot
 *****************************************************************************/

TrussPivot::TrussPivot ( ObjectStateManager* mng, 
                         const TrussMaterialLibrary& mLib ) :
    Pivot<TrussNode>(mng, mLib), 
    drawingStatus(true)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removePivotHighlight()) );  
    setMaterial( materialLibrary().getSelectedMaterial() );
}

TrussPivot::TrussPivot ( TrussNode& first, TrussNode& last,
                         ObjectStateManager* mng,
                         const TrussMaterialLibrary& mLib ) :
    Pivot<TrussNode>(first, last, mng, mLib), 
    drawingStatus(true)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removePivotHighlight()) );   
    setMaterial( materialLibrary().getSelectedMaterial() );
}

void TrussPivot::loadFromXML ( const QDomElement& pivotElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( pivotElem );

    /** 
     * Set thickness
     *****************/
    if ( ! pivotElem.hasAttribute( "thickness" ) )
        throw LoadException();

    bool ok;
    double thickness = pivotElem.attribute( "thickness" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setThickness( thickness );

     /** 
     * Set material
     *****************/
    if ( ! pivotElem.hasAttribute( "materialID" ) )
        return;

    TrussMaterial* material = 
        materialLibrary().getMaterial( pivotElem.attribute( "materialID" ) );
    if ( ! material )
        throw LoadException();
    
    setMaterial( material );
}

QDomElement TrussPivot::saveToXML ( QDomDocument& doc )
{
    QDomElement pivotElem = XMLSerializableObject::saveToXML( doc );
    pivotElem.setTagName( "TrussPivot" );

    /** 
     * Save nodes uuids
     *******************/
    pivotElem.setAttribute( "firstNodeId", getFirstNode().getUUID() );
    pivotElem.setAttribute( "lastNodeId", getLastNode().getUUID() );

    /** 
     * Save thickness
     *****************/
    pivotElem.setAttribute( "thickness", getThickness() );

    /** 
     * Save material
     *****************/
    const TrussMaterial* m = getMaterial();
    if ( ! m )
        return pivotElem;
    pivotElem.setAttribute( "materialID", m->getUUID() );

    return pivotElem;
}

bool TrussPivot::getDrawingStatus () const
{
    return drawingStatus;
}

void TrussPivot::setDrawingStatus ( bool status )
{
    drawingStatus = status;
    emit onDrawingStatusChange( status );
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

    solidRenderer solidRend( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::ellipse ell;

    DoublePoint pos1, pos2;
    const DoublePoint& coord1 = getFirstNode().getPoint();
    const DoublePoint& coord2 = getLastNode().getPoint();
    pos1.setX( coord1.x() * scaleMult.x() + Global::trussBufIndent);
    pos1.setY( Global::flipY ? ( trussAreaHeight - coord1.y() ) * 
               scaleMult.y() + Global::trussBufIndent :
               coord1.y() * scaleMult.y() + Global::trussBufIndent );
    pos2.setX( coord2.x() * scaleMult.x() + Global::trussBufIndent );
    pos2.setY( Global::flipY ? ( trussAreaHeight - coord2.y() ) * 
               scaleMult.y() + Global::trussBufIndent:
               coord2.y() * scaleMult.y() + Global::trussBufIndent );
    if ( isHighlighted () )
        drawLine( ras, solidRend, sl, pos1, pos2, Global::pivotsWidth + 5, 
                  agg::rgba(200, 135, 15, 0.6) );
    if ( drawingStatus )
        drawLine( ras, solidRend, sl, pos1, pos2, 
                  Global::pivotsWidth, agg::rgba(0, 0, 0) );
    else 
        drawLine( ras, solidRend, sl, pos1, pos2, 
                  Global::pivotsWidth - 1, agg::rgba(0, 0, 0) );
}

/****************************************************************************/
