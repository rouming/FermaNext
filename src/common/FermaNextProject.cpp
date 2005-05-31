
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

    // Should be connected to avoid double deletion.
    connect( designerWindow, SIGNAL(destroyed()), SLOT(markWindowDestroyed()) );

    // Catch trusses creation or deletion.
    connect( &trussManager, SIGNAL(onTrussUnitCreate(TrussUnit&)), 
             designerWindow, SLOT(addTrussUnit(TrussUnit&)) );
    connect( &trussManager, SIGNAL(onTrussUnitRemove(const TrussUnit&)), 
             designerWindow, SLOT(removeTrussUnit(const TrussUnit&)) );

    //TODO: remove this block in future
	for( uint i = 0; i <1; i++)
	{
        trussManager.createTrussUnit("Truss unit");
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

TrussUnitManager& FermaNextProject::getTrussUnitManager ()
{
    return trussManager;
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
