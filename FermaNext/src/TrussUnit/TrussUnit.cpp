
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

void TrussUnit::setPosition ( double x1, double y1, double x2, double y2 )
{
	_x1 = x1;
	_y1 = y1;
	_x2 = x2;
	_y2 = y2;
}

double TrussUnit::getX1 () const
{
	return _x1;
}

double TrussUnit::getY1 () const
{
	return _y1;
}

double TrussUnit::getX2 () const
{
	return _x2;
}

double TrussUnit::getY2 () const
{
	return _y2;
}

double TrussUnit::getHeadlineWidth () const
{
	return headWidth;
}

double TrussUnit::getBorderWidth () const
{
	return borderWidth;
}

void TrussUnit::setHeadlineWidth ( double width )
{
	headWidth = width;
}

void TrussUnit::setBorderWidth ( double width )
{
	borderWidth = width;
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

bool TrussUnit::inCanvasRect ( double x, double y )
{
	if ( x >= _x1 && x <= _x2 && y >= _y1 && y <= _y2 )
	{
		return true;
	}
	return false;
}

bool TrussUnit::inHeadlineRect ( double x, double y )
{
	double bordW = getBorderWidth ();
	double headW = getHeadlineWidth ();
	if ( x >= _x1 + bordW && x <= _x2 - bordW && y >= 
		_y1 + bordW && y <= _y1 - bordW + headW)
	{
		return true;
	}
	return false;
}

void TrussUnit::paint ( agg::rendering_buffer& buffer ) const
{
	double bordW = getBorderWidth ();
	double headW = getHeadlineWidth ();
	pixfmt pixf(buffer);
	base_renderer rb(pixf);
	rb.copy_bar ( _x1, _y1, _x2, _y2, getBorderColor() );
	rb.copy_bar ( _x1  + bordW, _y1 + headW, _x2 - bordW, _y2 - bordW, 
			getCanvasColor() );
	rb.copy_bar ( _x1 + bordW, _y1 + bordW, _x2 - bordW, _y1 + bordW + headW, 
				getHeadlineColor() );
}

/****************************************************************************/


