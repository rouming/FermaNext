
#include "TrussUnitWindow.h"
#include "TrussUnitActions.h"
#include <algorithm>


/*****************************************************************************/
struct line
{
    double x1, y1, x2, y2;
    int f;

    line(double x1_, double y1_, double x2_, double y2_) : 
        x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
        if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};

/*****************************************************************************/
struct arrow
{
    typedef agg::conv_stroke<line, agg::vcgen_markers_term>                     stroke_type;
    typedef agg::conv_marker<stroke_type::marker_type, agg::arrowhead>          marker_type;
    typedef agg::conv_concat<stroke_type, marker_type>                          concat_type;

    stroke_type    s;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    arrow(line& l, double w) : 
        s(l),
        ah(),
        m(s.markers(), ah),
        c(s, m)
    {
        s.width(w); 
        ah.head(4, 4, 4, 6);
        s.shorten(w * 2.0);
    }

    void rewind(unsigned id) { c.rewind(id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};


/*****************************************************************************
 * Truss Unit Window
 *****************************************************************************/

TrussUnitWindow::TrussUnitWindow ( const QString& name, ObjectStateManager* mng ) :
    TrussUnit(name, mng),
    rbuf ( new rbuf_dynarow(250,250) ),
    windowSize ( 250, 250),
    headFont (agg::verdana16_bold),
    numbersFont (agg::verdana13)
{
    setCanvasColor ( 8, 10, 12 );
    setHighlighted(false);    
}

TrussUnitWindow::~TrussUnitWindow ()
{
    delete rbuf;
}

void TrussUnitWindow::setHighlighted ( bool h )
{
    if ( h ) {
        setBorderColor( 25,55,65 );
        setResEllColor( 50, 50, 50 );
        setHeadlineFirstColor(180, 130, 100 );
        setHeadlineMiddleColor( 230, 190, 170 );
        setHeadlineLastColor( 150, 90, 80 );
    } else {
        setBorderColor( 40,65,60 );
        setResEllColor( 120, 120, 120 );
        setHeadlineFirstColor( 180, 130, 150 );
        setHeadlineMiddleColor( 230, 210, 200 );
        setHeadlineLastColor( 150, 90, 110 );        
    }
    TrussUnit::setHighlighted(h);
}

QPoint TrussUnitWindow::getWindowLeftTopPos () const
{
    return windowLeftTopPos;
}

QPoint TrussUnitWindow::getWindowRightBottomPos () const
{
    return windowRightBottomPos;
}

QPoint TrussUnitWindow::getTrussAreaLeftTopPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + leftWindowIndent );
    point.setY ( windowLeftTopPos.y() + topWindowIndent );
    return point;
}

QPoint TrussUnitWindow::getTrussAreaRightBottomPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + windowSize.width() - rigthWindowIndent );
    point.setY ( windowLeftTopPos.y() + windowSize.height() - bottomWindowIndent );
    return point;
}

const QSize& TrussUnitWindow::getWindowSize () const
{
    return windowSize;
}

QPoint TrussUnitWindow::getTrussCoordFromWidgetPos ( int x, int y ) const
{
    // TODO: flipY comparison
    double scaleMultX = getScaleMultiplierX ();
    double scaleMultY = getScaleMultiplierY ();
    int absX = int( ( x - windowLeftTopPos.x() - leftWindowIndent ) / scaleMultX );
    int absY = int( ( windowSize.height() - y + windowLeftTopPos.y() - bottomWindowIndent )
                   / scaleMultY );
    QPoint trussCoord ( absX, absY );
    return trussCoord;
}

QPoint TrussUnitWindow::getWidgetPosFromTrussCoord ( int x, int y ) const
{
    double scaleMultX = getScaleMultiplierX ();
    double scaleMultY = getScaleMultiplierY ();
    int widgetX = int ( x * scaleMultX ) + leftWindowIndent + windowLeftTopPos.x();
    int widgetY = ( flipY ? int( ( getTrussAreaSize().height() - y ) * 
                   scaleMultY ) + topWindowIndent + windowLeftTopPos.y() : 
                   int(y * scaleMultY) + topWindowIndent + windowLeftTopPos.y() );
    QPoint widgetCoord ( widgetX, widgetY );
    return widgetCoord;
}

