
#include "FermaNextWorkspace.h"
#include <iostream>

int main ()
{
    FermaNextConfig& conf = FermaNextWorkspace::workspace().config();
    QDomNode n = conf.findConfigNode("TrussUnitWindow");
    if ( n.isNull() ) {
        std::cout << "can't find\n";
        return 1;
    } else {        
        ConfigItems items = conf.getConfigItems(n);
        ConfigItems::iterator i = items.begin();
        for ( ; i != items.end() ; ++i ) { 
            QColor c = i.data().toColor();
            std::cout << i.key().ascii() << " : " << "\n";
            std::cout << "\t" << c.red() << "\n";
            std::cout << "\t" << c.green() << "\n";
            std::cout << "\t" << c.blue() << "\n";
        }
    }
    return 0;
}

/* Output:

BorderColor : 
	20
	35
	40
CanvasColor : 
	8
	10
	12
HeadlineColor : 
	8
	10
	12

*/

