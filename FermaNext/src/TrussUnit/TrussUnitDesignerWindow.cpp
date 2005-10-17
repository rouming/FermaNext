
#include "TrussUnitDesignerWindow.h"

/*****************************************************************************
 * Truss Unit Designer Window
 *****************************************************************************/

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& caption, 
                                                   QWidget* p, const char* n, 
                                                   WFlags f ) :
    QWidget(p, n, f),
    vbox( new QVBoxLayout(this) ),
    scroll( new QScrollView(this) )    
{   
    vbox->addWidget(scroll);    
    scroll->setVScrollBarMode( QScrollView::AlwaysOff );
    scroll->setHScrollBarMode( QScrollView::AlwaysOff );

    designerWidget = new TrussUnitDesignerWidget(scroll->viewport());
    scroll->addChild(designerWidget);
    designerWidget->setAcceptDrops(TRUE);    

    setMinimumSize( 640, 480 );
    setCaption( caption );
}

TrussUnitDesignerWindow::~TrussUnitDesignerWindow ()
{
    int i =0;
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
