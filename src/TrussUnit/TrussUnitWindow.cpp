
#include "TrussUnitWindow.h"
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

TrussUnitWindow::TrussUnitWindow ( const QString& name ) :
    TrussUnit(name)
{}

TrussUnitWindow::~TrussUnitWindow ()
{}

QPoint TrussUnitWindow::getPoint1 () const
{
    return _point1;
}

QPoint TrussUnitWindow::getPoint2 () const
{
    return _point2;
}

QPoint TrussUnitWindow::getTrussAreaPoint1 () const
{
    int leftAreaIndent = 40,
        upperAreaIndent = 55;
    QPoint point;
    point.setX ( _point1.x() + leftAreaIndent );
    point.setY ( _point1.y() + upperAreaIndent );
    return point;
}

QPoint TrussUnitWindow::getTrussAreaPoint2 () const
{
    int rigthAreaIndent = 30,
        lowerAreaIndent = 40;
    QPoint point;
    point.setX ( _point2.x() - rigthAreaIndent );
    point.setY ( _point2.y() - lowerAreaIndent );
    return point;
}

QPoint TrussUnitWindow::transCoord ( QPoint point, bool flipY )
{
    const QSize area = getArea();
    QPoint p1, p2;
    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    double realAreaLen = abs ( p2.x() - p1.x() );
    double realAreaWid = abs ( p2.y() - p1.y() );
    double scaleFactorX = realAreaLen / area.height();
    double scaleFactorY = realAreaWid / area.width();
    point.setX ( int(p1.x() + point.x() * scaleFactorX) );
    point.setY ( flipY ? int(p2.y() - point.y() * scaleFactorY) : 
                         int(p1.y() + point.y() * scaleFactorY) );
    return point;
}

int TrussUnitWindow::getHeadlineWidth () const
{
	return headW;
}

int TrussUnitWindow::getBorderWidth () const
{
	return bordW;
}

int TrussUnitWindow::getMinResizeVal () const
{
    return minResizeVal;
}

bool TrussUnitWindow::inWindowRect ( int x, int y )
{
	if ( x >= _point1.x() && x <= _point2.x() && y >= _point1.y() && y <= _point2.y() )
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inCanvasRect ( int x, int y )
{
    if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && 
        y >= _point1.y() + bordW + headW && y <= _point2.y() - bordW )
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inHeadlineRect ( int x, int y )
{
	if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && y >= 
		_point1.y() + bordW && y <= _point1.y() + bordW + headW)
	{
		return true;
	}
	return false;
}

