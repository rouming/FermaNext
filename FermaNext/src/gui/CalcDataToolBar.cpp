
#include "CalcDataToolBar.h"
#include "CalcDataWidget.h"

#include <qlabel.h>
#include <qiconset.h>

/*****************************************************************************
 * Calculation Data Tool Bar
 *****************************************************************************/

CalcDataToolBar::CalcDataToolBar ( QMainWindow* parent, const char* name ) :
    ToolBarManager(parent, name)
{
    setLabel( tr("Calculation data") );
    initCalcDataIcons();
}

CalcDataToolBar::~CalcDataToolBar ()
{
    clear();
}

CalcDataWidget* CalcDataToolBar::findCalcDataWidget ( TrussUnitWindow& truss )
{
    if ( ! calcDataWidgets.contains( &truss ) )
        return 0;
    return calcDataWidgets[&truss];
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
    calcDataIcons.setPixmap( QPixmap::fromMimeSource( imagesPath() + "/calcdata.png"),
                             QIconSet::Automatic, QIconSet::Active );
}

void CalcDataToolBar::addWidget ( QWidget& w )
{
    createTabbedWidget( w, w.caption(), calcDataIcons );    
}

void CalcDataToolBar::addTrussUnitWindow ( TrussUnitWindow& truss )
{
    // Catch renaming
    QObject::connect( &truss, SIGNAL(onTrussNameChange(const QString&)),
                              SLOT(trussUnitWindowRename(const QString&)) );
    // Catch life time changing
    QObject::connect( &truss, SIGNAL(onAfterDesist(StatefulObject&)), 
                              SLOT(trussUnitWindowDesist(StatefulObject&)) );
    QObject::connect( &truss, SIGNAL(onAfterRevive(StatefulObject&)), 
                              SLOT(trussUnitWindowRevive(StatefulObject&)) );

    CalcDataWidget* w = new CalcDataWidget( parentWidget(), 0, WType_TopLevel);
    w->setCaption( truss.getTrussName() );
    calcDataWidgets[&truss] = w;
    addWidget( *w );
}

void CalcDataToolBar::removeTrussUnitWindow ( TrussUnitWindow& truss )
{
    if ( ! calcDataWidgets.contains( &truss ) )
        return;
    QWidget* w = calcDataWidgets[&truss];
    removeWidget( *w );
    calcDataWidgets.remove( &truss );
    delete w;
}

void CalcDataToolBar::trussUnitWindowRename ( const QString& newName )
{
    try { 
        const TrussUnitWindow& trussWindow = 
            dynamic_cast<const TrussUnitWindow&>( *sender() );
        if ( calcDataWidgets.contains( &trussWindow ) ) {
            QWidget* w = calcDataWidgets[&trussWindow];
            w->setCaption( newName );
            TabbedWidget* tw = findByWidget( *w );
            if ( tw )
                tw->setName( newName );
        }
    } catch ( ... ) {}
}

void CalcDataToolBar::trussUnitWindowDesist ( StatefulObject& st )
{
    trussUnitWindowReviveDesist( st, false );
}

void CalcDataToolBar::trussUnitWindowRevive ( StatefulObject& st )
{
    trussUnitWindowReviveDesist( st, true );    
}

void CalcDataToolBar::trussUnitWindowReviveDesist ( StatefulObject& st,
                                                    bool action )
{
    try { 
        TrussUnitWindow& trussWindow = dynamic_cast<TrussUnitWindow&>(st);
        if ( ! calcDataWidgets.contains( &trussWindow ) )
            return;       
        QWidget* w = calcDataWidgets[&trussWindow];
        TabbedWidget* tw = findByWidget(*w);
        if ( tw == 0 )
            return;
        tw->setVisible( action );
    } catch ( ... ) { return; }
}

/*****************************************************************************/
