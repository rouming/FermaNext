
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
 protected:
    struct TabbedWidget
    {
        QAction* action;
        QWidget* widget;
        uint windowState;        
        
        TabbedWidget ( QWidget* w ) : widget(w) {}
    };

 public:
    ToolBarManager ( QWidget* parent = 0 );    

    ~ToolBarManager ();

    virtual void setToolBar ( QToolBar* );

 protected:
    virtual void clear ();
    virtual TabbedWidget* findByWidget ( QWidget* ) const;

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
    typedef std::vector<TabbedWidget*> TabbedWidgets;
    typedef TabbedWidgets::iterator TabbedWidgetsIter;
    typedef TabbedWidgets::const_iterator TabbedWidgetsConstIter;

    TabbedWidgets widgets;
    QToolBar* toolBar;
    TabbedWidget* currentActiveWidget;
};

#endif //TOOLBARMANAGER_H
