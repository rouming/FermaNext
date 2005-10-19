
#include "UndoRedoListBox.h"

/*****************************************************************************
 * Undo Redo List Box
 *****************************************************************************/

UndoRedoListBox::UndoRedoListBox ( QWidget* p, const char* n, WFlags f ) :
    QListBox( p, n, f ),
    stateManager(0)
{}

UndoRedoListBox::~UndoRedoListBox ()
{}

ObjectStateManager* UndoRedoListBox::getStateManager () const
{
    return stateManager;
}

void UndoRedoListBox::setStateManager ( ObjectStateManager* newMng )
{
    if ( stateManager ) {
        stateManager->disconnect( this );
        QListBox::clear();
    }

    stateManager = newMng;
    if ( stateManager == 0 )
        return;

    insertStringList( stateManager->stateBlockNames() );
}

/****************************************************************************/
