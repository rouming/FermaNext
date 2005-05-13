
#include "TrussUnitPseudoWindow.h"
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
        ah.head(5, 5, 5, 5);
        s.shorten(w * 2.0);
    }

    void rewind(unsigned id) { c.rewind(id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};


/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnitPseudoWindow::TrussUnitPseudoWindow ( TrussUnit& truss ) :
    trussUnit(truss)
{}

TrussUnitPseudoWindow::~TrussUnitPseudoWindow ()
{}

TrussUnit& TrussUnitPseudoWindow::getTrussUnit ()
{
    return trussUnit;
}

QPoint TrussUnitPseudoWindow::getPoint1 () const
{
    return _point1;
}

QPoint TrussUnitPseudoWindow::getPoint2 () const
{
    return _point2;
}

QPoint TrussUnitPseudoWindow::getTrussAreaPoint1 () const
{
    QPoint point;
    point.setX ( _point1.x() + 14 * bordW );
    point.setY ( _point1.y() + 2* bordW + 2 * headW );
    return point;
}

QPoint TrussUnitPseudoWindow::getTrussAreaPoint2 () const
{
    QPoint point;
    point.setX ( _point2.x() - 10 * bordW );
    point.setY ( _point2.y() - 10 * bordW - headW/2 );
    return point;
}

QPoint TrussUnitPseudoWindow::transCoord ( QPoint point, bool flipY )
{
    const QSize area = trussUnit.getArea();
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

int TrussUnitPseudoWindow::getHeadlineWidth () const
{
	return headW;
}

int TrussUnitPseudoWindow::getBorderWidth () const
{
	return bordW;
}

int TrussUnitPseudoWindow::getMinResizeVal () const
{
    return minResizeVal;
}

bool TrussUnitPseudoWindow::inWindowRect ( int x, int y )
{
	if ( x >= _point1.x() && x <= _point2.x() && y >= _point1.y() && y <= _point2.y() )
	{
		return true;
	}
	return false;
}

bool TrussUnitPseudoWindow::inCanvasRect ( int x, int y )
{
    if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && 
        y >= _point1.y() + bordW + headW && y <= _point2.y() - bordW )
	{
		return true;
	}
	return false;
}

bool TrussUnitPseudoWindow::inHeadlineRect ( int x, int y )
{
	if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && y >= 
		_point1.y() + bordW && y <= _point1.y() + bordW + headW)
	{
		return true;
	}
	return false;
}

bool TrussUnitPseudoWindow::inHorResizeRect ( int x, int y )
{
    if ( x >= _point1.x() && x <= _point1.x() + bordW && 
        y >= _point1.y() + winRoundRad && y <= _point2.y() - winRoundRad ||
        x >= _point2.x() - bordW && x <= _point2.x() && 
        y >= _point1.y() + winRoundRad && y <= _point2.y() - winRoundRad )
    {
        return true;
    }
    return false;
}

