
#include "UndoRedoListBox.h"

/*****************************************************************************
 * Undo Redo List Box
 *****************************************************************************/

UndoRedoListBox::UndoRedoListBox ( QWidget* p ) :
    QListWidget( p ),
    stateManager(0)
{
    QObject::connect( this, SIGNAL(itemSelectionChanged()),
                            SLOT(onItemSelectionChanged()) );
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
        QListWidget::clear();
    }

    stateManager = newMng;
    if ( stateManager == 0 )
        return;

    // Init item
    addItem( "New" );
    // State manager states
    insertItems( 1, stateManager->stateBlockNames() );
    setCurrentRow( stateManager->currentPos() );
}

void UndoRedoListBox::onItemSelectionChanged ()
{    
    QList<QListWidgetItem*> selected = selectedItems();
    if ( selected.size() != 1 )
        return;
    int itemIndx = row( selected.at(0) );
    if ( itemIndx == -1 || stateManager == 0 || 
         stateManager->countStateBlocks() == 0 )
        return;
    stateManager->step( itemIndx );
}

/****************************************************************************/
