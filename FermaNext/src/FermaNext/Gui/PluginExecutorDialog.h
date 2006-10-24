
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

    
private:
    PluginManager& plgMng;    
};

#endif //PLUGINEXECUTORDIALOG_H
