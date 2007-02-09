
#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QDialog>
#include <QTabWidget>

class QCheckBox;

/*****************************************************************************/

class PreferencesTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    PreferencesTabWidget ( QWidget* parent = 0 );
    void loadPreferences ();
    void savePreferences ();

protected:
    void initTrussViewTab ();

protected slots:
    void showNodeNumbers ( bool status );
    void showPivotNumbers ( bool status );
    void showFixations ( bool status );
    void showLoads ( bool status );

private:
    QCheckBox *showNodeNumbsCb, *showPivotNumbsCb, 
              *showFixationsCb, *showLoadsCb;
};

/*****************************************************************************/

class PreferencesWidget : public QDialog
{
    Q_OBJECT
public:
    PreferencesWidget ( QWidget* parent = 0, Qt::WFlags f = 0 );

public slots:
    void accept ();
    void reject ();

protected:
    void init ();

private:
    PreferencesTabWidget* tabWidget;
};

#endif //PREFERENCESWIDGET_H
