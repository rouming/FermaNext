
#ifndef PLUGINSRELOADER_H
#define PLUGINSRELOADER_H

#include <QDialog>

#include "Plugin.h"

class PluginManager;

class PluginReloader
{
public:
    static void reloadPlugins ( PluginManager& );
};

// Subsidiary private dialog
class ReloaderDialogPrivate : public QDialog
{
    Q_OBJECT
public slots:
    // Load
    virtual void beforePluginsLoad ( uint plgNum ) =0;
    virtual void afterPluginsLoad ( uint plgNum ) =0;
    virtual void beforePluginLoad ( const QString& path ) =0;
    virtual void afterPluginLoad ( Plugin& plg ) =0;
    // Unload
    virtual void beforePluginsUnload ( uint plgNum ) =0;
    virtual void afterPluginsUnload ( uint plgNum ) =0;
    virtual void beforePluginUnload ( Plugin& plg ) =0;
    virtual void afterPluginUnload ( const QString& path ) =0;
};

#endif //PLUGINSRELOADER_H
