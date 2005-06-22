
#include "TrussUnitDesignerWindow.h"

/*****************************************************************************
 * Truss Unit Designer Window
 *****************************************************************************/

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& caption, 
                                                   QWidget* p, const char* n, 
                                                   WFlags f ) :
    QMainWindow(p, n, f),
    vbox(this),
    scroll(this),
    designerWidget(new TrussUnitDesignerWidget(this))
{   
    scroll.addChild(designerWidget);
    designerWidget->setAcceptDrops(TRUE);

    vbox.addWidget(&scroll);
    vbox.activate();
 
    setMinimumSize( 640, 480 );
    setCaption( caption );
}

TrussUnitDesignerWidget& TrussUnitDesignerWindow::getDesignerWidget ()
{
    return *designerWidget;
}

void TrussUnitDesignerWindow::resizeEvent ( QResizeEvent* event )
{
    designerWidget->resize(event->size());   
}

/*****************************************************************************/
