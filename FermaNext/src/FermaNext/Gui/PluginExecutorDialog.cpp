
#include <QUiLoader>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>

#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QDial>

#include "PluginExecutorDialog.h"

// TODO: for now this headers are used for passing execution results to 
//       result manager. in future should be done in more attractive way.
#include "FermaNextWorkspace.h"
#include "FermaNextMainWindow.h"

/*****************************************************************************/

PluginExecutorDialog::PluginExecutorDialog ( PluginManager& mng, QWidget* p ) :
    QDialog(p),
    paramsLayout(0),
    depParamsLayout(0),
    plgMng(mng),
    execTree(mng),
    plugin(0)
{
    setupUi(this);

    // Create layout params group boxes
    paramsLayout = new QHBoxLayout( paramsGroupBox );
    depParamsLayout = new QHBoxLayout( depParamsGroupBox );

    // Connect buttons
    QObject::connect( includeButton, SIGNAL(clicked(bool)),
                      SLOT(includeIsPressed()) );

    QObject::connect( excludeButton, SIGNAL(clicked(bool)),
                      SLOT(excludeIsPressed()) );

    QObject::connect( executeButton, SIGNAL(clicked(bool)),
                      SLOT(executeIsPressed()) );

    
    // Connect tree
    QObject::connect( executionTreeWidget, 
                      SIGNAL(currentItemChanged(QTreeWidgetItem*, 
                                                QTreeWidgetItem*)),
                      SLOT(onCurrentItemChanged(QTreeWidgetItem*, 
                                                QTreeWidgetItem*)) );
}

PluginExecutorDialog::~PluginExecutorDialog ()
{
    clearParamsWidgets();
}

void PluginExecutorDialog::clearParamsWidgets ()
{
    // Clear used uuids
    usedUuids.clear();

    // Clear params layout
    QLayoutItem* child = 0;
    while ( (child = paramsLayout->takeAt(0)) != 0 )
        delete child;

    // Clear params layout
    while ( (child = depParamsLayout->takeAt(0)) != 0 )
        delete child;

    // Clear params widgets
    foreach ( QWidget* w, paramsWidgets.values() )
        delete w;
    paramsWidgets.clear();
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

QWidget* PluginExecutorDialog::createLabel ( 
    const QString& msg,
    QWidget* parent )
{
    QLabel* label = new QLabel( parent );
    QPalette palette;
    label->setPalette(palette);
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    font.setItalic(false);
    font.setUnderline(false);
    font.setWeight(55);
    font.setStrikeOut(false);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setText( msg );
    return label;
}

QWidget* PluginExecutorDialog::createPluginParamsWidget ( 
    const Plugin* plugin, QWidget* parent )
{
    Q_ASSERT(plugin);

    QFileInfo plgPathInfo( plugin->pluginPath() );
    QString plgPathBaseName = plgPathInfo.completeBaseName();
    // Remove plugin extension
    const QString& plgExt = PluginManager::systemPluginExtension();
    plgPathBaseName = plgPathBaseName.remove( "." + plgExt );
    plgPathBaseName = plgPathBaseName.remove( QRegExp("^lib") );

    QFile file( Global::pluginParamsPath() + Global::pathSeparator() +
                plgPathBaseName + ".ui" );
    if ( ! file.exists() )
        return 0;
    
    file.open( QFile::ReadOnly );
    QUiLoader loader;
    QWidget* plgParamsWidget = loader.load( &file, parent );
    file.close();

    return plgParamsWidget;
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
    catch ( Plugin::ContextIsNotValidException& e ) {
        QString name = getPluginNameByPluginUuid(e.pluginUuid);
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin '%1' said: execution context is not prepared properly.").
            arg(name));
    }
    catch ( Plugin::ParamsAreNotAcceptedException& e ) {
        QStringList problemParams;        
        for ( int i = 0; i < e.problemList.size(); ++i ) {
            QPair<QString,QString>& p = e.problemList[i];
            problemParams.append( QString("%1 : %2").arg(p.first, p.second) );
        }
        QString name = getPluginNameByPluginUuid(e.pluginUuid);
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin '%1' said: following params are not accepted:\n").
            arg(name) + problemParams.join("\n") );
    }
    catch ( Plugin::DependenciesAreNotResolvedException& e ) {
        QString name = getPluginNameByPluginUuid(e.pluginUuid);
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin '%1' said: following dependencies are not resolved:\n").
            arg(name) + e.unresolvedTypes.join("\n") );
    }
    catch ( Plugin::WrongExecutionArgsException& e ) {
        QString name = getPluginNameByPluginUuid(e.pluginUuid);
        QMessageBox::warning( this, tr("Plugin warning"), 
          tr("Plugin '%1' said: execution arguments are wrong.").
            arg(name) );
    }
    catch ( ... ) {
        QMessageBox::critical( this, tr("Plugin error"), 
          tr("Plugin has been generated unknown exception while execution.") );
    }
}

