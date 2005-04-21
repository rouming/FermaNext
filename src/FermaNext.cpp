
#include <qapplication.h>
#include "TrussUnit.h"
#include "TrussUnitDesignerWidget.h"

int main ( int argc, char* argv[] )
{
    QApplication app(argc, argv);
    TrussUnitDesignerWidget w;        
    w.resize( QSize(800, 600) );    
    w.show();
    app.setMainWidget(&w);    
    return app.exec();
}
