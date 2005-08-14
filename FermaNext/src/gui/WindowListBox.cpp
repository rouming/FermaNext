
#include "WindowListBox.h"
#include "SubsidiaryConstants.h"
#include "TrussUnitActions.h"

#include <qstring.h>
#include <qcursor.h>
#include <qpopupmenu.h>

/*****************************************************************************
 * Truss Unit Window Item
 *****************************************************************************/

TrussUnitWindowItem::TrussUnitWindowItem ( FermaNextProject& prj,
                                           TrussUnitWindow& truss,
                                           WindowListBox* lb,
                                           const QPixmap& pix,
                                           const QPixmap& disPix ) :
    QListBoxPixmap( lb, pix, truss.getTrussName() ),
    listBox(lb),
    project(prj), 
    trussWindow(truss),
    disabledPix(disPix),
    selected(false)
{
    // Catch renaming
    QObject::connect( &truss, SIGNAL(onTrussNameChange(const QString&)),
                      SLOT(setText(const QString&)) );
}

const QPixmap* TrussUnitWindowItem::pixmap () const
{
    if ( isShown() )
        return QListBoxPixmap::pixmap();
    else
        return &disabledPix;        
}

void TrussUnitWindowItem::fillPopup ( QPopupMenu* popup ) const
{
    if ( isShown() ) {
        if ( isSelectedInGroup() ) {
            popup->insertItem(
                    QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                    "Unselect", this, SLOT(unselectFromGroup()) );
        } else {
            popup->insertItem( 
                    QPixmap::fromMimeSource(imagesPath + "/select.png"),
                    "Select", this, SLOT(selectInGroup()) );
        }
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/select.png"),
                        "Select All", this, SLOT(selectAllInGroup()) );
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                        "Unselect All", this, SLOT(unselectAllFromGroup()) );
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/calculate.png"),
                        "Calculate", this, SLOT(calculate()) );
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/delete.png"),
                        "Remove", this, SLOT(remove()) );
        popup->insertSeparator();

        popup->insertItem( 
                      QPixmap::fromMimeSource( imagesPath + "/hide.png"),
                      "Hide", this, SLOT(hide()) );

    } else {
        popup->insertItem(
                      QPixmap::fromMimeSource(imagesPath + "/show.png"),
                      "Show", this, SLOT(show()) );
    }
    popup->exec( QCursor::pos() );
}

bool TrussUnitWindowItem::isSelectedInGroup () const
{
    return selected;
}

bool TrussUnitWindowItem::isShown () const
{
    return trussWindow.isVisible();
}

void TrussUnitWindowItem::setText ( const QString& text )
{
    QListBoxPixmap::setText( text );
}

void TrussUnitWindowItem::raise ()
{
    if ( ! trussWindow.isVisible() )
        return;
    TrussUnitDesignerWidget& widget = project.getDesignerWindow().
        getDesignerWidget();
    widget.focusOnWindow( trussWindow );
}

void TrussUnitWindowItem::show ()
{
    if ( trussWindow.isVisible() )
        return;
    trussWindow.setVisible( true );
    raise();
}

void TrussUnitWindowItem::hide ()
{
    if ( ! trussWindow.isVisible() )
        return;
    trussWindow.setVisible( false );

}

void TrussUnitWindowItem::selectInGroup ()
{
    selected = true;
}

void TrussUnitWindowItem::unselectFromGroup ()
{
    selected = false;
}

void TrussUnitWindowItem::selectAllInGroup ()
{
    if ( listBox == 0 )
        return;
    listBox->selectAllInGroup();
}

void TrussUnitWindowItem::unselectAllFromGroup ()
{
    if ( listBox == 0 )
        return;
    listBox->unselectAllFromGroup();
}

void TrussUnitWindowItem::calculate ()
{
    //TODO: add calculation call here!
}

void TrussUnitWindowItem::remove ()
{
    // Save truss window remove state
    TrussUnitWindowManager& trussMng = project.getTrussUnitWindowManager();
    ObjectState& state = trussWindow.createState();
    TrussUnitWindowRemoveAction* action = 
                      new TrussUnitWindowRemoveAction( trussMng, trussWindow );
    state.addAction( action );
    state.save();
    // Remove truss window
    project.getTrussUnitWindowManager().removeTrussUnitWindow( trussWindow );
}

/*****************************************************************************
 * Windows List Box
 *****************************************************************************/

WindowListBox::WindowListBox ( FermaNextProject& prj, QWidget* parent,
                               const char* name, WFlags fl ) : 
    QListBox( parent, name, fl ),
    project(prj)
{
    // Catch double click on item
    QObject::connect( this, SIGNAL(doubleClicked(QListBoxItem*)),
                      SLOT(raiseTrussUnitWindowItem(QListBoxItem*)) );
    // Catch truss creation/removing
    TrussUnitWindowManager& trussMng = project.getTrussUnitWindowManager();
    QObject::connect( &trussMng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
                      SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    QObject::connect( &trussMng, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
                      SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
}

void WindowListBox::contextMenuEvent ( QContextMenuEvent* event )
{
    QListBoxItem* selected = selectedItem();
    if ( selected == 0 )
        return;
    TrussUnitWindowItem* item = 0;
    try { item = dynamic_cast<TrussUnitWindowItem*>(selected); }
    catch ( ... ) { return; }

    QPopupMenu* popup = new QPopupMenu( this );
    Q_CHECK_PTR( popup );
    item->fillPopup( popup );
    delete popup;
}

TrussUnitWindowItem* WindowListBox::findByTrussUnitWindow ( 
    const TrussUnitWindow& truss ) const
{
    if ( ! windowItems.contains( &truss ) )
        return 0;
    return windowItems[&truss];
}

void WindowListBox::addTrussUnitWindow ( TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = new TrussUnitWindowItem( 
                  project, truss, this, 
                  QPixmap::fromMimeSource(imagesPath + "/project.png"),
                  QPixmap::fromMimeSource(imagesPath + "/project_d.png"));
    windowItems[&truss] = item;
}

void WindowListBox::removeTrussUnitWindow ( TrussUnitWindow& truss )
{
    if ( ! windowItems.contains( &truss ) )
        return;
    const TrussUnitWindowItem* item = windowItems[&truss];
    windowItems.remove(&truss);
    delete item;
}

void WindowListBox::selectAllInGroup ()
{
    WindowMapIter iter = windowItems.begin();
    for ( ; iter != windowItems.end(); ++iter )
        iter.data()->selectInGroup();
}

void WindowListBox::unselectAllFromGroup ()
{
    WindowMapIter iter = windowItems.begin();
    for ( ; iter != windowItems.end(); ++iter )
        iter.data()->unselectFromGroup();
}

void WindowListBox::raiseTrussUnitWindowItem ( QListBoxItem* it )
{
    TrussUnitWindowItem* item = 0;
    try { item = dynamic_cast<TrussUnitWindowItem*>(it); }
    catch ( ... ) { return; }

    item->raise();
}

/*****************************************************************************/
