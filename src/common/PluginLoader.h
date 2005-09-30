
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "DynaLoader.h"

class Plugin;

class PluginLoader
{
public:
    // Excceptions
    class PluginLoadException {};

    PluginLoader ( const QString& pluginName ) throw (PluginLoadException);
    virtual ~PluginLoader ();

    virtual Plugin& pluginInstance ();
    virtual const Plugin& pluginInstance () const;

private:
    DynaLoader dynaLoader;
    Plugin* plugin;
};

#endif //PLUGINLOADER_H
