#include "TrussUnit.h"
#include "TrussUnitActions.h"
#include <algorithm>

/*****************************************************************************
 * Paintable Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(true), 
    highlighted(false),
    enabled(true),
    renderedFlag(false)
{}

PaintableTrussElement::PaintableTrussElement ( bool h_, bool v_, bool e_, bool r_ ) :
    visible(v_), 
    highlighted(h_),
    enabled(e_),
    renderedFlag(r_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    if ( visible == v_ ) 
        return;
    rendered(false);
    visible = v_;
    emit onVisibleChange( visible );
}

void PaintableTrussElement::setHighlighted ( bool h_ )
{
    if ( highlighted == h_ )
        return;
    rendered(false);
    highlighted = h_;
    emit onHighlightChange( highlighted );
}

void PaintableTrussElement::setEnabled ( bool e_ )
{
    if ( enabled == e_ )
        return;
    rendered(false);
    enabled = e_;    
    emit onEnableChange( enabled );
}

void PaintableTrussElement::rendered ( bool r_ ) const
{
    renderedFlag = r_;
}

bool PaintableTrussElement::isVisible () const
{
    return visible;
}

bool PaintableTrussElement::isHighlighted () const
{
    return highlighted;
}

bool PaintableTrussElement::isEnabled () const
{
    return enabled;
}

bool PaintableTrussElement::isRendered () const
{
    return renderedFlag;
}

/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit ( const QString& name, ObjectStateManager* mng ) :
    Truss<TrussNode, TrussPivot>(mng),
    trussName(name),
    trussAreaSize( 300, 300 )
{
    // We should render again when state has been changed
    QObject::connect( this, SIGNAL(onStateChange()), 
                            SLOT(trussUnitStateIsChanged()) );
    QObject::connect( this, SIGNAL(afterNodeDesist( Node& )),
                            SLOT(clearFrontNodePointer( Node& )) );
    frontNode = 0;
}

TrussUnit::~TrussUnit ()
{}

void TrussUnit::trussUnitStateIsChanged ()
{
    rendered(false);
}

const QString& TrussUnit::getTrussName () const
{
    return trussName;
}

void TrussUnit::setTrussName ( const QString& name )
{
    trussName = name;    
    emit onTrussNameChange( trussName );
    emit onStateChange();
}

void TrussUnit::setTrussAreaSize ( const QSize& area )
{    
    trussAreaSize = area;    
    emit onAreaChange( trussAreaSize );
    emit onStateChange();
}

const QSize& TrussUnit::getTrussAreaSize () const
{
    return trussAreaSize;
}

void TrussUnit::nodeToFront ( TrussNode& node )
{
    frontNode = &node;
}

void TrussUnit::clearFrontNodePointer ( Node& node )
{
    if ( &node == frontNode )
        frontNode = 0;
}

void TrussUnit::desistAdjoiningPivots ( const TrussNode& node )
{
    // Find all pivots
    PivotList pivotList = findAdjoiningPivots( node, false );
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        if ( ! pivot->isAlive() )
            continue;
        // Save remove pivot action
        ObjectState& state = pivot->createState();
        state.addAction( new TrussPivotRemoveAction( *pivot ) );
        state.save();
        pivot->desist();
    }
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

    TrussUnitLoadCase* loadCase = getLoadCases().getCurrentLoadCase();
    NodeList nodeList = getNodeList ();
    NodeList::const_iterator nodesIter = nodeList.begin();
    for ( ; nodesIter != nodeList.end(); ++nodesIter )
        if ( *nodesIter != frontNode ) {
            TrussNode* node = *nodesIter;
            node->paint( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
            if ( loadCase ) {
                // TODO: write TrussUnit method to paint truss load
                //TrussLoad* load = loadCase->findLoad( *node );
                //if ( load ) paintLoad( *load,  ......  );
            }
        }
    if ( frontNode ) {
        frontNode->paint( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
        if ( loadCase ) {
            // TODO: write TrussUnit method to paint truss load
            //TrussLoad* load = loadCase->findLoad( *frontNode );
            //if ( load ) paintLoad( *load, ...... );
        }
    }
}

/****************************************************************************/
