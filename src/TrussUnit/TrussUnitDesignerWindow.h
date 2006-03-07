 
#ifndef TRUSSUNITDESIGNERWINDOW_H
#define TRUSSUNITDESIGNERWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QScrollArea>
#include <QScrollArea>

#include "TrussUnitDesignerWidget.h"

class TrussUnitDesignerWindow : public QWidget
{
    Q_OBJECT
public:
    TrussUnitDesignerWindow ( const QString& caption, QWidget* p = 0,
                              //FIXME QT3TO4: WType_TopLevel
                              Qt::WFlags f = Qt::Window );

    virtual TrussUnitDesignerWidget& getDesignerWidget ();
    
    void resizeEvent ( QResizeEvent* );

private:
    QVBoxLayout* vbox;
    QScrollArea* scroll;
    TrussUnitDesignerWidget* designerWidget;
};

#endif //TRUSSUNITDESIGNERWINDOW_H
