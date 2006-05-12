
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
                                           const QIcon& icon,
                                           const QIcon& disIcon ) :
    QListWidgetItem( icon, truss.getTrussName(), &lb ),
    listBox(lb),
    project(prj), 
    trussWindow(truss),
    disabledIcon(disIcon),
    selected(false)
{
    // Catch renaming
    QObject::connect( &truss, SIGNAL(onTrussNameChange(const QString&)),
                              SLOT(setText(const QString&)) );
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

QVariant TrussUnitWindowItem::data ( int role ) const
{
    if ( role != Qt::DecorationRole )
        return QListWidgetItem::data( role );
    
    if ( isShown() )
        return QListWidgetItem::data( role );
    else
        return disabledIcon;
}

void TrussUnitWindowItem::fillPopup ( QMenu* popup ) const
{
    if ( isShown() ) {
        if ( isSelectedInGroup() ) {
            popup->addAction( QIcon(Global::imagesPath() + "/unselect.png"),
                              "Unselect", this, SLOT(unselectFromGroup()) );
        } else {
            popup->addAction( QIcon(Global::imagesPath() + "/select.png"),
                              "Select", this, SLOT(selectInGroup()) );
        }
        popup->addSeparator();
        popup->addAction( QIcon(Global::imagesPath() + "/select.png"),
                          "Select All", this, SLOT(selectAllInGroup()) );
        popup->addAction( QIcon(Global::imagesPath() + "/unselect.png"),
                          "Unselect All", this, SLOT(unselectAllFromGroup()));
        popup->addSeparator();
        popup->addAction( QIcon(Global::imagesPath() + "/calculate.png"),
                          "Calculate", this, SLOT(calculate()) );
        popup->addSeparator();
        popup->addAction( QIcon(Global::imagesPath() + "/delete.png"),
                          "Remove", this, SLOT(remove()) );
        popup->addSeparator();

        popup->addAction( QIcon(Global::imagesPath() + "/hide.png"),
                          "Hide", this, SLOT(hide()) );

    } else {
        popup->addAction( QIcon(Global::imagesPath() + "/show.png"),
                          "Show", this, SLOT(show()) );
        popup->addSeparator();
        popup->addAction( QIcon(Global::imagesPath() + "/delete.png"),
                          "Remove", this, SLOT(remove()) );

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
    QListWidgetItem::setText( text );
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
    emit onWindowItemShow( *this );
}

void TrussUnitWindowItem::hide ()
{
    if ( ! trussWindow.isVisible() )
        return;
    trussWindow.setVisible( false );
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
                               tr("Plugin '") + plugin->pluginInfo().name + 
                                  "' has violated type contract." );
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

WindowListBox::WindowListBox ( FermaNextProject& prj, QWidget* parent ) : 
    QListWidget( parent ),
    project(prj)
{
    // Catch double click on item
    QObject::connect( this, SIGNAL(itemSelectionChanged()),
                      SLOT(raiseSelectedWindowItem()) );

    // Catch focus changes
    TrussDesignerWidget& w = prj.getDesignerWidget();
    QObject::connect( &w, SIGNAL(onFocusReceive(TrussUnitWindow&)), 
                          SLOT(trussWindowReceivedFocus(TrussUnitWindow&)) );
    QObject::connect( &w, SIGNAL(onFocusLose(TrussUnitWindow&)), 
                          SLOT(trussWindowLostFocus(TrussUnitWindow&)) );

    // Catch truss creation/removing
    TrussUnitWindowManager& trussMng = project.getTrussUnitWindowManager();
    QObject::connect( &trussMng, 
                      SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
                      SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    QObject::connect( &trussMng, 
                      SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
                      SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
}

void WindowListBox::contextMenuEvent ( QContextMenuEvent* )
{
    QList<QListWidgetItem*> items = selectedItems();
    if ( items.size() == 0 )
        return;
    QListWidgetItem* selected = items.at(0);

    try { 
        TrussUnitWindowItem& item = 
            dynamic_cast<TrussUnitWindowItem&>( *selected ); 
        QMenu* popup = new QMenu( this );
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
                  QIcon(Global::imagesPath() + "/project.png"),
                  QIcon(Global::imagesPath() + "/project_d.png"));

    IndexedItem iitem = { item, row(item) };
    windowItems[&truss] = iitem;
    // Set selected newbie
    setItemSelected( item, true );
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
        iter.value().item->selectInGroup();
}

void WindowListBox::unselectAllFromGroup ()
{
    WindowMapIter iter = windowItems.begin();
    for ( ; iter != windowItems.end(); ++iter )
        iter.value().item->unselectFromGroup();
}

void WindowListBox::raiseSelectedWindowItem ()
{
    QList<QListWidgetItem*> selected = selectedItems();
    if ( selected.size() != 1 )
        return;
    try {
        TrussUnitWindowItem& item = 
            dynamic_cast<TrussUnitWindowItem&>( *selected.at(0) );
        item.raise();
    }
    catch ( ... ) { return; }
}

void WindowListBox::trussWindowDesisted ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    int index = row( item );
    if ( item && index != -1 ) {
        takeItem( index );
    }
    
    // Select focused window if it was not selected
    if ( selectedItems().size() )
        return;

    TrussDesignerWidget& w = project.getDesignerWidget();
    TrussUnitWindow* focusedWindow = w.getFocusedWindow();
    if ( focusedWindow == 0 || ! windowItems.contains(focusedWindow) )
        return;
    setItemSelected( windowItems[focusedWindow].item, true );
}

void WindowListBox::trussWindowRevived ( const TrussUnitWindow& truss )
{
    TrussUnitWindowItem* item = findByTrussUnitWindow( truss );
    if ( item && row( item ) == -1 ) {
        insertItem( windowItems[&truss].index, item );
        setCurrentItem( item );
    }
}

void WindowListBox::trussWindowReceivedFocus ( TrussUnitWindow& window )
{
    if ( ! windowItems.contains(&window) )
        return;
    setItemSelected( windowItems[&window].item, true );
}

void WindowListBox::trussWindowLostFocus ( TrussUnitWindow& window )
{
    if ( ! windowItems.contains(&window) )
        return;
    setItemSelected( windowItems[&window].item, false );
}

/*****************************************************************************/
