
#include "CalcDataToolBar.h"

/*****************************************************************************
 * Calculation Data Tool Bar
 *****************************************************************************/

#include <qpushbutton.h>
#include <qlabel.h>
CalcDataToolBar::CalcDataToolBar ( QMainWindow* parent, const char* name ) :
    ToolBarManager(parent, name)
{
    setNewLine ( true );  

    QIconSet iconEnabled ( QPixmap::fromMimeSource( "images/calcdata.png") );
    //QIconSet iconDisabled ( QPixmap::fromMimeSource( "images/calcdata_d.png") );
    QButton* button = new QPushButton( iconEnabled, tr("Truss unit 1"), this );    
    button = new QPushButton( iconEnabled, tr("Results are not valid"), this );    
    button->setEnabled(false);
    button = new QPushButton( iconEnabled, tr("Next variant"), this );    

    // Blank widget
    setStretchableWidget( new QLabel(this) );
}

/*****************************************************************************/
