
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include <vector>
#include <agg_rendering_buffer.h>
#include "agg_pixfmt_rgb.h"
#include "agg_renderer_base.h"

typedef agg::pixfmt_bgr24 pixfmt;
typedef pixfmt::color_type color_type;
typedef agg::renderer_base<pixfmt> base_renderer;

class TrussNode;
class TrussPivot;

class PaintableTrussElement 
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool e, bool v );
    virtual void setVisible ( bool );
    virtual void setEnabled ( bool );
    virtual bool isVisible () const;
    virtual bool isEnabled () const;
    virtual void paint ( agg::rendering_buffer& ) const = 0;
private:
    bool visible;
    bool enabled;    
};

class TrussUnit : public Truss<TrussNode, TrussPivot>, 
                  public PaintableTrussElement
{
public:
    TrussUnit ();
    virtual ~TrussUnit ();
	void setPosition ( double x1, double y1, double x2, double y2 );
	double getX1 () const;
	double getY1 () const;
	double getX2 () const;
	double getY2 () const;
	double getHeadlineWidth () const;
	double getBorderWidth () const;
	bool inCanvasRect( double x, double y);
	bool inHeadlineRect( double x, double y);
	void setHeadlineWidth ( double width);
	void setBorderWidth ( double width);
	void setCanvasColor ( int r, int g, int b );
	void setHeadlineColor ( int r, int g, int b );
	void setBorderColor ( int r, int g, int b );
	color_type getCanvasColor () const;
	color_type getHeadlineColor () const;
	color_type getBorderColor () const;
    void paint ( agg::rendering_buffer& ) const;
private:
	double _x1, _x2, _y1, _y2;
	double headWidth, borderWidth;
	color_type canvColor, headColor, borderColor;
};

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    void paint ( agg::rendering_buffer& ) const;
};

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    void paint ( agg::rendering_buffer& ) const;
};


#endif //TRUSSUNIT_H
