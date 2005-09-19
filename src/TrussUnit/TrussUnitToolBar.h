
#ifndef TRUSSUNITTOOLBAR_H
#define TRUSSUNITTOOLBAR_H

#include "AggToolBar.h"
#include "qtimer.h"

/*****************************************************************************/

class AggToolBarHideButton : public AggButton
{
public:
    AggToolBarHideButton ();
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

    virtual AggToolBarButton& addButton ( const QString& fname, 
                                          const QString& label, 
                                          QPoint leftTopPos, 
                                          uint width, uint height, 
                                          QObject* widget, 
                                          const char* signal, 
                                          const char* slot );

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
    virtual QPoint hideButtonPos ();
    virtual void initHideButton ();
    virtual void removeButtonHighlight ();
    virtual AggToolBarButton* getSelectedButton ( int x, int y ) const;

private:
    QPoint topPos;
    int cornerRadius;
    AggToolBarHideButton* hideButton;
    bool enabled;
    QTimer* timer;
    color_type barFirstColor, barMiddleColor, barLastColor, selectionColor;
};

#endif //TRUSSUNITTOOLBAR_H

