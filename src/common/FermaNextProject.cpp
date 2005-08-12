
#include "FermaNextProject.h"
#include "SubsidiaryConstants.h"

#include <qtabwidget.h>
#include <qwidgetstack.h>
/*TODO:remove*/#include <qpushbutton.h>
/*TODO:remove*/#include <qlabel.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/
FermaNextProject::FermaNextProject ( const QString& name_, QWidgetStack* stack ) :
    name(name_),
    stateManager(),
    trussWindowManager(&stateManager),
    widgetStack(stack),
    projectMainWidget( new QMainWindow(widgetStack, 0, 0) ),
    calcDataToolBar( new CalcDataToolBar(projectMainWidget) ),
    projectTab( new QTabWidget(projectMainWidget) )
{
    projectMainWidget->setRightJustification( true );
    projectMainWidget->setDockEnabled( DockLeft, false );
    projectMainWidget->setDockEnabled( DockRight, false );

    widgetStack->addWidget(projectMainWidget);
    projectMainWidget->setCentralWidget( projectTab );

    calcDataToolBar->setLabel( tr("Calculation data") );

    justStrengthAnalisysWidget = new QWidget(projectTab);
    designerWindow = new TrussUnitDesignerWindow(name_, projectTab);

    projectTab->setTabPosition( QTabWidget::Bottom );
    projectTab->addTab( designerWindow, tr("Designer") );
    projectTab->addTab( justStrengthAnalisysWidget, tr("Strength Analysis") );
      

    TrussUnitDesignerWidget& designerWidget = designerWindow->getDesignerWidget();

    // Catch trusses creation or deletion.
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             &designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowRemove(const TrussUnitWindow&)), 
             &designerWidget, SLOT(removeTrussUnitWindow(const TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),     
             calcDataToolBar, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowRemove(const TrussUnitWindow&)), 
             calcDataToolBar, SLOT(removeTrussUnitWindow(const TrussUnitWindow&)) );

    //TODO: remove this block in future
	for( uint i = 0; i <1; i++)
	{
        trussWindowManager.createTrussUnitWindow("Truss unit");
	}
}

FermaNextProject::~FermaNextProject ()
{
    if ( widgetStack )
        widgetStack->removeWidget( projectMainWidget );
    delete projectMainWidget;
}

void FermaNextProject::activate ()
{
    if ( widgetStack && widgetStack->visibleWidget() != projectMainWidget )
        widgetStack->raiseWidget( projectMainWidget );
}

const QString& FermaNextProject::getName () const
{
    return name;
}

void FermaNextProject::setName ( const QString& name_ )
{
    name = name_;
    emit onNameChange(name);
}

ObjectStateManager& FermaNextProject::getStateManager ()
{
    return stateManager;
}

TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return trussWindowManager;
}

TrussUnitDesignerWindow& FermaNextProject::getDesignerWindow ()
{
    return *designerWindow;
}

/*****************************************************************************/
