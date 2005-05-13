
#include "TrussUnitDesignerWindow.h"

/*****************************************************************************
 * Designer Window
 *****************************************************************************/

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& caption, 
                                                   QWidget* p, const char* n, 
                                                   WFlags f ) :
    QMainWindow(p, n, f),
    vbox(this),
    scroll(this),
    designerWidget(new TrussUnitDesignerWidget(this))
{       
    designerWidget->resize(900,800);
    scroll.addChild(designerWidget);
    designerWidget->setAcceptDrops(TRUE);

    vbox.addWidget(&scroll);
    vbox.activate();
 
    setMinimumSize( 640, 480 );
    setCaption( caption );
}

void TrussUnitDesignerWindow::addTrussUnit ( TrussUnit& truss )
{    
    designerWidget->addTrussUnit ( truss ); 
}

bool TrussUnitDesignerWindow::removeTrussUnit ( const TrussUnit& truss )
{
    return designerWidget->removeTrussUnit( truss );
}

bool TrussUnitDesignerWindow::removeTrussUnitPseudoWindow 
                                ( const TrussUnitPseudoWindow& window )
{
    return designerWidget->removeTrussUnitPseudoWindow ( window );
}


/*****************************************************************************/