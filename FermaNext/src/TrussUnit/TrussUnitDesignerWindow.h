 
#ifndef TRUSSUNITDESIGNERWINDOW_H
#define TRUSSUNITDESIGNERWINDOW_H

#include <qmainwindow.h>
#include <qlayout.h>
#include <qscrollview.h>
#include "TrussUnitDesignerWidget.h"

class TrussUnitDesignerWindow : public QWidget
{
    Q_OBJECT
public:
    TrussUnitDesignerWindow ( const QString& caption, QWidget* p = 0, 
                              const char* n = 0, WFlags f =WType_TopLevel );

    ~TrussUnitDesignerWindow ();
    
    virtual TrussUnitDesignerWidget& getDesignerWidget ();
    
    void resizeEvent ( QResizeEvent* );

private:
    QVBoxLayout* vbox;
    QScrollView* scroll;
    TrussUnitDesignerWidget* designerWidget;
};

#endif //TRUSSUNITDESIGNERWINDOW_H