QString PluginExecutorDialog::getPluginNameByPluginUuid ( const QString& uuid )
{
    Plugin* plugin = plgMng.findPluginByUUID(uuid);
    if ( plugin == 0 ) {
        return "Unknown";
    }
    return plugin->pluginInfo().name;
}

PluginExecutionParams PluginExecutorDialog::widgetToExecutionParams ( 
    QWidget* paramsWidget )
{
    Q_ASSERT(paramsWidget);

    QList<QCheckBox*> checkBoxes = qFindChildren<QCheckBox*>(paramsWidget);
    QList<QRadioButton*> rButtons = qFindChildren<QRadioButton*>(paramsWidget);
    QList<QComboBox*> comboBoxes = qFindChildren<QComboBox*>(paramsWidget);
    QList<QLineEdit*> lineEdits = qFindChildren<QLineEdit*>(paramsWidget);
    QList<QTextEdit*> textEdits = qFindChildren<QTextEdit*>(paramsWidget);
    QList<QSpinBox*> spins = qFindChildren<QSpinBox*>(paramsWidget);
    QList<QDoubleSpinBox*> dSpins = qFindChildren<QDoubleSpinBox*>(paramsWidget);
    QList<QSlider*> sliders = qFindChildren<QSlider*>(paramsWidget);
    QList<QDial*> dials = qFindChildren<QDial*>(paramsWidget);


    PluginExecutionParams params;

    // Convert QCheckBox to params
    foreach ( QCheckBox* object, checkBoxes ) {
        Qt::CheckState checked = object->checkState();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant((int)checked) );
    }

    // Convert QRadioButton to params
    foreach ( QRadioButton* object, rButtons ) {
        bool checked = object->isChecked();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant((int)checked) );
    }

    // Convert QComboBox to params
    foreach ( QComboBox* object, comboBoxes ) {
        int index = object->currentIndex();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(index) );
    }

    // Convert QLineEdit to params
    foreach ( QLineEdit* object, lineEdits ) {
        QString text = object->text();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(text) );
    }

    // Convert QTextEdit to params
    foreach ( QTextEdit* object, textEdits ) {
        QString text = object->toPlainText();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(text) );
    }

    // Convert QSpinBox to params
    foreach ( QSpinBox* object, spins ) {
        int value = object->value();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(value) );
    }

    // Convert QDoubleSpinBox to params
    foreach ( QDoubleSpinBox* object, dSpins ) {
        double value = object->value();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(value) );
    }

    // Convert QSlider to params
    foreach ( QSlider* object, sliders ) {
        int value = object->value();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(value) );
    }

    // Convert QDial to params
    foreach ( QDial* object, dials ) {
        int value = object->value();
        QString name = object->objectName();
        if ( name.isEmpty() ) {
            qWarning("Object of class '%s' has empty name. " 
                     "Could not be saved to params.", 
                     object->metaObject()->className() );
            continue;
        }
        params.addParam( name, QVariant(value) );
    }

    return params;
}

void PluginExecutorDialog::setExecutionParams ()
{
    foreach ( QString uuid, usedUuids ) {
        PluginExecutionTree::Node node = execTree.findNodeByUUID( uuid );
        if ( node.isNull() || node.getPlugin() == 0 ) {
            qWarning( "setExecutionParams: can't find node by plugin uuid "
                      "'%s'", qPrintable(uuid) );
            return;
        }

        if ( ! paramsWidgets.contains(uuid) ) {
            qWarning( "setExecutionParams: can't find params widget by plugin "
                      "uuid '%s'", qPrintable(uuid) );
            return;
        }

        node.setPluginParams( widgetToExecutionParams(paramsWidgets[uuid]) );
    }        
}

