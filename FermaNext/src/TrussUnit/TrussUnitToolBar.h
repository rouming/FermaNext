
#ifndef TRUSSUNITTOOLBAR_H
#define TRUSSUNITTOOLBAR_H

#include "AggToolBar.h"

class TrussUnitToolBar : public AggToolBar
{
public:
    TrussUnitToolBar ( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                       int bordBottom, int separation, int rad );
    ~TrussUnitToolBar ();

    virtual bool inToolBarRect ( int x, int y ) const;
    virtual void checkMouseMoveEvent ( int x, int y );
    virtual void checkMousePressEvent ( int x, int y );

    virtual void drawButtons ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, solidRenderer& solidRend ) const;
    virtual void paint ( base_renderer& baseRenderer ) const;

protected:
    virtual AggToolBarButton* getSelectedButton ( int x, int y ) const;

private:
    int cornerRadius;
    color_type barFirstColor, barMiddleColor, barLastColor, selectionColor; 
};

#endif //TRUSSUNITTOOLBAR_H

