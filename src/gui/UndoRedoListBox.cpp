
#include "UndoRedoListBox.h"

/*****************************************************************************
 * Undo Redo List Box
 *****************************************************************************/

UndoRedoListBox::UndoRedoListBox ( QWidget* p, const char* n, Qt::WFlags f ) :
    Q3ListBox( p, n, f ),
    stateManager(0)
{
    QObject::connect( this, SIGNAL(clicked(Q3ListBoxItem*)),
                            SLOT(clickOnItem(Q3ListBoxItem*)) );
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
        Q3ListBox::clear();
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

void UndoRedoListBox::clickOnItem ( Q3ListBoxItem* item )
{
    int itemIndx = index( item );
    if ( itemIndx == -1 || stateManager == 0 || 
         stateManager->countStateBlocks() == 0 )
        return;
    stateManager->step( itemIndx );
}

/****************************************************************************/