void PluginExecutorDialog::showExecutionTree ()
{
    Q_ASSERT(plugin);

    // Destroy previous widgets
    clearParamsWidgets();

    // Add params widget
    QWidget* plgWidget = createPluginParamsWidget( plugin, paramsGroupBox );

    if ( plgWidget == 0 )
        plgWidget = createLabel( "Params do not exist", paramsGroupBox );
    
    // Save widget
    paramsWidgets[execTree.getTreeTop().uuid()] = plgWidget;

    paramsLayout->addWidget( plgWidget );

    // Create tree
    QList<PluginExecutionTree::Node> nodes =
        execTree.getTreeTop().childNodes();
    addNodesToExecutionTree(nodes);
    
    show();
}

void PluginExecutorDialog::addNodesToExecutionTree ( 
    const QList<PluginExecutionTree::Node>& nodes )
{
    Q_ASSERT(plugin);

    // Always include top node in execution process
    execTree.getTreeTop().use( true );
    usedUuids.append( execTree.getTreeTop().uuid() );

    executionTreeWidget->clear();
    foreach ( PluginExecutionTree::Node node, nodes ) {
        if ( node.isNull() || node.getPlugin() == 0 )
            continue;
        Plugin* plugin = node.getPlugin();

        // Create plugin params widget
        QWidget* plgWidget = 
            createPluginParamsWidget( plugin, depParamsGroupBox );

        if ( plgWidget == 0 )
            plgWidget = createLabel( "Params<br>do not exist",
                                     depParamsGroupBox );
        // It is not visible by default
        plgWidget->setVisible(false);
        // Save widget
        paramsWidgets[node.uuid()] = plgWidget;

        QTreeWidgetItem* item = new QTreeWidgetItem(executionTreeWidget);
        if ( ! node.canBeResolved() )
            // Disable item
            item->setFlags( item->flags() & ~Qt::ItemIsEnabled );

        item->setText( 0, plugin->pluginInfo().name );
        item->setText( 1, plugin->pluginInfo().type );
        item->setData( 0, Qt::UserRole, node.uuid() );

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

        // Create plugin params widget
        QWidget* plgWidget = 
            createPluginParamsWidget( plugin, depParamsGroupBox );

        if ( plgWidget == 0 )
            plgWidget = createLabel( "Params<br>do not exist",
                                     depParamsGroupBox );
        // It is not visible by default
        plgWidget->setVisible(false);
        // Save widget
        paramsWidgets[node.uuid()] = plgWidget;

        QTreeWidgetItem* item = new QTreeWidgetItem(prevItem);
        if ( ! node.canBeResolved() )
            // Disable item
            item->setFlags( item->flags() & ~Qt::ItemIsEnabled );

        item->setText( 0, plugin->pluginInfo().name );
        item->setText( 1, plugin->pluginInfo().type );
        item->setData( 0, Qt::UserRole, node.uuid() );

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
    PluginExecutionTree::Node node = execTree.findNodeByUUID( uuid );
    if ( node.isNull() || node.getPlugin() == 0 ) {
        qWarning( "Can't find node by plugin uuid '%s'", qPrintable(uuid) );
        return;
    }

    usedUuids.append( uuid );
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
    PluginExecutionTree::Node node = execTree.findNodeByUUID( uuid );
    if ( node.isNull() || node.getPlugin() == 0 ) {
        qWarning( "Can't find node by plugin uuid '%s'", qPrintable(uuid) );
        return;
    }

    usedUuids.removeAll( uuid );

    node.use( false );
}

void PluginExecutorDialog::executeIsPressed ()
{
    Q_ASSERT(plugin);

    setExecutionParams();

    execute( plugin, execTree, execParams );
}

void PluginExecutorDialog::onCurrentItemChanged ( 
    QTreeWidgetItem* current, 
    QTreeWidgetItem* )
{
    if ( current == 0 )
        return;

    QString uuid = current->data(0, Qt::UserRole).toString();
    PluginExecutionTree::Node node = execTree.findNodeByUUID( uuid );
    if ( node.isNull() || node.getPlugin() == 0 ) {
        qWarning( "Can't find node by plugin uuid '%s'", qPrintable(uuid) );
        return;
    }

    if ( ! paramsWidgets.contains(uuid) ) {
        qWarning( "Can't find params widget by plugin uuid '%s'", 
                  qPrintable(uuid) );
        return;
    }

    QWidget* plgWidget = paramsWidgets[uuid];

    // Clear dependence params layout
    QLayoutItem* child = 0;
    while ( (child = depParamsLayout->takeAt(0)) != 0 ) {
        child->widget()->setVisible(false);
        delete child;
    }
    plgWidget->setVisible(false);
    depParamsLayout->addWidget(plgWidget);
    plgWidget->setVisible(true);
}

/*****************************************************************************/
