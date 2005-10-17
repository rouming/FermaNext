
#include "FermaNextProject.h"
#include "SubsidiaryConstants.h"

#include <qtabwidget.h>
#include <qwidgetstack.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/

FermaNextProject::FermaNextProject ( const QString& name_, QWidgetStack* stack ) :
    name(name_),
    widgetStack(stack),
    projectMainWidget( new QMainWindow(widgetStack, 0, 0) ),
    calcDataToolBar( new CalcDataToolBar(projectMainWidget) ),
    projectTab( new QTabWidget(projectMainWidget) ),
    justStrengthAnalisysWidget( new QWidget(projectTab) ),
    designerWindow( new TrussUnitDesignerWindow(name_, projectTab) )    
{
    projectMainWidget->setRightJustification( true );
    projectMainWidget->setDockEnabled( DockLeft, false );
    projectMainWidget->setDockEnabled( DockRight, false );

    widgetStack->addWidget(projectMainWidget);
    projectMainWidget->setCentralWidget( projectTab );
  
    projectTab->setTabPosition( QTabWidget::Bottom );
    projectTab->addTab( designerWindow, tr("Designer") );
    projectTab->addTab( justStrengthAnalisysWidget, tr("Strength Analysis") );
      

    TrussUnitDesignerWidget& designerWidget = designerWindow->getDesignerWidget();

    // Catch trusses creation or deletion.
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             &designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             &designerWidget, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),     
             calcDataToolBar, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             calcDataToolBar, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
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

TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return trussWindowManager;
}

TrussUnitDesignerWindow& FermaNextProject::getDesignerWindow ()
{
    return *designerWindow;
}

CalcDataToolBar& FermaNextProject::getCalcDataToolBar ()
{
    return *calcDataToolBar;
}

/*****************************************************************************/
