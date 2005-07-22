
#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <qtoolbar.h>
#include <qiconset.h>
#include <vector>

class QWidget;
class QToolButton;
class ToolBarManager;

class TabbedWidget : public QObject
{
    Q_OBJECT
 public:
    TabbedWidget ( ToolBarManager&,  QWidget&, 
                   const QString& name, const QIconSet& iconSet );

 public slots:
    void activate ();
    void disactivate ();
    void focus ();
    void defocus ();

    bool isActive () const;
    bool isFocused () const;

 signals:
    void onActivate ( TabbedWidget& );
    void onDisactivate ( TabbedWidget& );
    void onFocus ( TabbedWidget& );
    void onDefocus ( TabbedWidget& );

 private:
    QString name;
    QWidget& widget;
    QToolButton* button;
    uint windowState;
};

class ToolBarManager : public QToolBar
{
    Q_OBJECT
 protected:

 public:
    ToolBarManager ( QMainWindow* parent = 0, const char* name = 0 );

    ~ToolBarManager ();

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
    TabbedWidget* currentActiveWidget;
};

#endif //TOOLBARMANAGER_H
