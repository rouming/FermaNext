
#include "FermaNextProject.h"
#include "FermaNextWorkspace.h"
#include "SubsidiaryConstants.h"

#include <qtabwidget.h>
#include <qwidgetstack.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/
FermaNextProject::FermaNextProject ( const QString& name_, QWidgetStack* stack ) :
    maximizedDesginerWindow(true),
    name(name_),
    projectTab( new QTabWidget(stack) ),
    windowIsAlreadyDestroyed(false)
{    
    justStrengthAnalisysWidget = new QWidget(stack);
    designerWindow = new TrussUnitDesignerWindow(name_, stack);

    projectTab->setTabPosition( QTabWidget::Bottom );
    projectTab->addTab( designerWindow, tr("Designer") );
    projectTab->addTab( justStrengthAnalisysWidget, tr("Strength Analysis") );
    stack->addWidget(projectTab);

    designerWindow->setIcon( QPixmap::fromMimeSource( imagesPath + "/project.png" ) );
    designerWindow->installEventFilter(this);

    TrussUnitDesignerWidget& designerWidget = designerWindow->getDesignerWidget();

    // Should be connected to avoid double deletion.
    connect( designerWindow, SIGNAL(destroyed()), SLOT(markWindowDestroyed()) );
    
    // Catch trusses creation or deletion.
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             &designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( &trussWindowManager, SIGNAL(onTrussUnitWindowRemove(const TrussUnitWindow&)), 
             &designerWidget, SLOT(removeTrussUnitWindow(const TrussUnitWindow&)) );

    //TODO: remove this block in future
	for( uint i = 0; i <1; i++)
	{
        trussWindowManager.createTrussUnitWindow("Truss unit");
	}
}

FermaNextProject::~FermaNextProject ()
{
    FermaNextWorkspace::workspace().getWidgetStack().removeWidget(projectTab);
    // Double deletion detected
    if ( !windowIsAlreadyDestroyed ) {
        delete designerWindow;
        delete projectTab;
        //TODO: access violation delete justStrengthAnalisysWidget;
    }
}

void FermaNextProject::activate ()
{
    FermaNextWorkspace::workspace().activateProject(*this);
}

void FermaNextProject::markWindowDestroyed ()
{
    windowIsAlreadyDestroyed = true;
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
/* TODO
ObjectStateManager& FermaNextProject::getStateManager ()
{
    return stateManager;
}
*/
TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return trussWindowManager;
}

QTabWidget& FermaNextProject::getProjectTab ()
{
    return *projectTab;
}

/*****************************************************************************/
