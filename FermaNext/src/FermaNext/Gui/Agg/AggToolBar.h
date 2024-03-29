
#ifndef AGGTOOLBAR_H
#define AGGTOOLBAR_H

#include "AggButton.h"
#include "AggPaintThread.h"

class AggComboBox;
class QBasicTimer;

/*****************************************************************************/

class AggToolBarButton : public AggButton
{
public:
    AggToolBarButton ( const QString& iconPath );
    virtual ~AggToolBarButton ();

    virtual void paint ( ren_dynarow& baseRend ) const;

private:
    mutable pathRenderer pathRend;
    color_type fillCol, lineCol, highlightFill, highlightLine;
};

/*****************************************************************************/

class AggToolBarHideButton : public AggButton
{
public:
    AggToolBarHideButton ();
    virtual ~AggToolBarHideButton ();

    virtual void paint ( ren_dynarow& baseRend ) const;
    
private:
    color_type fillCol, lineCol, highlightFill;
};

/*****************************************************************************/

class AggToolBar : public QObject
{
    Q_OBJECT
public:
    AggToolBar ( QWidget* parentWidget );
    virtual ~AggToolBar ();
    
    void addButton ( AggButton& );
    void removeButton ( AggButton& );
    void removeButton ( int indx );

    AggButton* getButton ( int indx ) const;
    AggButton* getButton ( int x, int y ) const;

    void addComboBox ( AggComboBox& );
    AggComboBox* getComboBox ( int ) const;

    int getWidth () const;
    int getHeight () const;

    QPoint getPosition () const;
    void setPosition ( QPoint );

    bool isVisible () const;
    void setVisible ( bool );

    bool isEnabled () const;
    void setEnabled ( bool );

    bool isRendered () const;

    bool inToolBarRect ( int x, int y, bool bordCheck = false ) const;

    QWidget* parentWidget () const;
  
    virtual void mouseMove ( int x, int y );
    virtual void mousePress ( int x, int y );
    virtual void mouseRelease ( int x, int y );
    
    virtual void paint ( base_renderer &baseRenderer ) const;

protected:
    typedef QList<AggButton*> ButtonList;
    typedef ButtonList::Iterator ButtonListIter;
    typedef ButtonList::ConstIterator ButtonListConstIter;
    typedef QList<AggComboBox*> ComboBoxList;

protected:
    void resize ( int, int );

    QPoint fromGlobalPos ( int, int ) const;
    QPoint fromLocalPos (int, int ) const;

    ButtonList getButtonList () const;

    virtual void timerEvent ( QTimerEvent* );

protected slots:
    void releaseButtons ();
    void removeButtonHighlight ();

    void setToolBarHinted ();
    void clearButtonHint ();

    void clearToolBarRenderedFlag ();

    void hideToolBar ();
    void showToolBar ();

    void updateToolBarGeometry ();

private:
    void clean ();
    void drawButtons ( ren_dynarow& baseRend ) const;

signals:
    void onRenderingStatusCleared ();
    void onHintShowsUp ( const QString& hint, const QPoint pos, bool smooth );
    void onHintHides ( bool smooth );
    void onAnimationPlays ( bool );
    void onShowStatusTip ( const QString& text );

private:
    ButtonList buttons;
    ComboBoxList comboBoxes;

    mutable rbuf_dynarow* toolBarBuf;

    // parent widget
    QWidget* designerWidget;
    
    // current left top position of the button hint
    QPoint hintCurrentPos;

    AggButton* currentHintedButton;

    AggToolBarHideButton* hideButton;

    // timer for the button hints
    QTimer* hintTimer;
    
    // tool bar hide animation timer
    QBasicTimer* animTimer;
    
    // current number of visible pixels of the tool bar
    int pixNumb;

    // global position of the tool bar left top corner   
    QPoint toolBarLeftTopPos;

    // tool bar geometry
    int toolBarWidth, toolBarHeight;
    int borderLeft, borderRight, borderTop, borderBottom; 
    int separator;
    int cornerRadius; 
    
    // tool bar flags
    mutable bool rendered;
    bool visible, enabled;
    bool toolBarIsHinted;
};

#endif //AGGTOOLBAR_H
