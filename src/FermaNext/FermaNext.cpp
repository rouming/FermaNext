
#include <qapplication.h>
#include "FermaNextMainFrame.h"

/*****************************************************************************
 * Main 
 *****************************************************************************/

int main ( int argc, char* argv[] )
{
    QApplication app(argc, argv);
    FermaNextMainFrame ferma; 
    ferma.showMaximized();
    app.setMainWidget(&ferma);   
    return app.exec();
}

/*****************************************************************************/

