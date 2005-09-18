
#ifndef TRUSSUNITTOOLBAR_H
#define TRUSSUNITTOOLBAR_H

#include "AggToolBar.h"
#include "qtimer.h"

/*****************************************************************************/

class AggToolBarHideButton : public AggButton
{
public:
    AggToolBarHideButton ( QPoint leftTopPos, int width, int height );
    virtual ~AggToolBarHideButton ();
    virtual void paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                         agg::scanline_p8& sl, solidRenderer& solidRend  ) const;
private:
    color_type fillCol, lineCol, highlightFill;
};

/*****************************************************************************/

class TrussUnitToolBar : public AggToolBar
{
    Q_OBJECT
public:
    TrussUnitToolBar ( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                       int bordBottom, int separation, int rad );
    ~TrussUnitToolBar ();

    void addHideButton ( QPoint leftTopPos, int width, int height, QObject* widget );
    AggToolBarHideButton* getHideButton () const;
    virtual void removeHideButton ();

    virtual QPoint getDynarowBufPos ( int x, int y ) const;
    virtual bool inToolBarRect ( int x, int y ) const;

    virtual void checkMouseMoveEvent ( int x, int y );
    virtual void checkMousePressEvent ( int x, int y );

    virtual void drawButtons ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, solidRenderer& solidRend ) const;
    virtual void paint ( base_renderer& baseRenderer ) const;

protected slots:
    void hideToolBar ();
    void showToolBar ();
    void changeToolBarPosition ();

protected:
    virtual void removeButtonHighlight ();
    virtual AggToolBarButton* getSelectedButton ( int x, int y ) const;

private:
    QPoint topPos;
    bool enabled;
    QTimer* timer;
    int cornerRadius;
    color_type barFirstColor, barMiddleColor, barLastColor, selectionColor;
    AggToolBarHideButton* hideButton;
};

#endif //TRUSSUNITTOOLBAR_H

