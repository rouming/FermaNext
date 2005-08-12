
#include "CalcDataToolBar.h"

#include <qlabel.h>
#include <qiconset.h>

/*****************************************************************************
 * Calculation Data Tool Bar
 *****************************************************************************/

CalcDataToolBar::CalcDataToolBar ( QMainWindow* parent, const char* name ) :
    ToolBarManager(parent, name)
{
    initCalcDataIcons();
}

CalcDataToolBar::~CalcDataToolBar ()
{
    clear();
}

void CalcDataToolBar::clear ()
{
    CalcDataMapIter iter = calcDataWidgets.begin();
    for ( ; iter != calcDataWidgets.end(); ++iter )
        delete iter.data();
    calcDataWidgets.clear();
}

void CalcDataToolBar::initCalcDataIcons ()
{
    // Active icons
    calcDataIcons.setPixmap( QPixmap::fromMimeSource( "images/calcdata.png"),
                             QIconSet::Automatic, QIconSet::Active );
}

void CalcDataToolBar::addWidget ( QWidget& w )
{
    createTabbedWidget( w, w.caption(), calcDataIcons );    
}

void CalcDataToolBar::addTrussUnitWindow ( TrussUnitWindow& truss )
{
    QWidget* w = new QWidget();
    w->setCaption( truss.getTrussName() );
    calcDataWidgets[&truss] = w;
    addWidget( *w );
}

void CalcDataToolBar::removeTrussUnitWindow ( const TrussUnitWindow& truss )
{
    if ( ! calcDataWidgets.contains( &truss ) )
        return;
    QWidget* w = calcDataWidgets[&truss];
    removeWidget( *w );
    calcDataWidgets.remove( &truss );
    delete w;
}

/*****************************************************************************/
