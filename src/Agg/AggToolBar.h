
#ifndef AGGTOOLBAR_H
#define AGGTOOLBAR_H

#include "AggButton.h"
#include "AggSubsidiary.h"
#include "AggPaintThread.h"
#include <vector>

/*****************************************************************************/

class AggToolBarButton : public AggButton
{
public:
    AggToolBarButton ( const QString& fname, const QString& label, 
                       QPoint leftTopPos, int width, int height );
    virtual ~AggToolBarButton ();

    virtual void paint ( ren_dynarow& baseRend, 
                         scanline_rasterizer& ras,
                         agg::scanline_p8& sl, 
                         solidRenderer& solidRend,
                         agg::trans_affine& mtx, 
                         double scaleX, double scaleY  ) const;

private:
    mutable pathRenderer pathRend;
    color_type fillCol, lineCol, highlightFill, highlightLine;
};

/*****************************************************************************/

class AggToolBar : public QObject
{
    Q_OBJECT
public:
    AggToolBar ( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                 int bordBottom, int separation_ );
    virtual ~AggToolBar ();

    virtual AggToolBarButton& addButton ( const QString& fname, 
                                          const QString& label, 
                                          QPoint leftTopPos, 
                                          uint width, uint height, 
                                          QWidget* widget, 
                                          const char* signal, 
                                          const char* slot );
    virtual void removeButton ( const QString& label );
    virtual void clearToolBar ();

    void changeCenterPosition ( QPoint );

    virtual void setButtonSeparation ( int );
    virtual int getButtonSeparation () const;

    virtual void setBorderLeft ( int width );
    virtual int getBorderLeft () const;

    virtual void setBorderRight ( int width );
    virtual int getBorderRight () const;

    virtual void setBorderBottom ( int width );
    virtual int getBorderBottom () const;

    virtual void setBorderTop ( int width );
    virtual int getBorderTop () const;

    virtual int getWidth () const;
    virtual int getHeight () const;

    virtual bool isHinted () const;
    virtual void setHinted ( bool );

    virtual bool isVisible () const;
    virtual void setVisible ( bool );
    virtual bool isRendered () const;

    virtual void paint ( base_renderer &baseRenderer ) const = 0;

signals:
    void onRenderingStatusCleared ();
    void onChangeToolBarState ();

protected slots:
    void releaseButtons ();
    void clearToolBarRenderedFlag ();

protected:
    typedef std::vector<AggToolBarButton*> ButtonList;
    typedef ButtonList::iterator ButtonListIter;
    typedef ButtonList::const_iterator ButtonListConstIter;

    virtual void clean ();

    virtual QPoint getPosition () const;
    virtual void setPosition ( QPoint );

    virtual ButtonList getButtonList () const;

    virtual void removeButtonHighlight ();
    virtual int findMaxButtonHeight () const;
    virtual rbuf_dynarow& getRenderingBuffer () const;
    virtual void setRendered ( bool ) const;

private:
    ButtonList buttons;
    // tool bar main geometry
    QPoint toolBarLeftTopPos, centerPos;
    int borderLeft, borderRight, borderTop, borderBottom, separation; 
    uint toolBarWidth, toolBarHeight;
    mutable rbuf_dynarow* toolBarBuf;
    mutable bool renderedFlag;
    bool visible, hinted;
};

#endif //AGGTOOLBAR_H
