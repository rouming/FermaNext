
#include "FermaNextProject.h"

#include <qworkspace.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/

FermaNextProject::FermaNextProject ( const QString& name_, QWorkspace* qWsp ) :
    designerWindow( new TrussUnitDesignerWindow(name_, qWsp, name_, 0) ),
    name(name_),
    windowIsAlreadyDestroyed(false)
{
    connect( designerWindow, SIGNAL(destroyed()), SLOT(markWindowDestroyed()) );
}

FermaNextProject::~FermaNextProject ()
{
    // Double deletion detected
    if ( !windowIsAlreadyDestroyed )
        delete designerWindow;
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

TrussUnitDesignerWindow& FermaNextProject::getWindow ()
{
    return *designerWindow;
}

ObjectStateManager& FermaNextProject::getStateManager ()
{
    return stateManager;
}

/*****************************************************************************/
