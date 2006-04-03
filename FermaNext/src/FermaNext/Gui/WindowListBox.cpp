
#include <QString>
#include <QCursor>
#include <QMessageBox>

#include "WindowListBox.h"
#include "Global.h"
#include "TrussUnitActions.h"
#include "FermaNextWorkspace.h"
#include "TrussCalcData.h"
//FIXME QT3TO4
//#include "CalcDataWidget.h"
#include "CalculationInterface.h"

/*****************************************************************************
 * Truss Unit Window Item
 *****************************************************************************/

TrussUnitWindowItem::TrussUnitWindowItem ( FermaNextProject& prj,
                                           TrussUnitWindow& truss,
                                           WindowListBox& lb,
                                           const QPixmap& pix,
                                           const QPixmap& disPix ) :
    Q3ListBoxPixmap( &lb, pix, truss.getTrussName() ),
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

const QPixmap* TrussUnitWindowItem::pixmap () const
{
    if ( isShown() )
        return Q3ListBoxPixmap::pixmap();
    else
        return &disabledPix;        
}

void TrussUnitWindowItem::fillPopup ( Q3PopupMenu* popup ) const
{
    if ( isShown() ) {
        if ( isSelectedInGroup() ) {
            popup->insertItem( QIcon(imagesPath() + "/unselect.png"),
                               "Unselect", this, SLOT(unselectFromGroup()) );
        } else {
            popup->insertItem( QIcon(imagesPath() + "/select.png"),
                               "Select", this, SLOT(selectInGroup()) );
        }
        popup->insertSeparator();
        popup->insertItem( QIcon(imagesPath() + "/select.png"),
                           "Select All", this, SLOT(selectAllInGroup()) );
        popup->insertItem( QIcon(imagesPath() + "/unselect.png"),
                           "Unselect All", this, SLOT(unselectAllFromGroup()));
        popup->insertSeparator();
        popup->insertItem( QIcon(imagesPath() + "/calculate.png"),
                           "Calculate", this, SLOT(calculate()) );
        popup->insertSeparator();
        popup->insertItem( QIcon(imagesPath() + "/delete.png"),
                           "Remove", this, SLOT(remove()) );
        popup->insertSeparator();

        popup->insertItem( QIcon(imagesPath() + "/hide.png"),
                           "Hide", this, SLOT(hide()) );

    } else {
        popup->insertItem( QIcon(imagesPath() + "/delete.png"),
                           "Remove", this, SLOT(remove()) );
        popup->insertSeparator();

        popup->insertItem( QIcon(imagesPath() + "/show.png"),
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
    Q3ListBoxPixmap::setText( text );
}

void TrussUnitWindowItem::raise ()
{
    if ( ! trussWindow.isVisible() )
        return;
    TrussDesignerWidget& widget = project.getDesignerWidget();
    widget.focusOnWindow( trussWindow );
}

void TrussUnitWindowItem::show ()
{
    if ( trussWindow.isVisible() )
        return;
    trussWindow.setVisible( true );
    raise();
    update();
    emit onWindowItemShow( *this );
}

void TrussUnitWindowItem::hide ()
{
    if ( ! trussWindow.isVisible() )
        return;
    trussWindow.setVisible( false );
    update();
    emit onWindowItemHide( *this );
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
    FermaNextWorkspace& wsp = project.getWorkspace();    
    PluginManager& plgManager = wsp.pluginManager();
    PluginList plugins = plgManager.loadedPluginsOfType( CALCULATION_TYPE );
    if ( plugins.size() == 0 ) {
        QMessageBox::warning( 0, tr("Plugin manager warning"), 
                                 tr("Calculation plugin was not found "
                                    "in the plugin dir.") );
        return;
    }

    // Find first calculation plugin. 
    // TODO: plural calculation plugin support
    Plugin* plugin = plugins[0];

    try {
        // FIXME QT3TO4
        /*
        CalculationInterface& calcPlugin = 
            dynamic_cast<CalculationInterface&>(*plugin);

        // Try to find truss calc data widget
        CalcDataToolBar& calcToolBar = project.getCalcDataToolBar();        
        CalcDataWidget* calcForm = 
            calcToolBar.findCalcDataWidget( trussWindow );

        // Do calculation with new topology and calc data
        TrussCalcData calcData;
        TrussTopology& topology = trussWindow.createTopology();
        calcPlugin.calculate( topology, calcData );
        calcForm->initCalc( calcData );
        calcForm->show();

        // TODO: toplogy manager
        topology.desist();
        */
    }
    catch ( std::exception& ) {
        QMessageBox::critical( 0, tr("Plugin manager error"),
                               tr("Plugin '"+  plugin->pluginInfo().name + 
                                  "' has violated type contract.") );
    }
    catch ( ... ) {
        QMessageBox::critical( 0, tr("Plugin manager error"),
                                  tr("Internal plugin error. \n"
                                     "Please, inform plugin developer.") );
    }
}

void TrussUnitWindowItem::remove ()
{
    if ( QMessageBox::question( 0, tr("Truss unit deleting - \"%1\"").
                                  arg(trussWindow.getTrussName()),
                                tr("Delete truss window?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) ) {
        return;
    }
    // Remove truss window
    trussWindow.desist();
}

/*****************************************************************************
 * Windows List Box
 *****************************************************************************/

WindowListBox::WindowListBox ( FermaNextProject& prj, QWidget* parent,
                               const char* name, Qt::WFlags fl ) : 
    Q3ListBox( parent, name, fl ),
    project(prj)
{
    // Catch double click on item
    QObject::connect( this, SIGNAL(doubleClicked(Q3ListBoxItem*)),
                      SLOT(raiseWindowItem(Q3ListBoxItem*)) );

    // Catch focus changes
    TrussDesignerWidget& w = prj.getDesignerWidget();
    QObject::connect( &w, SIGNAL(onFocusReceive(TrussUnitWindow&)), 
                          SLOT(trussWindowReceivedFocus(TrussUnitWindow&)) );
    QObject::connect( &w, SIGNAL(onFocusLose(TrussUnitWindow&)), 
                          SLOT(trussWindowLostFocus(TrussUnitWindow&)) );

    // Catch truss creation/removing
    TrussUnitWindowManager& trussMng = project.getTrussUnitWindowManager();
    QObject::connect( &trussMng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
                      SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    QObject::connect( &trussMng, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
                      SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
}

void WindowListBox::contextMenuEvent ( QContextMenuEvent* )
{
    Q3ListBoxItem* selected = selectedItem();
    if ( selected == 0 )
        return;
    try { 
        TrussUnitWindowItem& item = 
            dynamic_cast<TrussUnitWindowItem&>( *selected ); 
        Q3PopupMenu* popup = new Q3PopupMenu( this );
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
                  QPixmap(imagesPath() + "/project.png"),
                  QPixmap(imagesPath() + "/project_d.png"));

    // Catches item visibility changes 
    QObject::connect( item, SIGNAL(onWindowItemShow(TrussUnitWindowItem&)), 
                            SLOT(showWindowItem(TrussUnitWindowItem&)) );
    QObject::connect( item, SIGNAL(onWindowItemHide(TrussUnitWindowItem&)), 
                            SLOT(hideWindowItem(TrussUnitWindowItem&)) );

    IndexedItem iitem = { item, index(item) };
    windowItems[&truss] = iitem;
    // Set selected newbie
    setSelected( item, true );
    triggerUpdate(true);
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

void WindowListBox::raiseWindowItem ( Q3ListBoxItem* it )
{    
    if ( it == 0 )
        return;
    try { 
        TrussUnitWindowItem& item = dynamic_cast<TrussUnitWindowItem&>(*it);
        item.raise();
    }
    catch ( ... ) { return; }
}

void WindowListBox::showWindowItem ( TrussUnitWindowItem& )
{    
}

void WindowListBox::hideWindowItem ( TrussUnitWindowItem& item )
{
    TrussDesignerWidget& w = project.getDesignerWidget();
    TrussUnitWindow* focusedWindow = w.getFocusedWindow();
    if ( focusedWindow == 0 || ! windowItems.contains(focusedWindow) )
        return;
    if ( &item == windowItems[focusedWindow].item )
        // Nothing to select
        return;

    setSelected( windowItems[focusedWindow].item, true );
    triggerUpdate(true);
}

void WindowListBox::trussWindowDesisted ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item && index( item ) != -1 )
        takeItem( item );    
    
    // Select focused window if it was not selected
    if ( selectedItem() )
        return;

    TrussDesignerWidget& w = project.getDesignerWidget();
    TrussUnitWindow* focusedWindow = w.getFocusedWindow();
    if ( focusedWindow == 0 || ! windowItems.contains(focusedWindow) )
        return;
    setSelected( windowItems[focusedWindow].item, true );
    triggerUpdate(true);    
}

void WindowListBox::trussWindowRevived ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item && index( item ) == -1 ) {
        insertItem( item, windowItems[&truss].index );
        setCurrentItem( item );
    }
}

void WindowListBox::trussWindowReceivedFocus ( TrussUnitWindow& window )
{
    if ( ! windowItems.contains(&window) )
        return;
    setSelected( windowItems[&window].item, true );
    triggerUpdate(true);
}

void WindowListBox::trussWindowLostFocus ( TrussUnitWindow& window )
{
    if ( ! windowItems.contains(&window) )
        return;
    setSelected( windowItems[&window].item, false );
    triggerUpdate(true);
}

/*****************************************************************************/
