
#ifndef PLUGINEXECUTORDIALOG_H
#define PLUGINEXECUTORDIALOG_H

#include <QDialog>
#include <QHBoxLayout>

#include "Plugin.h"
#include "PluginManager.h"

#include "ui_PluginExecutorDialog.h"

class PluginExecutorDialog : public QDialog, 
                             public Ui::PluginExecutorDialog
{
    Q_OBJECT
public:
    PluginExecutorDialog ( PluginManager&, QWidget* parent = 0 );
    ~PluginExecutorDialog ();

    void executePlugin ( Plugin*, const QList<UUIDObject*>& );

private slots:
    void includeIsPressed ();
    void excludeIsPressed ();
    void executeIsPressed ();
    void stopIsPressed ();

    void onCurrentItemChanged ( QTreeWidgetItem*, QTreeWidgetItem* );

private:
    void clearParamsWidgets ();

    QString getPluginNameByPluginUuid ( const QString& );

    PluginExecutionParams widgetToExecutionParams ( QWidget* );
    void setExecutionParams ();
    void showExecutionTree ();
    void addNodesToExecutionTree ( const QList<PluginExecutionTree::Node>& );
    void addNodesToExecutionTreeItem ( QTreeWidgetItem*,
                const QList<PluginExecutionTree::Node>& );

    QWidget* createLabel ( const QString&, QWidget* parent = 0 );
    QWidget* createPluginParamsWidget ( const Plugin*, QWidget* parent = 0 );

    void execute ( Plugin*, const PluginExecutionTree&, 
                   const QList<UUIDObject*>& );

    
private:
    QHBoxLayout* paramsLayout;
    QHBoxLayout* depParamsLayout;
    PluginManager& plgMng;
    PluginExecutionTree execTree;
    QList<UUIDObject*> execParams;
    QHash<QString, QWidget*> paramsWidgets;
    QList<QString> usedUuids;
    Plugin* plugin;
};

#endif //PLUGINEXECUTORDIALOG_H
