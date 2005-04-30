
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
    TrussUnitDesignerWindow ( const QString& name );


private:
    QVBoxLayout vbox;
    QScrollView scroll;
    TrussUnitDesignerWidget designerWidget;
};

#endif //TRUSSUNITDESIGNERWINDOW_H
