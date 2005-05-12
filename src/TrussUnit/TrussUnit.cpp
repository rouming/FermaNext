
#include "TrussUnit.h"
#include <algorithm>


/*****************************************************************************
 * Paintable Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(false), 
    enabled(false)
{}

PaintableTrussElement::PaintableTrussElement ( bool e_, bool v_ ) :
    visible(v_), 
    enabled(e_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    visible = v_;
}

void PaintableTrussElement::setEnabled ( bool e_ )
{
    enabled = e_;
    if ( !enabled ) visible = false;
}

bool PaintableTrussElement::isVisible () const
{
    return visible;
}

bool PaintableTrussElement::isEnabled () const
{
    return enabled;
}


/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit ()
{}

TrussUnit::~TrussUnit ()
{}

const QString& TrussUnit::getTrussName () const
{
    return trussName;
}

void TrussUnit::setTrussName ( const QString& name )
{
    trussName = name;
}

void TrussUnit::setNodesRadius ( int rad )
{
    NodeList nodeList = getNodeList ();
    NodeList::iterator iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
        (*iter)->setRadius ( rad );
}

void TrussUnit::setPivotsWidth ( int wid )
{
    PivotList pivotList = getPivotList ();
    PivotList::iterator iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
        (*iter)->setPivotWidth ( wid );
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


