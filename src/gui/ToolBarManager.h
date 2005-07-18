
#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <qobject.h>
#include <vector>

class QToolBar;
class QWidget;
class QAction;

class ToolBarManager : public QObject
{
    Q_OBJECT
 public:
    ToolBarManager ();
    ToolBarManager ( QToolBar* );

    ~ToolBarManager ();

    virtual void setToolBar ( QToolBar* );

 private:
    void clear ();

 public slots:
    virtual void activateWidget ( QWidget*, bool activationFlag );
    virtual void activateWidget ( QWidget* );
    virtual void disactivateWidget ( QWidget* );

 protected slots:
    virtual void addWidget ( QWidget* );
    virtual void removeWidget ( QWidget* );

 signals: 
    void onToolBarChange ( QToolBar* );
    void onActivate ( QWidget* );
    void onDisactivate ( QWidget* );
    void onAdd ( QWidget* );
    void onRemove ( QWidget* );

 private:
    struct TabbedWidget
    {
        QAction* action;
        QWidget* widget;
        uint windowState;        
        
        TabbedWidget ( QWidget* w ) : widget(w) {}
        inline bool operator == ( const TabbedWidget* tw ) const
        { return widget == tw->widget; }
    };
    typedef std::vector<TabbedWidget*> TabbedWidgets;
    typedef TabbedWidgets::iterator TabbedWidgetsIter;

    TabbedWidgets widgets;
    QToolBar* toolBar;
    TabbedWidget* currentActiveWidget;
};

#endif //TOOLBARMANAGER_H
