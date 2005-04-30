
#include "TrussUnitDesignerWindow.h"

TrussUnitDesignerWindow::TrussUnitDesignerWindow ( const QString& name ) :
    scroll(this),
    vbox(this)
{
    designerWidget.resize(800,800);
    scroll.addChild(&designerWidget);
    designerWidget.setAcceptDrops(TRUE);

	vbox.addWidget(&scroll);
	vbox.activate();
    
    setCaption( name );
}

