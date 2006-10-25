
#ifndef PLUGINEXECUTORDIALOG_H
#define PLUGINEXECUTORDIALOG_H

#include <QDialog>

#include "Plugin.h"
#include "PluginManager.h"

#include "ui_PluginExecutorDialog.h"

class PluginExecutorDialog : public QDialog, 
                             public Ui::PluginExecutorDialog
{
    Q_OBJECT
public:
    PluginExecutorDialog ( PluginManager&, QWidget* parent = 0 );

    void executePlugin ( Plugin*, const QList<UUIDObject*>& );

private:
    void showExecutionTree ();

    void execute ( Plugin*, const PluginExecutionTree&, 
                   const QList<UUIDObject*>& );

    
private:
    PluginManager& plgMng;
    PluginExecutionTree execTree;
    QList<UUIDObject*> execParams;
    Plugin* plugin;
};

#endif //PLUGINEXECUTORDIALOG_H
