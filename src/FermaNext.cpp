
#include <qapplication.h>
#include "TrussUnit.h"
#include "TrussUnitDesignerWindow.h"

int main ( int argc, char* argv[] )
{
    QApplication app(argc, argv);
    TrussUnitDesignerWindow w("FFF");        
    w.resize( 800, 600 );    
    w.show();
    app.setMainWidget(&w);    
    return app.exec();
}
