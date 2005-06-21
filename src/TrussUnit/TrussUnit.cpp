
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

TrussUnit::TrussUnit ( const QString& name ) :
    trussName(name),
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

int TrussUnit::getPivotsWidth () const
{
    return pivotsWidth;
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
    pivotsWidth = wid;
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

const QSize& TrussUnit::getArea () const
{
    return area;
}

void TrussUnit::paint ( base_renderer& baseRend ) const
{
    solid_renderer solidRend ( baseRend );
    glyph_gen glyph(0);
    text_renderer textRend ( baseRend, glyph );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;
    glyph.font ( agg::verdana17_bold );


    PivotList pivotList = getPivotList ();
    PivotList::const_iterator pivotsIter = pivotList.begin();
    for ( ; pivotsIter != pivotList.end(); ++pivotsIter )
        (*pivotsIter)->paint ( baseRend );
    NodeList nodeList = getNodeList ();
    NodeList::const_iterator nodesIter = nodeList.begin();
    for ( ; nodesIter != nodeList.end(); ++nodesIter )
        (*nodesIter)->paint ( baseRend );
}

/****************************************************************************/


