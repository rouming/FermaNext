
#include "FermaNextProject.h"

#include <qworkspace.h>

const QString imagesPath( "images");

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
    connect( designerWindow, SIGNAL(destroyed()), SLOT(markWindowDestroyed()) );
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

bool FermaNextProject::eventFilter( QObject* o, QEvent* e )
{
    switch ( e->type() )
    {    
    case QEvent::WindowStateChange:
        maximizedDesginerWindow = designerWindow->isMaximized();
        break;    
    }
    return false;
}

/*****************************************************************************/
