
#ifndef AGGTRUSSTOOLBAR_H
#define AGGTRUSSTOOLBAR_H

#include "AggToolBar.h"

class TrussDesignerWidget;

/*****************************************************************************/

class AggTrussToolBar : public AggToolBar
{
    Q_OBJECT
public:
    AggTrussToolBar ( TrussDesignerWidget* );
    ~AggTrussToolBar ();

protected:
    void initButtons ();

private:
    TrussDesignerWidget* designerWidget;
};

#endif //AGGTRUSSTOOLBAR_H
