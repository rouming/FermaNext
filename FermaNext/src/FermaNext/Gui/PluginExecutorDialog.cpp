
#include <QMessageBox>

#include "PluginExecutorDialog.h"

// TODO: for now this headers are used for passing execution results to 
//       result manager. in future should be done in more attractive way.
#include "FermaNextWorkspace.h"
#include "FermaNextMainWindow.h"

/*****************************************************************************/

PluginExecutorDialog::PluginExecutorDialog ( PluginManager& mng, QWidget* p ) :
    QDialog(p),
    plgMng(mng),
    execTree(mng),
    plugin(0)
{
    setupUi(this);

    QObject::connect( includeButton, SIGNAL(clicked(bool)),
                      SLOT(includeIsPressed()) );

    QObject::connect( excludeButton, SIGNAL(clicked(bool)),
                      SLOT(excludeIsPressed()) );

    QObject::connect( executeButton, SIGNAL(clicked(bool)),
                      SLOT(executeIsPressed()) );                      
                      
}

void PluginExecutorDialog::executePlugin ( Plugin* plg, 
                                           const QList<UUIDObject*>& params )
{
    // Save plugin, params and build execution tree for future use
    execParams = params;
    execTree.buildExecutionTree(plg);
    plugin = plg;

    // If top node is alone (i.e. plugin does not have dependencies), 
    // just silently execute plugin.
    if ( execTree.getTreeTop().childNodes().size() == 0 ) {
        // Mark top node as usable
        execTree.getTreeTop().use(true);
        execute( plg, execTree, params );
    }
    else {
        showExecutionTree();
    }
}

void PluginExecutorDialog::execute ( Plugin* plg, 
                                     const PluginExecutionTree& execTree,
                                     const QList<UUIDObject*>& params )
{
    try { 
        plgMng.preparePluginExecutionContext( execTree );
        Plugin::ExecutionResult results = plg->execute( params );
        
        // TODO: in future should be replaced by attractive call of 
        //       results manager
        FermaNextMainWindow* mw = FermaNextWorkspace::workspace().mainWindow();
        mw->pluginWasExecuted( plg, results );
    }
    catch ( PluginExecutionContext::ContextIsEmptyException& ) {
        QMessageBox::warning( this, tr("Context warning"), 
          tr("Execution tree does not prepared properly for context.") );
    }
    catch ( Plugin::ContextIsNotValidException& ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin execution context is not prepared properly.") );
    }
    catch ( Plugin::ParamsAreNotAcceptedException& e ) {
        QStringList problemParams;        
        for ( int i = 0; e.problemList.size(); ++i ) {
            QPair<QString,QString>& p = e.problemList[i];
            problemParams.append( QString("%1 is %2").arg(p.first, p.second) );
        }
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Following plugin params are not accepted by plugin:\n") + 
          problemParams.join("\n") );
    }
    catch ( Plugin::DependenciesAreNotResolvedException& e ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Following plugin dependencies are not resolved:\n") +
          e.unresolvedTypes.join("\n") );
    }
    catch ( Plugin::WrongExecutionArgsException& ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin execution arguments are wrong.") );
    }
    catch ( ... ) {
        QMessageBox::critical( this, tr("Plugin error"), 
          tr("Plugin has been generated unknown exception while execution.") );
    }
}

void PluginExecutorDialog::showExecutionTree ()
{
    Q_ASSERT(plugin);

    QList<PluginExecutionTree::Node> nodes
        = execTree.getTreeTop().childNodes();
    addNodesToExecutionTree(nodes);
    
    show();
}

void PluginExecutorDialog::addNodesToExecutionTree ( 
    const QList<PluginExecutionTree::Node>& nodes )
{
    Q_ASSERT(plugin);

    // Always include top node in execution process
    execTree.getTreeTop().use( true );

    executionTreeWidget->clear();
    foreach ( PluginExecutionTree::Node node, nodes ) {
        if ( node.isNull() || node.getPlugin() == 0 )
            continue;
        Plugin* plugin = node.getPlugin();
        QTreeWidgetItem* item = new QTreeWidgetItem(executionTreeWidget);
        if ( ! node.canBeResolved() )
            // Disable item
            item->setFlags( item->flags() & ~Qt::ItemIsEnabled );

        item->setText( 0, plugin->pluginInfo().name );
        item->setText( 1, plugin->pluginInfo().type );
        item->setData( 0, Qt::UserRole, plugin->getUUID() );

        addNodesToExecutionTreeItem( item, node.childNodes() );
    }    
}

void PluginExecutorDialog::addNodesToExecutionTreeItem ( 
    QTreeWidgetItem* prevItem,
    const QList<PluginExecutionTree::Node>& nodes )
{
    foreach ( PluginExecutionTree::Node node, nodes ) {
        if ( node.isNull() || node.getPlugin() == 0 )
            continue;
        Plugin* plugin = node.getPlugin();
        QTreeWidgetItem* item = new QTreeWidgetItem(prevItem);
        if ( ! node.canBeResolved() )
            // Disable item
            item->setFlags( item->flags() & ~Qt::ItemIsEnabled );

        item->setText( 0, plugin->pluginInfo().name );
        item->setText( 1, plugin->pluginInfo().type );
        item->setData( 0, Qt::UserRole, plugin->getUUID() );

        addNodesToExecutionTreeItem( item, node.childNodes() );
    }
}

void PluginExecutorDialog::includeIsPressed ()
{
    QTreeWidgetItem* item = executionTreeWidget->currentItem();
    if ( item == 0 )
        return;

    QFont font = item->font( 0 );
    font.setBold(true);
    item->setFont( 0, font );
    item->setFont( 1, font );

    QString uuid = item->data(0, Qt::UserRole).toString();
    PluginExecutionTree::Node node = execTree.findNodeByPluginUUID( uuid );
    if ( node.isNull() || node.getPlugin() == 0 ) {
        qWarning( "Can't find node by plugin uuid '%s'", qPrintable(uuid) );
        return;
    }

    node.use( true );
}

void PluginExecutorDialog::excludeIsPressed ()
{
    QTreeWidgetItem* item = executionTreeWidget->currentItem();
    if ( item == 0 )
        return;

    QFont font = item->font( 0 );
    font.setBold(false);
    item->setFont( 0, font );
    item->setFont( 1, font );

    QString uuid = item->data(0, Qt::UserRole).toString();
    PluginExecutionTree::Node node = execTree.findNodeByPluginUUID( uuid );
    if ( node.isNull() || node.getPlugin() == 0 ) {
        qWarning( "Can't find node by plugin uuid '%s'", qPrintable(uuid) );
        return;
    }

    node.use( false );
}

void PluginExecutorDialog::executeIsPressed ()
{
    Q_ASSERT(plugin);
    execute( plugin, execTree, execParams );
}

/*****************************************************************************/
