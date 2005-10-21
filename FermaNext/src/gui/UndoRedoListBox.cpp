
#include "UndoRedoListBox.h"

/*****************************************************************************
 * Undo Redo List Box
 *****************************************************************************/

UndoRedoListBox::UndoRedoListBox ( QWidget* p, const char* n, WFlags f ) :
    QListBox( p, n, f ),
    stateManager(0)
{
    QObject::connect( this, SIGNAL(clicked(QListBoxItem*)),
                            SLOT(clickOnItem(QListBoxItem*)) );
}

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

    // Init item
    insertItem( "New" );
    // State manager states
    insertStringList( stateManager->stateBlockNames() );
    setSelected( stateManager->currentPos(), true );
    setBottomItem( stateManager->currentPos() );
}

void UndoRedoListBox::clickOnItem ( QListBoxItem* item )
{
    int itemIndx = index( item );
    if ( itemIndx == -1 || stateManager == 0 )
        return;
    stateManager->step( itemIndx );
}

/****************************************************************************/
