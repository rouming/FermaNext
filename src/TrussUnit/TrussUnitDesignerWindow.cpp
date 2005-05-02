
#include "TrussUnitDesignerWindow.h"

/*****************************************************************************
 * Designer Window
 *****************************************************************************/

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& caption, QWidget* p, 
                                                   const char* n, WFlags f ) :
    QMainWindow(p, n, f),
    designerWidget(this),
    scroll(this),
    vbox(this)
{       
    designerWidget.resize(900,800);
    scroll.addChild(&designerWidget);
    designerWidget.setAcceptDrops(TRUE);

    vbox.addWidget(&scroll);
    vbox.activate();
 
    setMinimumSize( 640, 480 );
    setCaption( caption );
}

/*****************************************************************************/
