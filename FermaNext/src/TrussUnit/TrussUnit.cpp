
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
    frontNode(0)
{
    // We should render again when state has been changed
    QObject::connect( this, SIGNAL(onStateChange()), 
                            SLOT(trussUnitStateIsChanged()) );
    QObject::connect( this, SIGNAL(afterNodeDesist( Node& )),
                            SLOT(clearFrontNodePointer( Node& )) );
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

void TrussUnit::paintLoad ( const TrussLoad& load, const QPoint& tailPos, 
                            ren_dynarow& baseRend ) const
{
    double x = load.getXForce (),
           y = load.getYForce (), 
           x1 = 1, y1 = 1, sinA;

    if ( x == 0 && y == 0 )
        return;

    if ( x != 0 && y != 0 )
    {
        sinA = fabs ( y / sqrt( x * x + y * y ) );
        y1 = sinA * 1;
        x1 = sqrt( 1 - y1 * y1 );
    }

    QPoint forceDirection;

    if ( x < 0 )
        forceDirection.setX ( - 1 );
    else if ( x > 0 )
        forceDirection.setX ( 1 );

    if ( y < 0 )
        forceDirection.setY ( - 1 );
    else if ( y > 0 )
        forceDirection.setY ( 1 );

    QPoint headPos( int( tailPos.x() + 22 * x1 * forceDirection.x() ), 
                    int( tailPos.y() - 22 * y1 * forceDirection.y() ) );
    
    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    color_type color = agg::rgba( 0, 0, 25 );

    drawArrow ( ras, solidRend, sl, tailPos, headPos, color, 4, 4, 4, 6 ); 
}

void TrussUnit::paint ( ren_dynarow& baseRend, double scaleMultX, 
                        double scaleMultY ) const
{
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::ellipse ell;
    QPoint pos;
    double trussAreaHeight = getTrussAreaSize().height();
    PivotList pivotList = getPivotList ();
    PivotList::const_iterator pivotsIter = pivotList.begin();
    for ( ; pivotsIter != pivotList.end(); ++pivotsIter )
        (*pivotsIter)->paint( baseRend, scaleMultX, scaleMultY, trussAreaHeight );

    TrussUnitLoadCase* loadCase = getLoadCases().getCurrentLoadCase();
    NodeList nodeList = getNodeList ();
    NodeList::const_iterator nodesIter = nodeList.begin();
    for ( ; nodesIter != nodeList.end(); ++nodesIter )
        if ( *nodesIter != frontNode ) {
            TrussNode* node = *nodesIter;
            const DoublePoint& coord = node->getPoint();
            pos.setX( int(coord.x() * scaleMultX) + leftWindowIndent );
            pos.setY( flipY ? int(( trussAreaHeight - coord.y() ) * scaleMultY) + 
                      topWindowIndent : int(pos.y() * scaleMultY) + topWindowIndent );
            if ( loadCase ) 
            {
                TrussLoad* load = loadCase->findLoad( *node );
                if ( load ) paintLoad( *load, pos, baseRend );
            }
            node->paint( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
        }
    if ( frontNode ) {
        if ( loadCase ) 
        {
            const DoublePoint& coord = frontNode->getPoint();
            pos.setX( int(coord.x() * scaleMultX) + leftWindowIndent );
            pos.setY( flipY ? int(( trussAreaHeight - coord.y() ) * scaleMultY) + 
                      topWindowIndent : int(coord.y() * scaleMultY) + topWindowIndent );
            TrussLoad* load = loadCase->findLoad( *frontNode );
            if ( load ) paintLoad( *load, pos, baseRend );
        }
        frontNode->paint( baseRend, scaleMultX, scaleMultY, trussAreaHeight );
    }
}

/****************************************************************************/
