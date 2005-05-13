
#include "TrussUnit.h"
#include <algorithm>


/*****************************************************************************
 * Paintable Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(false), 
    highlighted(false)
{}

PaintableTrussElement::PaintableTrussElement ( bool h_, bool v_ ) :
    visible(v_), 
    highlighted(h_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    visible = v_;
}

void PaintableTrussElement::setHighlighted ( bool h_ )
{
    highlighted = h_;
}

bool PaintableTrussElement::isVisible () const
{
    return visible;
}

bool PaintableTrussElement::isHighlighted () const
{
    return highlighted;
}


/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit () :
    area( 300, 300 )
{}

TrussUnit::~TrussUnit ()
{}


const QString& TrussUnit::getTrussName () const
{
    return trussName;
}

int TrussUnit::getNodesRadius () const
{
    return nodesRadius;
}

void TrussUnit::setTrussName ( const QString& name )
{
    QString old(trussName);
    trussName = name;
    emit onTrussNameChange( old, trussName );
}

void TrussUnit::setNodesRadius ( int rad )
{
    nodesRadius = rad;
    NodeList nodeList = getNodeList ();
    NodeList::iterator iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
        (*iter)->setRadius ( rad );
    emit onNodesRadiusChange( rad );
}

void TrussUnit::setPivotsWidth ( int wid )
{
    PivotList pivotList = getPivotList ();
    PivotList::iterator iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
        (*iter)->setPivotWidth ( wid );
    emit onPivotsWidthChange( wid );
}

void TrussUnit::setArea ( const QSize& a )
{    
    area = a;
    emit onAreaChange( area );
}

const QSize& TrussUnit::getArea ()
{
    return area;
}

void TrussUnit::paint ( base_renderer& baseRend, solid_renderer& solidRend,
                        text_renderer& textRend, scanline_rasterizer& ras, 
                        agg::scanline_p8& sl, agg::ellipse& ell ) const
{
    PivotList pivotList = getPivotList ();
    PivotList::const_iterator pivotsIter = pivotList.begin();
    for ( ; pivotsIter != pivotList.end(); ++pivotsIter )
        (*pivotsIter)->paint ( baseRend, solidRend, textRend, ras, sl, ell );
    NodeList nodeList = getNodeList ();
    NodeList::const_iterator nodesIter = nodeList.begin();
    for ( ; nodesIter != nodeList.end(); ++nodesIter )
        (*nodesIter)->paint ( baseRend, solidRend, textRend, ras, sl, ell );
}

/****************************************************************************/