double TrussUnitWindow::getScaleMultiplierX () const
{
    const QSize trussAreaSize = getTrussAreaSize();
    QPoint p1, p2;
    p1 = getTrussAreaLeftTopPos ();
    p2 = getTrussAreaRightBottomPos ();
    double realTrussAreaLen = abs ( p2.x() - p1.x() );
    double scaleMultiplierX = realTrussAreaLen / trussAreaSize.height();
    return scaleMultiplierX;
}

double TrussUnitWindow::getScaleMultiplierY () const
{
    const QSize trussAreaSize = getTrussAreaSize();
    QPoint p1, p2;
    p1 = getTrussAreaLeftTopPos ();
    p2 = getTrussAreaRightBottomPos ();
    double realTrussAreaWid = abs ( p2.y() - p1.y() );
    double scaleMultiplierY = realTrussAreaWid / trussAreaSize.width();
    return scaleMultiplierY;
}

bool TrussUnitWindow::inWindowRect ( int x, int y ) const
{
	if ( x >= windowLeftTopPos.x() && x <= windowRightBottomPos.x() && 
        y >= windowLeftTopPos.y() && y <= windowRightBottomPos.y() )
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inCanvasRect ( int x, int y ) const
{
    if ( x >= windowLeftTopPos.x() + bordWidth && 
        x <= windowRightBottomPos.x() - bordWidth && 
        y >= windowLeftTopPos.y() + bordWidth + headWidth && 
        y <= windowRightBottomPos.y() - bordWidth )
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inHeadlineRect ( int x, int y ) const
{
	if ( x >= windowLeftTopPos.x() + bordWidth && 
        x <= windowRightBottomPos.x() - bordWidth && y >= 
		windowLeftTopPos.y() + bordWidth && 
        y <= windowLeftTopPos.y() + bordWidth + headWidth)
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inTrussAreaRect ( int x, int y ) const
{
	if ( x >= getTrussAreaLeftTopPos().x() && 
        x <= getTrussAreaRightBottomPos().x() &&
        y >= getTrussAreaLeftTopPos().y() &&
        y <= getTrussAreaRightBottomPos().y() )
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inHorResizeRect ( int x, int y ) const
{
    if ( x >= windowLeftTopPos.x() && x <= windowLeftTopPos.x() + bordWidth && 
        y >= windowLeftTopPos.y() + winCornerRadius && 
        y <= windowRightBottomPos.y() - winCornerRadius ||
        x >= windowRightBottomPos.x() - bordWidth && x <= windowRightBottomPos.x() && 
        y >= windowLeftTopPos.y() + winCornerRadius && 
        y <= windowRightBottomPos.y() - winCornerRadius )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inVerResizeRect ( int x, int y ) const
{
    if ( x >= windowLeftTopPos.x() + winCornerRadius && 
        x <= windowRightBottomPos.x() - winCornerRadius && 
        y >= windowLeftTopPos.y() && y <= windowLeftTopPos.y() + bordWidth ||
        x >= windowLeftTopPos.x() + winCornerRadius && 
        x <= windowRightBottomPos.x() - winCornerRadius && 
        y >= windowRightBottomPos.y() - bordWidth && y <= windowRightBottomPos.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inBDiagResizeRect ( int x, int y ) const
{
    if ( sqrt( (x - (windowRightBottomPos.x() - bordWidth)) * 
        (x - (windowRightBottomPos.x() - bordWidth)) +
        (y - (windowLeftTopPos.y() + bordWidth)) * 
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad + 2 ||
        sqrt( (x - (windowLeftTopPos.x() + bordWidth)) * 
        (x - (windowLeftTopPos.x() + bordWidth)) + 
        (y - (windowRightBottomPos.y() - bordWidth)) * 
        (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inFDiagResizeRect ( int x, int y ) const
{
    if ( sqrt( (x - (windowLeftTopPos.x() + bordWidth)) * 
        (x - (windowLeftTopPos.x() + bordWidth)) +
        (y - (windowLeftTopPos.y() + bordWidth)) * 
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad + 2 ||
        sqrt( (x - (windowRightBottomPos.x() - bordWidth)) * 
        (x - (windowRightBottomPos.x() - bordWidth)) +
        (y - (windowRightBottomPos.y() - bordWidth)) * 
        (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inNodeRadius ( int x, int y ) const
{
    TrussNode* node = findNodeByWidgetPos ( x , y );
    if ( node )
        return true;
    return false;
}

bool TrussUnitWindow::isPivotSelected ( int x, int y ) const
{
    TrussPivot* pivot = findPivotByWidgetPos ( x, y, pivotFindingPrecision );
    if ( pivot )
        return true;
    return false;
}

TrussNode* TrussUnitWindow::findNodeByWidgetPos ( int x, int y ) const
{
    QPoint widgetCoord;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        // get node widget coords (in pixels) from its truss (absolute) coords
        widgetCoord = getWidgetPosFromTrussCoord ( node->getX(), node->getY() );
        if ( abs ( widgetCoord.x() - x ) < nodesRadius && 
             abs ( widgetCoord.y() - y ) < nodesRadius )
        {
            return node;
        }
    }
    return 0;
}

TrussPivot* TrussUnitWindow::findPivotByWidgetPos ( int x, int y, int precision ) const
{
    QPoint trussCoord1, trussCoord2, widgetPos1, widgetPos2, firstNodePos, lastNodePos;
    int Y;
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
	{
        TrussPivot* pivot = *iter;
        trussCoord1 = pivot->getFirstNode ().getPoint ();
        trussCoord2 = pivot->getLastNode ().getPoint ();
        widgetPos1 = getWidgetPosFromTrussCoord ( trussCoord1.x(), trussCoord1.y() );
        widgetPos2 = getWidgetPosFromTrussCoord ( trussCoord2.x(), trussCoord2.y() );

        firstNodePos = getWidgetPosFromTrussCoord ( pivot->getFirstNode().getX(),
                                                    pivot->getFirstNode().getY() );
        lastNodePos = getWidgetPosFromTrussCoord ( pivot->getLastNode().getX(),
                                                   pivot->getLastNode().getY() );

        if ( ( x - firstNodePos.x() ) * ( x - firstNodePos.x() ) + 
             ( y - firstNodePos.y() ) * ( y - firstNodePos.y() ) > 
               nodesRadius * nodesRadius && 
             ( x - lastNodePos.x() ) * ( x - lastNodePos.x() ) + 
             ( y - lastNodePos.y() ) * ( y - lastNodePos.y() ) > 
               nodesRadius * nodesRadius )
        {               
            // equation of line
            Y = abs( ( x - widgetPos1.x() ) * ( widgetPos1.y() - widgetPos2.y() ) - 
                     ( y - widgetPos1.y() ) * ( widgetPos1.x() - widgetPos2.x() ) );

            if ( ( x >= widgetPos1.x() && x <= widgetPos2.x() || 
                   x >= widgetPos2.x() && x <= widgetPos1.x() ) && 
                 ( y >= widgetPos1.y() && y <= widgetPos2.y() || 
                   y >= widgetPos2.y() && y <= widgetPos1.y() ) && Y <= precision )
                return pivot;
        }
    }
    return 0;
}

color_type TrussUnitWindow::getCanvasColor () const
{
	return canvColor;
}

color_type TrussUnitWindow::getHeadlineFirstColor () const
{
	return headFirstColor;
}

color_type TrussUnitWindow::getHeadlineMiddleColor () const
{
	return headMiddleColor;
}

color_type TrussUnitWindow::getHeadlineLastColor () const
{
	return headLastColor;
}

color_type TrussUnitWindow::getBorderColor () const
{
	return borderColor;
}

void TrussUnitWindow::setWindowSize ( int width, int height )
{
    QSize oldSize = windowSize;
    windowSize.setWidth ( width );
    windowSize.setHeight ( height );
    rendered(false);
    emit onResize( oldSize, windowSize );
}

void TrussUnitWindow::resize ( QPoint leftTop, QPoint rightBottom )
{
    int dx = rightBottom.x() - leftTop.x();
    int dy = rightBottom.y() - leftTop.y();
    rbuf->init( dx, dy );
    setWindowSize( dx, dy );
    setWindowPosition( leftTop );    
}

void TrussUnitWindow::setWindowPosition ( QPoint pos )
{
    QPoint oldPos = windowLeftTopPos;
    windowLeftTopPos = pos;
    windowRightBottomPos.setX ( windowLeftTopPos.x() + windowSize.width() );
    windowRightBottomPos.setY ( windowLeftTopPos.y() + windowSize.height() );
    rendered(false);
    emit onMove( oldPos, pos );
}

void TrussUnitWindow::setNodeHighlight ( int x, int y )
{
    TrussNode* node = findNodeByWidgetPos ( x , y );
    node->setHighlighted ( true ); 
    rendered(false);
}

void TrussUnitWindow::setPivotHighlight ( int x, int y )
{
    TrussPivot* pivot = findPivotByWidgetPos ( x , y, pivotFindingPrecision );
    pivot->setHighlighted ( true );
    pivot->getFirstNode().setHighlighted ( true );
    pivot->getLastNode().setHighlighted ( true );
    rendered(false);
}

void TrussUnitWindow::removeNodesHighlight ()
{
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        if ( node->isHighlighted () )
        {
            node->setHighlighted ( false );
            rendered(false);
        }
    }
}

void TrussUnitWindow::removePivotsHighlight ()
{
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
	{
        TrussPivot* pivot = *iter;
        if ( pivot->isHighlighted () )
        {
            pivot->setHighlighted ( false );
            pivot->getFirstNode().setHighlighted ( false );
            pivot->getLastNode().setHighlighted ( false );
            rendered(false);
        }
    }
}

void TrussUnitWindow::moveTrussNode ( int x, int y, TrussNode* node )
{
    QPoint newCoord = getTrussCoordFromWidgetPos ( x, y );

    int areaWidth = getTrussAreaSize().width();
    int areaHeight = getTrussAreaSize().height();
    if ( newCoord.x() < 0 )
        newCoord.setX( 0 );
    if ( newCoord.x() > areaWidth )
        newCoord.setX( areaWidth );
    if ( newCoord.y() < 0 )
        newCoord.setY( 0 );
    if ( newCoord.y() > areaHeight )
        newCoord.setY( areaHeight );

    node->setPoint( newCoord.x(), newCoord.y() );
    rendered(false);
}

void TrussUnitWindow::moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                      QPoint firstNodeClickDist, QPoint lastNodeClickDist )
{
    QPoint newCoord = getTrussCoordFromWidgetPos ( x, y );
    int newXFirst = newCoord.x()  + firstNodeClickDist.x();
    int newYFirst = newCoord.y() + firstNodeClickDist.y();
    int newXLast = newCoord.x() + lastNodeClickDist.x();
    int newYLast = newCoord.y() + lastNodeClickDist.y();
    
    QPoint oldFirstCoord = pivot->getFirstNode().getPoint();
    QPoint oldLastCoord  = pivot->getLastNode().getPoint();

    int oldXFirst = pivot->getFirstNode().getPoint().x();
    int oldYFirst = pivot->getFirstNode().getPoint().y();
    int oldXLast  = pivot->getLastNode().getPoint().x();
    int oldYLast  = pivot->getLastNode().getPoint().y();
    int areaWidth = getTrussAreaSize().width();
    int areaHeight = getTrussAreaSize().height();

    if ( newXFirst < 0 )
    {
        newXFirst = 0;
        newXLast = oldXLast - oldXFirst;
    }
    if ( newXLast < 0 )
    {
        newXLast = 0;
        newXFirst = oldXFirst - oldXLast;
    }
    if ( newYFirst < 0 )
    {
        newYFirst = 0;
        newYLast = oldYLast - oldYFirst;
    }
    if ( newYLast < 0 )
    {
        newYLast = 0;
        newYFirst = oldYFirst - oldYLast;
    }
    if ( newXFirst > areaWidth )
    {
        newXFirst = areaWidth;
        newXLast = oldXLast + ( areaWidth - oldXFirst );
    }
    if ( newXLast > areaWidth )
    {
        newXLast = areaWidth;
        newXFirst = oldXFirst + ( areaWidth - oldXLast );
    }
    if ( newYFirst > areaHeight )
    {
        newYFirst = areaHeight;
        newYLast = oldYLast + ( areaHeight - oldYFirst);
    }
    if ( newYLast > areaHeight )
    {
        newYLast = areaHeight;
        newYFirst = oldYFirst + ( areaHeight - oldYLast);
    }

    pivot->getFirstNode().setPoint( newXFirst, newYFirst );
    pivot->getLastNode().setPoint( newXLast, newYLast );
    rendered(false);
}

TrussNode* TrussUnitWindow::nodesMergingComparison ( TrussNode* comparableNode, 
                                                     int precision, bool fixationCheck )
{
    TrussNode* node = findNodeWithSameCoord ( comparableNode, precision );
    if ( node != 0 && node != comparableNode )
    {
        if ( fixationCheck )
        {
            if ( node->getFixation() == comparableNode->getFixation() )
            {
                return node;
            }
        }
        else
            return node;
    }
    rendered(false);
    return 0;
}

void TrussUnitWindow::mergeNodes ( TrussNode* node, TrussNode* mergingNode )
{
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();

    ObjectState& state = mergingNode->createState();

    for ( ; iter != pivotList.end(); ++iter )
	{
        TrussPivot* pivot = *iter;
        TrussNode* firstNode = &pivot->getFirstNode();
        if ( firstNode == mergingNode )
            pivot->setFirstNode ( node );
        TrussNode* lastNode = &pivot->getLastNode();
        if ( lastNode == mergingNode )
            pivot->setLastNode ( node );

        // remove fake pivots
        if ( &pivot->getFirstNode() == &pivot->getLastNode() ||
             findPivotCopy( pivot ) ) {
            removePivot( *pivot );
            // Save remove pivot action
            state.addAction( new TrussPivotRemoveAction( *this, *pivot ) );
        }
    }
    removeNode ( *mergingNode );
    // Save remove node action
    state.addAction( new TrussNodeRemoveAction( *this, *mergingNode ) );
    state.save();

    rendered(false);
}

void TrussUnitWindow::dividePivot ( TrussPivot& dividualPivot, TrussNode& dividingNode )
{
    TrussNode& first = dividualPivot.getFirstNode();
    TrussNode& last = dividualPivot.getLastNode();

    ObjectStateManager* mng = dividualPivot.getStateManager();
    mng->startStateBlock();

    removePivot ( dividualPivot );
    // Save remove pivot action
    ObjectState& state = dividualPivot.createState();
    state.addAction( new TrussPivotRemoveAction( *this, dividualPivot ) );
    state.save();

    TrussPivot* pivot1 = findPivotByNodes ( first, dividingNode );
    TrussPivot* pivot2 = findPivotByNodes ( last, dividingNode );
    // Check if there is another pivot with the same coords. 
    // If it so, we should create only one pivot instead of removed (divided) pivot.
    if ( pivot1 )
    {
        TrussPivot* newlyCreated = 0;
        if ( last.getNumber() < dividingNode.getNumber() )
            newlyCreated = &createPivot( last, dividingNode );
        else
            newlyCreated = &createPivot( dividingNode, last );

        // Save create pivot action
        ObjectState& state = newlyCreated->createState();
        state.addAction( new TrussPivotCreateAction( *this, *newlyCreated ) );
        state.save();
    }
    else if ( pivot2 )
    {
        TrussPivot* newlyCreated = 0;
        if ( first.getNumber() < dividingNode.getNumber() )
            newlyCreated = &createPivot( first, dividingNode );
        else
            newlyCreated = &createPivot( dividingNode, first );

        // Save create pivot action
        ObjectState& state = newlyCreated->createState();
        state.addAction( new TrussPivotCreateAction( *this, *newlyCreated ) );
        state.save();
    }
    // Create two pivots instead of removed one.
    else
    {
        TrussPivot* newlyCreated = 0;

        if ( first.getNumber() < dividingNode.getNumber() )
            newlyCreated = &createPivot( first, dividingNode );
        else
            newlyCreated = &createPivot( dividingNode, first );

        // Save create pivot action
        ObjectState& stateFirst = newlyCreated->createState();
        stateFirst.addAction( new TrussPivotCreateAction( *this, *newlyCreated ) );
        stateFirst.save();

        if ( last.getNumber() < dividingNode.getNumber() )
            newlyCreated = &createPivot( last, dividingNode );
        else
            newlyCreated = &createPivot( dividingNode, last );

        // Save create pivot action
        ObjectState& stateSecond = newlyCreated->createState();
        stateSecond.addAction( new TrussPivotCreateAction( *this, *newlyCreated ) );
        stateSecond.save();
    }
    mng->endStateBlock();

    rendered(false);
}

void TrussUnitWindow::checkAfterNodeManipulation ( TrussNode* selectedNode )
{
    QPoint nodePos = getWidgetPosFromTrussCoord ( selectedNode->getX(), 
                                                    selectedNode->getY() );
    TrussNode* node = nodesMergingComparison( selectedNode, 150, true );
    TrussPivot* pivot = findPivotByWidgetPos ( nodePos.x(), nodePos.y(), 250 );
    if ( node )
        mergeNodes ( node, selectedNode );
    else if ( pivot )
        dividePivot ( *pivot, *selectedNode );
}

void TrussUnitWindow::setCanvasColor ( int r, int g, int b )
{
	canvColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setHeadlineFirstColor ( int r, int g, int b )
{
	headFirstColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setHeadlineMiddleColor ( int r, int g, int b )
{
	headMiddleColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setHeadlineLastColor ( int r, int g, int b )
{
	headLastColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setBorderColor ( int r, int g, int b )
{
	borderColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setResEllColor ( int r , int g, int b )
{
    resEllColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::drawText ( ren_dynarow&, textRenderer& textRend,
                                 const QString& str, color_type col, 
                                 QPoint point ) const 
{
    textRend.color( col );    
    textRend.render_text( point.x(), point.y(), str.ascii(), flipY );
}

void TrussUnitWindow::drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( coordLineWidth ); 
    ras.add_path ( stroke );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitWindow::drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    arrow newArrow ( newLine, coordLineWidth );
    ras.add_path ( newArrow );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitWindow::drawOutlineRoundedRect ( solidRenderer& solidRend, 
                                                     scanline_rasterizer& ras,
                                                     agg::scanline_p8& sl, 
                                                     QPoint point1, QPoint point2, 
                                                     int cornerRadius,
                                                     color_type color) const
{
    agg::rounded_rect rectangle ( point1.x(), point1.y(), 
                                  point2.x(), point2.y(), cornerRadius );
    solidRend.color ( color );
    agg::conv_stroke<agg::rounded_rect> stroke(rectangle);
    stroke.width ( 1.0 );
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRend);
}

void TrussUnitWindow::drawTrussArea ( ren_dynarow& baseRend, 
                               scanline_rasterizer& ras, textRenderer& textRend, 
                               solidRenderer& solidRend, agg::scanline_p8& sl ) const
{
//  draw coordinate lines with arrow heads
    QPoint leftTopAreaPos ( leftWindowIndent, topWindowIndent ),
           rightBottomAreaPos ( windowSize.width() - rigthWindowIndent, 
                                windowSize.height() - bottomWindowIndent );
    QPoint p1, p2;
    p2 = leftTopAreaPos;
    p1 = rightBottomAreaPos;
    baseRend.copy_bar ( p2.x(), p2.y(), p1.x(), p1.y(), agg::rgba8(255,255,255) );
    p2.setY ( p2.y() - arrowHeadIndent );
    p1.setX ( p2.x() );
    p1.setY ( p1.y() + arrowTailIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    p1 = rightBottomAreaPos;
    p1.setX ( p2.x() - arrowTailIndent );
    p2 = rightBottomAreaPos;
    p2.setX (p2.x() + arrowHeadIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX ( p1.x() - scalePieceLength );
    p1.setY ( p1.y() );
    p2.setY ( p1.y() );
    drawLine ( ras, solidRend, sl, p1, p2 );

    p1 = rightBottomAreaPos;
    p1.setY ( p1.y() + scalePieceLength );
    drawLine ( ras, solidRend, sl, p1, p2 );

//  draw scale strokes and numbers
    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    double areaLenInPix = p2.x() - p1.x();
    double areaWidInPix = p2.y() - p1.y();
    double scaleFactorXInPix = areaLenInPix / 5;
    double scaleFactorYInPix = areaWidInPix / 5;
    int scaleFactorXInAbs = getTrussAreaSize().height() / 5;
    int scaleFactorYInAbs = getTrussAreaSize().width() / 5;

    p2.setX ( p1.x() - scalePieceLength );
    QString str;
    unsigned i;
    for (i = 4; i > 0; i-- )
    {
        p1.setY ( int(p1.y() + scaleFactorYInPix) );
        p2.setY ( p1.y() );
        drawLine ( ras, solidRend, sl, p1, p2 );
        QPoint textPos ( p2.x() - 23, p2.y() + 3 );
        str = QString("%1").arg( i * scaleFactorYInAbs );   
        drawText ( baseRend, textRend, str, agg::rgba(100, 100, 100), textPos );
    }

    p1 = rightBottomAreaPos;
    p2 = leftTopAreaPos;
    p1.setX ( p2.x() );
    p2.setY ( p1.y() + scalePieceLength );
    for (i = 0; i < 4; i++ )
    {
        p1.setX ( int(p1.x() + scaleFactorXInPix) );
        p2.setX ( p1.x() );
        drawLine ( ras, solidRend, sl, p1, p2 );
        QPoint textPos ( p2.x() - 10, p2.y() + 10 );
        str = QString("%1").arg( (i + 1) * scaleFactorXInAbs );   
        drawText ( baseRend, textRend, str, agg::rgba(100, 100, 100), textPos );
    }

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX (p1.x() - 23 - scalePieceLength );
    p1.setY (p1.y() + 3 );
    str = QString("%1").arg( getTrussAreaSize().height() );
    drawText  ( baseRend, textRend, str, agg::rgba(100, 100, 100), p1 );
    p2.setX (p2.x() - 10 );
    p2.setY (p2.y() + 10 + scalePieceLength );
    str = QString("%1").arg( getTrussAreaSize().width() );
    drawText  ( baseRend, textRend, str, agg::rgba(100, 100, 100), p2 );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX ( p1.x() - scaleTextLeftBottomIndent );
    p1.setY ( p2.y() + scaleTextLeftBottomIndent );
    drawText  ( baseRend, textRend, "0", agg::rgba(100, 100, 100), p1 );
    
}

void TrussUnitWindow::drawHeadline ( ren_dynarow& baseRend, 
                               solidRenderer& solidRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, gradient_span_alloc& gradSpan,
                               linear_gradient& gradFunc, color_array_type& gradColors,
                               agg::trans_affine& mtx ) const
{
    QPoint p1, p2;
    p1.setX ( 3 * bordWidth );
    p1.setY ( bordWidth );
    p2.setX ( windowSize.width() - 3 * bordWidth );
    p2.setY ( headWidth );
    agg::rounded_rect headline ( p1.x(), p1.y(), p2.x(), p2.y(), winCornerRadius/2 );
    unsigned i;
    for(i = 0; i < 128; ++i)
    {
        gradColors[i] = headFirstColor.gradient ( headMiddleColor, i / 128.0 );
    }
    for(; i < 256; ++i)
    {
        gradColors[i] = headMiddleColor.gradient ( headLastColor, (i - 128) / 128.0 );
    }

    interpolator inter ( mtx );
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, gradColors, 
                                          0, headWidth );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path ( headline );
    agg::render_scanlines ( ras, sl, gradRend );

    drawOutlineRoundedRect ( solidRend, ras, sl, p1, p2, 
                             winCornerRadius/2, agg::rgba(30,20,10) ); 
}

void TrussUnitWindow::paint ( base_renderer& baseRenderer ) const
{
    if ( !isVisible() )
        return;

    glyph_gen glyph(0);
    scanline_rasterizer   ras;    
    agg::scanline_p8     sl;
    agg::ellipse ell;

    pixf_dynarow pixf(*rbuf);

    if ( ! isRendered() ) 
    { 
        ren_dynarow baseRend(pixf);
        solidRenderer solidRend ( baseRend );    
        textRenderer textRend ( baseRend, glyph );

        /*------draw resize ellipses------*/
        solidRend.color ( resEllColor );
        ell.init ( bordWidth, bordWidth, resEllRad, resEllRad, 10 );
        ras.add_path ( ell );
        ell.init ( windowSize.width() - bordWidth, bordWidth, resEllRad, resEllRad, 10 );
        ras.add_path ( ell );
        ell.init ( bordWidth, windowSize.height() - bordWidth, resEllRad, resEllRad, 10 );
        ras.add_path ( ell );
        agg::render_scanlines ( ras, sl, solidRend );
        ell.init ( windowSize.width() - bordWidth, windowSize.height() - bordWidth, 
                  resEllRad, resEllRad, 10 );
        ras.add_path(ell);
        agg::render_scanlines ( ras, sl, solidRend );


        /*------draw window canvas------*/
        QPoint rightBottomPos, leftTopPos ( 0, 0 );
        rightBottomPos.setX ( windowSize.width() );
        rightBottomPos.setY ( windowSize.height() );
        drawOutlineRoundedRect ( solidRend, ras, sl, leftTopPos, rightBottomPos, 
                                winCornerRadius, agg::rgba(1,1,1) );
        agg::rounded_rect border ( leftTopPos.x(), leftTopPos.y(), rightBottomPos.x(),
                                  rightBottomPos.y(), winCornerRadius );
        ras.add_path ( border );
        solidRend.color ( borderColor );
        agg::render_scanlines(ras, sl, solidRend);
        agg::rounded_rect canvas ( bordWidth, bordWidth + headWidth, 
                                  windowSize.width() - bordWidth, 
                                  windowSize.height() - bordWidth - headWidth/2, 
                                  winCornerRadius/3 );
        ras.add_path ( canvas );
        solidRend.color ( canvColor );
        agg::render_scanlines ( ras, sl, solidRend );


        /*------draw window headline------*/
        gradient_span_alloc gradSpan;
        linear_gradient gradFunc;
        color_array_type gradColors;
        agg::trans_affine mtx;
        glyph.font ( headFont );
        drawHeadline ( baseRend, solidRend, ras, sl, gradSpan, gradFunc, gradColors, mtx );


        /*------draw window title text------*/
        QPoint point;
        point.setX ( windowSize.width()/2 - 10 * bordWidth );
        point.setY ( 17 );
        drawText ( baseRend, textRend, getTrussName (), 
               agg::rgba(1, 1, 1), point );


        /*------draw editable area in which canvas truss unit can be painted------*/
        glyph.font ( numbersFont );
        drawTrussArea ( baseRend, ras, textRend, solidRend, sl );


        /*------draw truss unit------*/
        TrussUnit::paint ( baseRend, getScaleMultiplierX (), getScaleMultiplierY (),
                           getTrussAreaSize().height() );

        rendered( true );
    }
    baseRenderer.blend_from ( pixf, 0, windowLeftTopPos.x(), windowLeftTopPos.y(), 
                             unsigned(1.0 * 255) );
}

/****************************************************************************/

