
#include "TrussUnitDesignerWindow.h"

/*****************************************************************************
 * Truss Unit Designer Window
 *****************************************************************************/

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& caption, 
                                                   QWidget* p, Qt::WFlags f ) :
    QWidget(p, f),
    vbox( new QVBoxLayout(this) ),
    scroll( new QScrollArea(this) )    
{   
    vbox->addWidget(scroll);    
    scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    designerWidget = new TrussUnitDesignerWidget(scroll->viewport());
    // FIXME QT3TO4: scroll->addChild(designerWidget);
    scroll->setWidget(designerWidget);
    designerWidget->setAcceptDrops(true);

    setMinimumSize( 640, 480 );
    setWindowTitle( caption );
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
