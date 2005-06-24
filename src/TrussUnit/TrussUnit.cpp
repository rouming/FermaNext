
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
    trussAreaSize( 300, 300 )
{}

TrussUnit::~TrussUnit ()
{}

const QString& TrussUnit::getTrussName () const
{
    return trussName;
}

void TrussUnit::setTrussName ( const QString& name )
{
    QString old(trussName);
    trussName = name;
    emit onTrussNameChange( old, trussName );
}

void TrussUnit::setTrussAreaSize ( const QSize& area )
{    
    trussAreaSize = area;
    emit onAreaChange( trussAreaSize );
}

const QSize& TrussUnit::getTrussAreaSize () const
{
    return trussAreaSize;
}

void TrussUnit::paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                       int trussAreaHeight ) const
{
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;
    PivotList pivotList = getPivotList ();
    PivotList::const_iterator pivotsIter = pivotList.begin();
    for ( ; pivotsIter != pivotList.end(); ++pivotsIter )
        (*pivotsIter)->paint ( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
    NodeList nodeList = getNodeList ();
    NodeList::const_iterator nodesIter = nodeList.begin();
    for ( ; nodesIter != nodeList.end(); ++nodesIter )
        (*nodesIter)->paint ( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
}

/****************************************************************************/


