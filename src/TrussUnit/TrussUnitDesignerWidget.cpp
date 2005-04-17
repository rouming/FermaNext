
#include "TrussUnitDesignerWidget.h"
#include <algorithm>

/*****************************************************************************
 * Truss Designer Widget
 *****************************************************************************/

TrussUnit& TrussUnitDesignerWidget::createTrussUnit ()
{
    TrussUnit* truss = new TrussUnit;
    trussUnits.push_back(truss);
    return *truss;
}

bool TrussUnitDesignerWidget::removeTrussUnit ( const TrussUnit& truss )
{
    TrussUnitList::iterator iter = trussUnits.begin();
    for ( ; iter != trussUnits.end(); ++iter )
        if ( (*iter) == &truss ) {
            delete *iter;
            trussUnits.erase(iter);
            return true;
        }            
    return false; 
}

/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* pe )
{
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* re )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* ctrl )
{
}

/*****************************************************************************/
