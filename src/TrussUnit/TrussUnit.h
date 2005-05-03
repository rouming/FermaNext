
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include <vector>
#include <qpoint.h> 

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_gamma_lut.h"
#include "agg_ellipse.h"
#include "agg_rounded_rect.h"
#include "agg_conv_stroke.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
typedef agg::rasterizer_scanline_aa<>                scanline_rasterizer;
typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8> gamma_lut_type;
typedef agg::pixfmt_rgb24_gamma<gamma_lut_type> pixfmt;
typedef pixfmt::color_type color_type;
typedef agg::renderer_base<pixfmt> base_renderer;
typedef agg::renderer_scanline_aa_solid<base_renderer>  solid_renderer;
typedef agg::renderer_scanline_bin_solid<base_renderer> draft_renderer;

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
    virtual void paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const = 0;
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
    void setPosition ( QPoint point1, QPoint point2 );
    QPoint getPoint1 () const;
    QPoint getPoint2 () const;
	int getHeadlineWidth () const;
	int getBorderWidth () const;
    int getMinResizeVal () const;
    bool inWindowRect ( int x, int y );
	bool inCanvasRect ( int  x, int  y );
	bool inHeadlineRect ( int  x, int  y );
    bool inHorResizeRect ( int x, int y );
    bool inVerResizeRect ( int x, int y );
    bool inBDiagResizeRect ( int x, int y );
    bool inFDiagResizeRect ( int x, int y );
	color_type getCanvasColor () const;
	color_type getHeadlineColor () const;
	color_type getBorderColor () const;
	void setHeadlineWidth ( int  width );
	void setBorderWidth ( int  width );
    void setMinResizeVal ( int value );
	void setCanvasColor ( int r, int g, int b );
	void setHeadlineColor ( int r, int g, int b );
	void setBorderColor ( int r, int g, int b );
    void setResEllRad ( int radius );
    void setWinRoundRad ( int radius );
    void setResEllColor ( int r, int g, int b );
//    void setResEllHighlight ( bool );
//    void setHighlightPos ( int x, int y );
    void paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const;
private:
	int  headWidth, borderWidth, resEllRad, winRoundRad, minResizeVal;
//    bool resEllHighlight;
    QPoint _point1, _point2;//, highlightPoint;
	color_type canvColor, headColor, borderColor, resEllColor;
};

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    TrussNode ();
    TrussNode ( const TrussNode& );
    void paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const;
};

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    TrussPivot ();
    TrussPivot ( TrussNode&, TrussNode& );
    TrussPivot ( const TrussPivot& );
    void paint ( base_renderer& baseRend, solid_renderer& solidRen, 
                 agg::rasterizer_scanline_aa<>& ras, agg::scanline_p8& sl, 
                 agg::ellipse& ell ) const;
};

#endif //TRUSSUNIT_H
