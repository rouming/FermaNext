 
#ifndef TRUSSUNITDESIGNERWINDOW_H
#define TRUSSUNITDESIGNERWINDOW_H

#include <qmainwindow.h>
#include <qlayout.h>
#include <qscrollview.h>
#include "TrussUnitDesignerWidget.h"

class TrussUnitDesignerWindow : public QMainWindow
{
    Q_OBJECT
public:
    TrussUnitDesignerWindow ( const QString& caption, QWidget* p = 0, 
                              const char* n = 0, WFlags f =WType_TopLevel );

public slots:
    // Delegate some slots of designerWidget
    virtual void addTrussUnit ( TrussUnit& );
    virtual bool removeTrussUnit ( const TrussUnit& );
    virtual bool removeTrussUnitPseudoWindow ( const TrussUnitPseudoWindow& );

private:
    QVBoxLayout vbox;
    QScrollView scroll;
    TrussUnitDesignerWidget* designerWidget;
};

#endif //TRUSSUNITDESIGNERWINDOW_H
