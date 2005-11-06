
#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qiconset.h>
#include <vector>

class ToolBarManager;
class QLabel;

/*****************************************************************************
 * Text Tool Button
 *****************************************************************************/

/* Just another tool button with text label alongside pixmap */
class TextToolButton : public  QToolButton
{
public:
    TextToolButton ( const QIconSet&, const QString&, const QString&, QObject*,
                     const char*, QToolBar*, const char* name = 0 );
    QSize sizeHint() const;
protected:
    void drawButtonLabel( QPainter *p );
};

/*****************************************************************************
 * Tabbed Widget
 *****************************************************************************/

class TabbedWidget : public QObject
{
    Q_OBJECT
public:
    TabbedWidget ( ToolBarManager& parent,  QWidget&, 
                   const QString& name, const QIconSet& iconSet );

    ~TabbedWidget ();

    bool isActive () const;
    bool isShown () const;

    QWidget& getWidget ();

protected:
    void saveWidgetState ();
    void loadWidgetState ();

public slots:
    void setName ( const QString& );
    void activate ();
    void activate ( bool );
    void disactivate ();
    void show ();
    void hide ();
    void setVisible ( bool );

protected slots:
    void toolBarDestroyed ();
    void showOrHide ();

signals:
    void onActivate ( TabbedWidget& );
    void onDisactivate ( TabbedWidget& );
    void onShow ( TabbedWidget& );
    void onHide ( TabbedWidget& );

private:
    ToolBarManager& toolBar;
    QString name;
    QWidget& widget;
    TextToolButton* button;
    uint windowState;
    bool toolBarIsDestroyed;
};

/*****************************************************************************
 * Tool Bar Manager
 *****************************************************************************/

class ToolBarManager : public QToolBar
{
    Q_OBJECT
public:
    ToolBarManager ( QMainWindow* parent = 0, const char* name = 0 );
    virtual ~ToolBarManager ();

protected:
    void clear ();
    virtual TabbedWidget* findByWidget ( QWidget& ) const;

public slots:
    virtual void activateWidget ( QWidget&, bool activationFlag );
    virtual void activateWidget ( QWidget& );
    virtual void disactivateWidget ( QWidget& );

protected slots:
    virtual TabbedWidget& createTabbedWidget ( QWidget&, 
                                               const QString& name, 
                                               const QIconSet& iconSet );

    virtual void addWidget ( QWidget& ) = 0;
    virtual void removeWidget ( QWidget& );

private:
    typedef std::vector<TabbedWidget*> TabbedWidgetList;
    typedef TabbedWidgetList::iterator TabbedWidgetListIter;
    typedef TabbedWidgetList::const_iterator TabbedWidgetListConstIter;

    TabbedWidgetList widgets;
    QLabel* blankLabel;
};

#endif //TOOLBARMANAGER_H
