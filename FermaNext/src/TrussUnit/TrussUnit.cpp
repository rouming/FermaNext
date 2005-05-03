
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
{
//    resEllHighlight = false;
}

TrussUnit::~TrussUnit ()
{}

void TrussUnit::setPosition ( QPoint point1, QPoint point2 )
{
    _point1 = point1;
    _point2 = point2;
}

QPoint TrussUnit::getPoint1 () const
{
    return _point1;
}

QPoint TrussUnit::getPoint2 () const
{
    return _point2;
}

int TrussUnit::getHeadlineWidth () const
{
	return headWidth;
}

int TrussUnit::getBorderWidth () const
{
	return borderWidth;
}

int TrussUnit::getMinResizeVal () const
{
    return minResizeVal;
}

bool TrussUnit::inWindowRect ( int x, int y )
{
	if ( x >= _point1.x() && x <= _point2.x() && y >= _point1.y() && y <= _point2.y() )
	{
		return true;
	}
	return false;
}

bool TrussUnit::inCanvasRect ( int x, int y )
{
	int bordW = getBorderWidth ();
	int headW = getHeadlineWidth ();
    if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && 
        y >= _point1.y() + bordW + headW && y <= _point2.y() - bordW )
	{
		return true;
	}
	return false;
}

bool TrussUnit::inHeadlineRect ( int x, int y )
{
	int bordW = getBorderWidth ();
	int headW = getHeadlineWidth ();
	if ( x >= _point1.x() + bordW && x <= _point2.x() - bordW && y >= 
		_point1.y() + bordW && y <= _point1.y() + bordW + headW)
	{
		return true;
	}
	return false;
}

bool TrussUnit::inHorResizeRect ( int x, int y )
{
    int bordW = getBorderWidth ();
    if ( x >= _point1.x() && x <= _point1.x() + bordW && 
        y >= _point1.y() + winRoundRad && y <= _point2.y() - winRoundRad ||
        x >= _point2.x() - bordW && x <= _point2.x() && 
        y >= _point1.y() + winRoundRad && y <= _point2.y() - winRoundRad )
    {
        return true;
    }
    return false;
}

bool TrussUnit::inVerResizeRect ( int x, int y )
{
    int bordW = getBorderWidth ();
    if ( x >= _point1.x() + winRoundRad && x <= _point2.x() - winRoundRad && 
        y >= _point1.y() && y <= _point1.y() + bordW ||
        x >= _point1.x() + winRoundRad && x <= _point2.x() - winRoundRad && 
        y >= _point2.y() - bordW && y <= _point2.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnit::inBDiagResizeRect ( int x, int y )
{
    int bordW = getBorderWidth ();
    if ( sqrt( (x - (_point2.x() - bordW)) * (x - (_point2.x() - bordW)) +
        (y - (_point1.y() + bordW)) * (y - (_point1.y() + bordW)) ) <= resEllRad + 2 ||
        sqrt( (x - (_point1.x() + bordW)) * (x - (_point1.x() + bordW)) +
        (y - (_point2.y() - bordW)) * (y - (_point2.y() - bordW)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

bool TrussUnit::inFDiagResizeRect ( int x, int y )
{
    int bordW = getBorderWidth ();
    if ( sqrt( (x - (_point1.x() + bordW)) * (x - (_point1.x() + bordW)) +
        (y - (_point1.y() + bordW)) * (y - (_point1.y() + bordW)) ) <= resEllRad + 2 ||
        sqrt( (x - (_point2.x() - bordW)) * (x - (_point2.x() - bordW)) +
        (y - (_point2.y() - bordW)) * (y - (_point2.y() - bordW)) ) <= resEllRad + 2 )
    {
        return true;
    }
    return false;
}

color_type TrussUnit::getCanvasColor () const
{
	return canvColor;
}

color_type TrussUnit::getHeadlineColor () const
{
	return headColor;
}

color_type TrussUnit::getBorderColor () const
{
	return borderColor;
}

void TrussUnit::setHeadlineWidth ( int width )
{
	headWidth = width;
}

void TrussUnit::setBorderWidth ( int width )
{
	borderWidth = width;
}

void TrussUnit::setResEllRad ( int radius )
{
    resEllRad = radius;
}

void TrussUnit::setWinRoundRad ( int radius )
{
    winRoundRad = radius;
}

void TrussUnit::setMinResizeVal ( int value )
{
    minResizeVal = value;
}

void TrussUnit::setCanvasColor ( int r, int g, int b )
{
	canvColor = agg::rgba(r, g, b);
}

void TrussUnit::setHeadlineColor ( int r, int g, int b )
{
	headColor = agg::rgba(r, g, b);
}

void TrussUnit::setBorderColor ( int r, int g, int b )
{
	borderColor = agg::rgba(r, g, b);
}

void TrussUnit::setResEllColor ( int r , int g, int b )
{
    resEllColor = agg::rgba(r, g, b);
}
/*
void TrussUnit::setResEllHighlight ( bool h )
{
    resEllHighlight = h;
}

void TrussUnit::setHighlightPos ( int x, int y )
{
    highlightPoint.setX ( x );
    highlightPoint.setY ( y );
}
*/
void TrussUnit::paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const
{
	int bordW = getBorderWidth ();
	int headW = getHeadlineWidth ();

    solidRen.color ( resEllColor );
    ell.init ( _point1.x() + bordW, _point1.y() + bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    ell.init ( _point2.x() - bordW, _point1.y() + bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    ell.init ( _point1.x() + bordW, _point2.y() - bordW, resEllRad, resEllRad, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRen );
    ell.init ( _point2.x() - bordW, _point2.y() - bordW, resEllRad, resEllRad, 16 );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, solidRen );
    agg::rounded_rect border ( _point1.x(), _point1.y(), 
                              _point2.x(), _point2.y(), winRoundRad );
    border.normalize_radius();
    solidRen.color ( agg::rgba8(255,255,255) );
    agg::conv_stroke<agg::rounded_rect> stroke(border);
    stroke.width(1.0);
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRen);
    ras.add_path ( border );
    solidRen.color ( borderColor );
    agg::render_scanlines(ras, sl, solidRen);
    agg::rounded_rect canvas ( _point1.x()  + bordW, _point1.y() + bordW + headW, 
        _point2.x() - bordW, _point2.y() - bordW - headW/2, winRoundRad/3 );
    ras.add_path ( canvas );
    solidRen.color ( canvColor );
    agg::render_scanlines(ras, sl, solidRen);
    agg::rounded_rect headline ( _point1.x()  + 3*bordW, _point1.y() + bordW, 
        _point2.x() - 3*bordW, _point1.y() + headW, winRoundRad/2 );
    ras.add_path ( headline );
    solidRen.color ( headColor );
    agg::render_scanlines(ras, sl, solidRen);
/*
    if ( resEllHighlight )
    {
        ell.init ( highlightPoint.x(), highlightPoint.y(), resEllRad, resEllRad, 16 );
        ras.add_path(ell);
        solidRen.color ( agg::rgba8(155,255,155,80)  );
        agg::render_scanlines ( ras, sl, solidRen );
    }
*/
}

/****************************************************************************/