bool TrussUnitWindow::inHorResizeRect ( int x, int y )
{
    if ( x >= _point1.x() && x <= _point1.x() + bordW && 
        y >= _point1.y() + winCornerRadius && y <= _point2.y() - winCornerRadius ||
        x >= _point2.x() - bordW && x <= _point2.x() && 
        y >= _point1.y() + winCornerRadius && y <= _point2.y() - winCornerRadius )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inVerResizeRect ( int x, int y )
{
    if ( x >= _point1.x() + winCornerRadius && x <= _point2.x() - winCornerRadius && 
        y >= _point1.y() && y <= _point1.y() + bordW ||
        x >= _point1.x() + winCornerRadius && x <= _point2.x() - winCornerRadius && 
        y >= _point2.y() - bordW && y <= _point2.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inBDiagResizeRect ( int x, int y )
{
    if ( sqrt( (x - (_point2.x() - bordW)) * (x - (_point2.x() - bordW)) +
        (y - (_point1.y() + bordW)) * (y - (_point1.y() + bordW)) ) <= resEllRad + 2 ||
        sqrt( (x - (_point1.x() + bordW)) * (x - (_point1.x() + bordW)) +
        (y - (_point2.y() - bordW)) * (y - (_point2.y() - bordW)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inFDiagResizeRect ( int x, int y )
{
    if ( sqrt( (x - (_point1.x() + bordW)) * (x - (_point1.x() + bordW)) +
        (y - (_point1.y() + bordW)) * (y - (_point1.y() + bordW)) ) <= resEllRad + 2 ||
        sqrt( (x - (_point2.x() - bordW)) * (x - (_point2.x() - bordW)) +
        (y - (_point2.y() - bordW)) * (y - (_point2.y() - bordW)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inNodeRadius ( int x, int y )
{
    TrussNode* node = findNodeByCoord ( x , y );
    if ( node )
        return true;
    return false;
}

bool TrussUnitWindow::isPivotSelected ( int x, int y )
{
    TrussPivot* pivot = findPivotByCoord ( x, y );
    if ( pivot )
        return true;
    return false;
}

TrussNode* TrussUnitWindow::findNodeByCoord ( int x, int y )
{
    int radius = getNodesRadius ();
    QPoint point;
    TrussUnit::NodeList nodeList = getNodeList ();
    TrussUnit::NodeList:: iterator iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        point = node->getNodeWidgetPosition ();
        if ( abs ( point.x() - x ) < radius && abs ( point.y() - y ) < radius )
            return node;
    }
    return 0;
}

TrussPivot* TrussUnitWindow::findPivotByCoord ( int x, int y )
{
    int eps = 100;
    QPoint p1, p2;
    int width = getPivotsWidth ();
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
	{
        TrussPivot* pivot = *iter;
        p1 = pivot->getFirstNode ().getNodeWidgetPosition ();
        p2 = pivot->getLastNode ().getNodeWidgetPosition ();
        if ( ( abs ( ( x - p1.x() ) * ( p1.y() - p2.y() ) - 
                     ( y - p1.y() ) * ( p1.x() - p2.x() ) ) <= eps ) && 
             ( ( x >= p1.x() && x <= p2.x() || x >= p2.x() && x <= p1.x() ) && 
               ( y >= p1.y() && y <= p2.y() || y >= p2.y() && y <= p1.y() ) ) )
            return pivot;
    }
    return 0;
}

color_type TrussUnitWindow::getCanvasColor () const
{
	return canvColor;
}

color_type TrussUnitWindow::getHeadlineColor () const
{
	return headColor;
}

color_type TrussUnitWindow::getBorderColor () const
{
	return borderColor;
}

void TrussUnitWindow::setPosition ( QPoint point1, QPoint point2 )
{
    _point1 = point1;
    _point2 = point2;
}

void TrussUnitWindow::setNodeHighlight ( int x, int y )
{
    TrussNode* node = findNodeByCoord ( x , y );
    node->setHighlighted ( true );
}

void TrussUnitWindow::setPivotHighlight ( int x, int y )
{
    TrussPivot* pivot = findPivotByCoord ( x , y );
    pivot->setHighlighted ( true );
}

void TrussUnitWindow::removeNodeHighlight ()
{
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        if ( node->isHighlighted () )
            node->setHighlighted ( false );
    }
}

void TrussUnitWindow::removePivotHighlight ()
{
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
	{
        TrussPivot* pivot = *iter;
        if ( pivot->isHighlighted () )
            pivot->setHighlighted ( false );
    }
}

void TrussUnitWindow::setTrussAreaWindowIndent ( int indent )
{
    trussAreaIndent = indent;
}

void TrussUnitWindow::setTrussNodesPosition ()
{
    QPoint point;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        point.setX ( node->getX () );
        point.setY ( node->getY () );
        point = transCoord ( point, true );
        node->setNodeWidgetPosition ( point );
    }
}

void TrussUnitWindow::setCoordinateLineWidth ( int width )
{
    lineWidth = width;
}

void TrussUnitWindow::setHeadlineWidth ( int width )
{
	headW = width;
}

void TrussUnitWindow::setBorderWidth ( int width )
{
	bordW = width;
}

void TrussUnitWindow::setResEllRad ( int radius )
{
    resEllRad = radius;
}

void TrussUnitWindow::setWinCornerRadius ( int radius )
{
    winCornerRadius = radius;
}

void TrussUnitWindow::setMinResizeVal ( int value )
{
    minResizeVal = value;
}

void TrussUnitWindow::setCanvasColor ( int r, int g, int b )
{
	canvColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setHeadlineColor ( int r, int g, int b )
{
	headColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setBorderColor ( int r, int g, int b )
{
	borderColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setResEllColor ( int r , int g, int b )
{
    resEllColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::drawText ( base_renderer& baseRend, text_renderer& textRend,
                           const QString& str, color_type col, QPoint point, 
                           bool flipY ) const 
{
    textRend.color( col );    
    textRend.render_text ( point.x(), point.y(), str.ascii(), flipY );
}

void TrussUnitWindow::drawLine ( scanline_rasterizer& ras, solid_renderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( lineWidth ); 
    ras.add_path ( stroke );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitWindow::drawArrow ( scanline_rasterizer& ras, solid_renderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    arrow newArrow ( newLine, lineWidth );
    ras.add_path ( newArrow );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitWindow::drawOutlineRoundedRect ( solid_renderer& solidRend, 
                                                     scanline_rasterizer& ras,
                                                     agg::scanline_p8& sl, 
                                                     QPoint point1, QPoint point2, 
                                                     int cornerRadius,
                                                     color_type color) const
{
    agg::rounded_rect rectangle ( point1.x(), point1.y(), 
                                  point2.x(), point2.y(), cornerRadius );
    rectangle.normalize_radius();
    solidRend.color ( color );
    agg::conv_stroke<agg::rounded_rect> stroke(rectangle);
    stroke.width ( 1.0 );
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRend);
}

void TrussUnitWindow::drawTrussArea ( base_renderer& baseRend, 
                               scanline_rasterizer& ras, text_renderer& textRend, 
                               solid_renderer& solidRend, agg::scanline_p8& sl ) const
{
    int arrowTailIndent = 10,
        arrowHeadIndent = 16,
        scalePieceLength = 8,
        scaleTextIndent = 15;
    QPoint p1, p2;
    p2 = getTrussAreaPoint1 ();
    p1 = getTrussAreaPoint2 ();
    baseRend.copy_bar ( p2.x(), p2.y(), p1.x(), p1.y(), agg::rgba8(255,255,255) );
    p2.setY ( p2.y() - arrowHeadIndent );
    p1.setX ( p2.x() );
    p1.setY ( p1.y() + arrowTailIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    p1 = getTrussAreaPoint2 ();
    p1.setX ( p2.x() - arrowTailIndent );
    p2 = getTrussAreaPoint2 ();
    p2.setX (p2.x() + arrowHeadIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );

    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    p1.setX ( p1.x() - scalePieceLength );
    p1.setY ( p1.y() );
    p2.setY ( p1.y() );
    drawLine ( ras, solidRend, sl, p1, p2 );
    p1 = getTrussAreaPoint2 ();
    p1.setY ( p1.y() + scalePieceLength );
    drawLine ( ras, solidRend, sl, p1, p2 );

    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    double realAreaLen = p2.x() - p1.x();
    double realAreaWid = p2.y() - p1.y();
    double scaleFactorX = realAreaLen / 5;
    double scaleFactorY = realAreaWid / 5;
    p2.setX ( p1.x() - scalePieceLength );
    unsigned i;
    for (i = 0; i < 4; i++ )
    {
        p1.setY ( int(p1.y() + scaleFactorY) );
        p2.setY ( p1.y() );
        drawLine ( ras, solidRend, sl, p1, p2 );
    }
    p1 = getTrussAreaPoint2 ();
    p2 = getTrussAreaPoint1 ();
    p1.setX ( p2.x() );
    p2.setY ( p1.y() + scalePieceLength );
    for (i = 0; i < 4; i++ )
    {
        p1.setX ( int(p1.x() + scaleFactorX) );
        p2.setX ( p1.x() );
        drawLine ( ras, solidRend, sl, p1, p2 );
    }

    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    p1.setX ( p1.x() - scaleTextIndent );
    p1.setY ( p2.y() + scaleTextIndent );
    drawText  ( baseRend, textRend, "0", agg::rgba(100, 100, 100), p1, true );
}

void TrussUnitWindow::drawHeadline ( base_renderer& baseRend, 
                               solid_renderer& solidRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, gradient_span_alloc& gradSpan,
                               linear_gradient& gradFunc, color_array_type& gradColors,
                               agg::trans_affine& mtx ) const
{
    QPoint p1, p2;
    p1.setX ( _point1.x()  + 3 * bordW );
    p1.setY ( _point1.y() + bordW );
    p2.setX ( _point2.x() - 3 * bordW );
    p2.setY ( _point1.y() + headW );
    agg::rounded_rect headline ( p1.x(), p1.y(), p2.x(), p2.y(), winCornerRadius/2 );
    color_type begin ( 180, 130, 100 ); 
    color_type middle ( 230, 190, 170 ); 
    color_type end ( 150, 90, 80 );
    unsigned i;
    for(i = 0; i < 128; ++i)
    {
        gradColors[i] = begin.gradient ( middle, i / 128.0 );
    }
    for(; i < 256; ++i)
    {
        gradColors[i] = middle.gradient ( end, (i - 128) / 128.0 );
    }

    interpolator inter ( mtx );
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, gradColors, 
                                           _point1.y(), _point1.y() + headW );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path ( headline );
    agg::render_scanlines ( ras, sl, gradRend );

    drawOutlineRoundedRect ( solidRend, ras, sl, p1, p2, 
                             winCornerRadius/2, agg::rgba(30,20,10) ); 
}

void TrussUnitWindow::paint ( base_renderer& baseRend, solid_renderer& solidRend,
                        text_renderer& textRend, scanline_rasterizer& ras, 
                        agg::scanline_p8& sl, agg::ellipse& ell ) const
{
    //draw resize ellipses
    solidRend.color ( resEllColor );
    ell.init ( _point1.x() + bordW, _point1.y() + bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    ell.init ( _point2.x() - bordW, _point1.y() + bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    ell.init ( _point1.x() + bordW, _point2.y() - bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
    ell.init ( _point2.x() - bordW, _point2.y() - bordW, resEllRad, resEllRad, 16 );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, solidRend );

    // draw -window canvas
    drawOutlineRoundedRect ( solidRend, ras, sl, _point1, _point2, 
                             winCornerRadius, agg::rgba(1,1,1) );
    agg::rounded_rect border ( _point1.x(), _point1.y(), 
                               _point2.x(), _point2.y(), winCornerRadius );
    ras.add_path ( border );
    solidRend.color ( borderColor );
    agg::render_scanlines(ras, sl, solidRend);
    agg::rounded_rect canvas ( _point1.x()  + bordW, _point1.y() + bordW + headW, 
        _point2.x() - bordW, _point2.y() - bordW - headW/2, winCornerRadius/3 );
    ras.add_path ( canvas );
    solidRend.color ( canvColor );
    agg::render_scanlines ( ras, sl, solidRend );

    // draw -window headline
    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    color_array_type gradColors;
    agg::trans_affine mtx;
    drawHeadline ( baseRend, solidRend, ras, sl, gradSpan, gradFunc, gradColors, mtx );

    // draw -window title text
    QPoint point;
    point.setX ( _point1.x() + ( _point2.x() - _point1.x() )/2 - 10 * bordW );
    point.setY ( _point1.y() + 18 );
    drawText ( baseRend, textRend, getTrussName (), 
               agg::rgba(1, 1, 1), point, true );

    // draw editable -window area in which canvas truss unit will be painted
    drawTrussArea ( baseRend, ras, textRend, solidRend, sl );

    // draw trusses
    TrussUnit::paint( baseRend, solidRend, textRend, ras, sl, ell );
}

/****************************************************************************/

