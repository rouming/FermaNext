
#include "WindowListBox.h"
#include "SubsidiaryConstants.h"
#include "TrussUnitActions.h"
#include "FermaNextWorkspace.h"
#include "TrussCalcData.h"
#include "CalcDataWidget.h"

#include <qstring.h>
#include <qcursor.h>
#include <qpopupmenu.h>

/*****************************************************************************
 * Truss Unit Window Item
 *****************************************************************************/

TrussUnitWindowItem::TrussUnitWindowItem ( FermaNextProject& prj,
                                           TrussUnitWindow& truss,
                                           WindowListBox& lb,
                                           const QPixmap& pix,
                                           const QPixmap& disPix ) :
    QListBoxPixmap( &lb, pix, truss.getTrussName() ),
    listBox(lb),
    project(prj), 
    trussWindow(truss),
    disabledPix(disPix),
    selected(false)
{
    // Catch renaming
    QObject::connect( &truss, SIGNAL(onTrussNameChange(const QString&)),
                              SLOT(setText(const QString&)) );
    //Catch visible changing
    QObject::connect( &truss, SIGNAL(onVisibleChange(bool)),
                              SLOT(update()) );
    // Catch life time changing
    QObject::connect( &truss, SIGNAL(onAfterDesist(StatefulObject&)), 
                              SLOT(trussWindowDesisted()) );
    QObject::connect( &truss, SIGNAL(onAfterRevive(StatefulObject&)), 
                              SLOT(trussWindowRevived()) );
}

void TrussUnitWindowItem::trussWindowDesisted ()
{
    listBox.trussWindowDesisted( trussWindow );
}

void TrussUnitWindowItem::trussWindowRevived ()
{
    listBox.trussWindowRevived( trussWindow );
}

void TrussUnitWindowItem::update ()
{
    listBox.triggerUpdate(true);
}

void TrussUnitWindowItem::saveVisibilityChange ( bool visible )
{
    // Save visibility change to undo-redo
    ObjectState& state = trussWindow.createState();
    state.addAction( new TrussUnitWindowVisibilityChange( trussWindow, 
                                                          visible ) );
    state.save();
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
                    QPixmap::fromMimeSource(imagesPath() + "/unselect.png"),
                    "Unselect", this, SLOT(unselectFromGroup()) );
        } else {
            popup->insertItem( 
                    QPixmap::fromMimeSource(imagesPath() + "/select.png"),
                    "Select", this, SLOT(selectInGroup()) );
        }
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath() + "/select.png"),
                        "Select All", this, SLOT(selectAllInGroup()) );
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath() + "/unselect.png"),
                        "Unselect All", this, SLOT(unselectAllFromGroup()) );
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath() + "/calculate.png"),
                        "Calculate", this, SLOT(calculate()) );
        popup->insertSeparator();
        popup->insertItem(
                        QPixmap::fromMimeSource(imagesPath() + "/delete.png"),
                        "Remove", this, SLOT(remove()) );
        popup->insertSeparator();

        popup->insertItem( 
                      QPixmap::fromMimeSource( imagesPath() + "/hide.png"),
                      "Hide", this, SLOT(hide()) );

    } else {
        popup->insertItem(
                      QPixmap::fromMimeSource(imagesPath() + "/show.png"),
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

bool TrussUnitWindowItem::isAlive () const
{
    return trussWindow.isAlive();
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
    saveVisibilityChange( true );
    raise();
    update();
}

void TrussUnitWindowItem::hide ()
{
    if ( ! trussWindow.isVisible() )
        return;
    trussWindow.setVisible( false );
    saveVisibilityChange( false );
    update();
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
    listBox.selectAllInGroup();
}

void TrussUnitWindowItem::unselectAllFromGroup ()
{
    listBox.unselectAllFromGroup();
}

void TrussUnitWindowItem::calculate ()
{ 
    FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();    
    PluginManager& plgManager = wsp.pluginManager();
    PluginHandleList pluginHandles = 
                          plgManager.loadedPluginsOfType( CALCULATION_PLUGIN );
    if ( pluginHandles.size() == 0 )
        return;
    try {
        // Try to find truss calc data widget
        CalcDataToolBar& calcToolBar = project.getCalcDataToolBar();        
        CalcDataWidget* calcForm = calcToolBar.findCalcDataWidget( trussWindow );

        // Find first calculation plugin. 
        // TODO: plural calculation plugin support
        Plugin& calcPlugin = plgManager.findPlugin( pluginHandles[0] );

        // Do calculation with new topology and calc data
        TrussCalcData calcData;
        TrussTopology& topology = trussWindow.createTopology();
        calcPlugin.calculate( topology, calcData );
        calcForm->initCalc( calcData );
        calcForm->show();

        // TODO: toplogy manager
        topology.desist();
    }
    catch ( ... ) {}                
}

void TrussUnitWindowItem::remove ()
{
    // Save truss window remove state
    ObjectState& state = trussWindow.createState();
    TrussUnitWindowRemoveAction* action = 
                      new TrussUnitWindowRemoveAction( trussWindow );
    state.addAction( action );
    state.save();
    // Remove truss window
    trussWindow.desist();
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

void WindowListBox::contextMenuEvent ( QContextMenuEvent* )
{
    QListBoxItem* selected = selectedItem();
    if ( selected == 0 )
        return;
    try { 
        TrussUnitWindowItem& item = 
            dynamic_cast<TrussUnitWindowItem&>( *selected ); 
        QPopupMenu* popup = new QPopupMenu( this );
        Q_CHECK_PTR( popup );
        item.fillPopup( popup );
        delete popup;
    }
    catch ( ... ) { return; }
}

TrussUnitWindowItem* WindowListBox::findByTrussUnitWindow ( 
    const TrussUnitWindow& truss ) const
{
    if ( ! windowItems.contains( &truss ) )
        return 0;
    return windowItems[&truss].item;
}

void WindowListBox::addTrussUnitWindow ( TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = new TrussUnitWindowItem( 
                  project, truss, *this, 
                  QPixmap::fromMimeSource(imagesPath() + "/project.png"),
                  QPixmap::fromMimeSource(imagesPath() + "/project_d.png"));
    IndexedItem iitem = { item, index(item) };
    windowItems[&truss] = iitem;
}

void WindowListBox::removeTrussUnitWindow ( TrussUnitWindow& truss )
{
    const TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item ) {
        windowItems.remove(&truss);
        delete item;
    }
}

void WindowListBox::selectAllInGroup ()
{
    WindowMapIter iter = windowItems.begin();
    for ( ; iter != windowItems.end(); ++iter )
        iter.data().item->selectInGroup();
}

void WindowListBox::unselectAllFromGroup ()
{
    WindowMapIter iter = windowItems.begin();
    for ( ; iter != windowItems.end(); ++iter )
        iter.data().item->unselectFromGroup();
}

void WindowListBox::raiseTrussUnitWindowItem ( QListBoxItem* it )
{    
    if ( it == 0 )
        return;
    try { 
        TrussUnitWindowItem& item = dynamic_cast<TrussUnitWindowItem&>(*it);
        item.raise();
    }
    catch ( ... ) { return; }
}

void WindowListBox::trussWindowDesisted ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item && index( item ) != -1 )
        takeItem( item );
}

void WindowListBox::trussWindowRevived ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item && index( item ) == -1 ) {
        insertItem( item, windowItems[&truss].index );
        setCurrentItem( item );
    }
}

/*****************************************************************************/