bool TrussUnitPseudoWindow::inVerResizeRect ( int x, int y )
{
    if ( x >= _point1.x() + winRoundRad && x <= _point2.x() - winRoundRad && 
        y >= _point1.y() && y <= _point1.y() + bordW ||
        x >= _point1.x() + winRoundRad && x <= _point2.x() - winRoundRad && 
        y >= _point2.y() - bordW && y <= _point2.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitPseudoWindow::inBDiagResizeRect ( int x, int y )
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

bool TrussUnitPseudoWindow::inFDiagResizeRect ( int x, int y )
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

color_type TrussUnitPseudoWindow::getCanvasColor () const
{
	return canvColor;
}

color_type TrussUnitPseudoWindow::getHeadlineColor () const
{
	return headColor;
}

color_type TrussUnitPseudoWindow::getBorderColor () const
{
	return borderColor;
}

void TrussUnitPseudoWindow::setPosition ( QPoint point1, QPoint point2 )
{
    _point1 = point1;
    _point2 = point2;
}

void TrussUnitPseudoWindow::setTrussAreaWindowIndent ( int indent )
{
    trussAreaIndent = indent;
}

void TrussUnitPseudoWindow::setTrussNodesPosition ()
{
    QPoint point;
    TrussUnit::NodeList nodeList = trussUnit.getNodeList ();
    TrussUnit::NodeList::iterator iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
	{
        TrussNode* node = *iter;
        point.setX ( node->getX () );
        point.setY ( node->getY () );
        point = transCoord ( point, true );
        node->setXWidgetPosition ( point.x() );
        node->setYWidgetPosition ( point.y() );
    }
}

void TrussUnitPseudoWindow::setCoordinateLineWidth ( int width )
{
    lineWidth = width;
}

void TrussUnitPseudoWindow::setHeadlineWidth ( int width )
{
	headW = width;
}

void TrussUnitPseudoWindow::setBorderWidth ( int width )
{
	bordW = width;
}

void TrussUnitPseudoWindow::setResEllRad ( int radius )
{
    resEllRad = radius;
}

void TrussUnitPseudoWindow::setWinRoundRad ( int radius )
{
    winRoundRad = radius;
}

void TrussUnitPseudoWindow::setMinResizeVal ( int value )
{
    minResizeVal = value;
}

void TrussUnitPseudoWindow::setCanvasColor ( int r, int g, int b )
{
	canvColor = agg::rgba(r, g, b);
}

void TrussUnitPseudoWindow::setHeadlineColor ( int r, int g, int b )
{
	headColor = agg::rgba(r, g, b);
}

void TrussUnitPseudoWindow::setBorderColor ( int r, int g, int b )
{
	borderColor = agg::rgba(r, g, b);
}

void TrussUnitPseudoWindow::setResEllColor ( int r , int g, int b )
{
    resEllColor = agg::rgba(r, g, b);
}

void TrussUnitPseudoWindow::drawLine ( scanline_rasterizer& ras, solid_renderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( lineWidth ); 
    ras.add_path ( stroke );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitPseudoWindow::drawArrow ( scanline_rasterizer& ras, solid_renderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    arrow newArrow ( newLine, lineWidth );
    ras.add_path ( newArrow );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitPseudoWindow::drawText ( base_renderer& baseRend, text_renderer& textRend,
                           const QString& str, color_type col, QPoint point, 
                           bool flipY ) const 
{
    textRend.color( col );    
    textRend.render_text ( point.x(), point.y(), str.ascii(), flipY );
}

void TrussUnitPseudoWindow::drawTrussArea ( base_renderer& baseRend, scanline_rasterizer& ras,
                                text_renderer& textRend, solid_renderer& solidRend, 
                                agg::scanline_p8& sl ) const
{
    QPoint p1, p2;
    p2 = getTrussAreaPoint1 ();
    p1 = getTrussAreaPoint2 ();
    baseRend.copy_bar ( p2.x(), p2.y(), p1.x(), p1.y(), agg::rgba8(255,255,255) );
    p2.setY ( p2.y() - 6 * bordW );
    p1.setX ( p2.x() );
    p1.setY ( p1.y() + 3 * bordW );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    p1 = getTrussAreaPoint2 ();
    p1.setX ( p2.x() - 3 * bordW );
    p2 = getTrussAreaPoint2 ();
    p2.setX (p2.x() + 6 * bordW );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    p1.setX ( p1.x() - 3 * bordW );
    p1.setY ( p1.y() );
    p2.setY ( p1.y() );
    drawLine ( ras, solidRend, sl, p1, p2 );
    p1 = getTrussAreaPoint2 ();
    p1.setY ( p1.y() + 3 * bordW );
    drawLine ( ras, solidRend, sl, p1, p2 );

    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    double realAreaLen = p2.x() - p1.x();
    double realAreaWid = p2.y() - p1.y();
    double scaleFactorX = realAreaLen / 5;
    double scaleFactorY = realAreaWid / 5;
    p2.setX ( p1.x() - 3 * bordW );
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
    p2.setY ( p1.y() + 3 * bordW );
    for (i = 0; i < 4; i++ )
    {
        p1.setX ( int(p1.x() + scaleFactorX) );
        p2.setX ( p1.x() );
        drawLine ( ras, solidRend, sl, p1, p2 );
    }

    p1 = getTrussAreaPoint1 ();
    p2 = getTrussAreaPoint2 ();
    p1.setX ( p1.x() - 5 * bordW );
    p1.setY ( p2.y() + 5 * bordW );
    drawText  ( baseRend, textRend, "0", agg::rgba(100, 100, 100), p1, true );
}

void TrussUnitPseudoWindow::drawHeadline ( base_renderer& baseRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, gradient_span_alloc& gradSpan,
                               linear_gradient& gradFunc, color_array_type& gradColors,
                               agg::trans_affine& mtx ) const
{
    agg::rounded_rect headline ( _point1.x()  + 3 * bordW, _point1.y() + bordW, 
        _point2.x() - 3 * bordW, _point1.y() + headW, winRoundRad/2 );
    color_type begin ( 190, 180, 100 ); 
    color_type middle ( 180, 170, 170 ); 
    color_type end ( 160, 100, 100 );
    unsigned i;
    for(i = 0; i < 128; ++i)
    {
        gradColors[i] = begin.gradient ( middle, i / 128.0 );
    }
    for(; i < 256; ++i)
    {
        gradColors[i] = middle.gradient ( end, (i - 128) / 128.0 );
    }
    mtx *= agg::trans_affine_scaling ( 1 / 2.0 );
    mtx *= agg::trans_affine_translation ( _point1.y() , _point2.y() );
    mtx.invert ();
    interpolator inter ( mtx );
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, 
                                    gradColors, 0.0, 10.0 );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path ( headline );
    agg::render_scanlines ( ras, sl, gradRend );
}

void TrussUnitPseudoWindow::paint ( base_renderer& baseRend, solid_renderer& solidRend,
                        text_renderer& textRend, scanline_rasterizer& ras, 
                        agg::scanline_p8& sl, agg::ellipse& ell ) const
{
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
    agg::rounded_rect border ( _point1.x(), _point1.y(), 
                               _point2.x(), _point2.y(), winRoundRad );
    border.normalize_radius();
    solidRend.color ( agg::rgba8(255,255,255) );
    agg::conv_stroke<agg::rounded_rect> stroke(border);
    stroke.width ( 1.0 );
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRend);
    ras.add_path ( border );
    solidRend.color ( borderColor );
    agg::render_scanlines(ras, sl, solidRend);
    agg::rounded_rect canvas ( _point1.x()  + bordW, _point1.y() + bordW + headW, 
        _point2.x() - bordW, _point2.y() - bordW - headW/2, winRoundRad/3 );
    ras.add_path ( canvas );
    solidRend.color ( canvColor );
    agg::render_scanlines ( ras, sl, solidRend );


/* TODO
    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    color_array_type gradColors;
    agg::trans_affine mtx;
    drawHeadline ( baseRend, ras, sl, gradSpan, gradFunc, gradColors, mtx );
*/
    QPoint point;
    agg::rounded_rect headline ( _point1.x()  + 3 * bordW, _point1.y() + bordW, 
        _point2.x() - 3 * bordW, _point1.y() + headW, winRoundRad/2 );
    ras.add_path ( headline );
    solidRend.color ( headColor );
    agg::render_scanlines(ras, sl, solidRend);
    point.setX ( _point1.x() + ( _point2.x() - _point1.x() )/2 - 10 * bordW );
    point.setY ( _point1.y() + 5 * bordW );
    drawText ( baseRend, textRend, trussUnit.getTrussName (), 
               agg::rgba(10, 10, 10), point, true );
    drawTrussArea ( baseRend, ras, textRend, solidRend, sl );

    trussUnit.paint ( baseRend, solidRend, textRend, ras, sl, ell );
}

/****************************************************************************/

