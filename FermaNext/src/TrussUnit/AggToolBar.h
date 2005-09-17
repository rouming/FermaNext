
#ifndef AGGTOOLBAR_H
#define AGGTOOLBAR_H

#include "AggButton.h"
#include "qstring.h"
#include "qpoint.h"
#include "qsize.h"
#include <qobject.h>
#include <vector>

/*****************************************************************************/

class AggToolBarButton : public AggButton
{
public:
    AggToolBarButton ( QString fname, QString label_, QPoint leftTopPos, 
                       int width, int height );
    virtual ~AggToolBarButton ();

    virtual void paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                         agg::scanline_p8& sl, solidRenderer& solidRend,
                         agg::trans_affine& mtx, double scaleX, double scaleY  ) const;
private:
    mutable pathRenderer pathRend;
    color_type fillCol, lineCol, highlightFill, highlightLine;
};

/*****************************************************************************/

class AggToolBar : public QObject
{
    Q_OBJECT
public:
    typedef std::vector<AggToolBarButton*> ButtonList;
    typedef ButtonList::iterator ButtonListIter;
    typedef ButtonList::const_iterator ButtonListConstIter;

    AggToolBar ( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                 int bordBottom, int separation_ );
    virtual ~AggToolBar ();

    AggToolBarButton& addButton ( QString fname, QString label, QPoint leftTopPos, 
                                  int width, int height, QObject* widget, 
                                  char* signal, char* slot );
    virtual void removeButton ( QString label );
    virtual void clearToolBar ();

    ButtonList getButtonList () const;

    virtual void setCenterPoint ( QPoint pos );
    virtual QPoint getCenterPoint () const; 

    virtual QPoint getLeftTopPos () const;

    virtual void setButtonSeparation ( int separation_ );
    virtual int getButtonSeparation () const;

    virtual void setBorderLeft ( int width );
    virtual int getBorderLeft () const;

    virtual void setBorderRight ( int width );
    virtual int getBorderRight () const;

    virtual void setBorderBottom ( int width );
    virtual int getBorderBottom () const;

    virtual void setBorderTop ( int width );
    virtual int getBorderTop () const;

    virtual bool isVisible () const;
    virtual void setVisible ( bool );
    virtual bool isRendered () const;

    virtual void paint ( base_renderer &baseRenderer ) const = 0;

signals:
    void onRenderingStatusCleared ();

protected slots:
    void releaseButtons ();
    void clearToolBarRenderedFlag ();

protected:
    virtual void removeButtonHighlight ();
    virtual int findMaxButtonHeight () const;
    virtual int getToolBarWidth () const;
    virtual int getToolBarHeight () const;
    virtual rbuf_dynarow& getRenderingBuffer () const;
    virtual void setRendered ( bool ) const;

private:
    mutable rbuf_dynarow* toolBarBuf;
    mutable bool renderedFlag;
    bool visible;
    ButtonList buttons;
    // tool bar main geometry
    QPoint toolBarLeftTopPos, centerPos;
    int borderLeft, borderRight, borderTop, borderBottom, separation; 
    int toolBarWidth, toolBarHeight;
};

#endif //AGGTOOLBAR_H
