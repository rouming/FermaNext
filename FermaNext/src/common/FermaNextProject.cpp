
#include "FermaNextProject.h"
#include "SubsidiaryConstants.h"

#include <qworkspace.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/

FermaNextProject::FermaNextProject ( const QString& name_, QWorkspace* qWsp ) :
    maximizedDesginerWindow(true),
    name(name_),
    designerWindow( new TrussUnitDesignerWindow(name_, qWsp, name_, 0) ),
    windowIsAlreadyDestroyed(false)
{
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
    // Double deletion detected
    if ( !windowIsAlreadyDestroyed )
        delete designerWindow;
}

void FermaNextProject::activate ( bool activate )
{
    if ( activate && maximizedDesginerWindow ) 
        designerWindow->showMaximized();
    else if ( activate && !maximizedDesginerWindow ) 
        designerWindow->showNormal();
    else
        designerWindow->hide();
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

ObjectStateManager& FermaNextProject::getStateManager ()
{
    return stateManager;
}

TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return trussWindowManager;
}

bool FermaNextProject::eventFilter( QObject*, QEvent* e )
{
    switch ( e->type() )
    {    
    case QEvent::WindowStateChange:
        maximizedDesginerWindow = designerWindow->isMaximized();
        break;
    default:
        break;
    }
    return false;
}

/*****************************************************************************/
