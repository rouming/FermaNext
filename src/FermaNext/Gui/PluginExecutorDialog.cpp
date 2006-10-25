
#include <QMessageBox>

#include "PluginExecutorDialog.h"

/*****************************************************************************/

PluginExecutorDialog::PluginExecutorDialog ( PluginManager& mng, QWidget* p ) :
    QDialog(p),
    plgMng(mng)    
{
    setupUi(this);
}

void PluginExecutorDialog::executePlugin ( Plugin* plg, 
                                           const QList<UUIDObject*>& params )
{
    const PluginExecutionTree& execTree = plgMng.buildExecutionTree(plg);

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
        plg->execute( params );
    }
    catch ( PluginExecutionContext::ContextIsEmptyException& ) {
        QMessageBox::warning( this, tr("Context warning"), 
          tr("Execution tree does not prepared properly for context.") );
    }
    catch ( Plugin::ContextIsNotValidException& ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin execution context is not prepared properly.") );
    }
    catch ( Plugin::ParamsAreNotAcceptedException& ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin params are not accepted by plugin.") );
    }
    catch ( Plugin::DependenciesAreNotResolvedException& ) {
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin dependencies are not resolved.") );
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
    //TODO: show tree dependencies representation

    show();

    qWarning("Not implemented yet");
}

/*****************************************************************************/
