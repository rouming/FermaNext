
#ifndef RESULTSTABWIDGET_H_
#define RESULTSTABWIDGET_H_

#include <QFrame>
#include <QTabWidget>
#include <QTextBrowser>

#include "ColorRangeControl.h"
#include "ResultsControlWidget.h"
#include "TrussDeformPicture.h"
#include "TrussSolutionResults.h"

class QWheelEvent;
class QScrollArea;
class HtmlPropertyTable;
class HtmlStressTable;
class HtmlDisplacementTable;

/*****************************************************************************/

class ResultsTab : public QWidget
{
public:
    ResultsTab ( QWidget* parent = 0, Qt::WFlags f = 0 );
    void fillTab ( const PluginResults&, TrussUnitCopy& );
    
protected:
    void init ();

private:
    QWidget* parentWidget;
    TrussDeformPicture* deformWidget;
    HtmlPropertyTable* propertyTable;
    HtmlStressTable* stressTable;
    HtmlDisplacementTable* dispTable;
    ColorRangeControl colorCtrl;
    ResultsControlWidget ctrlWidget;

    QScrollArea* parentArea;
};

/*****************************************************************************/

class ResultsTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    ResultsTabWidget ( QWidget* parent = 0 );
    ~ResultsTabWidget ();
    
    QSize sizeHint () const;

protected:
    ResultsTab* getPluginTab ( int );
    void addPluginTab ( const PluginResults&, TrussUnitCopy& );
    void removePluginTab ( int );

public slots:
    void setTrussSolutionResults ( const TrussSolutionResults& );

private:
    QScrollArea* scrollArea;
};

#endif // RESULTSTABWIDGET_H_
