
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include <qmap.h>
#include <qstringlist.h>
#include "Plugin.h"

typedef PluginChecksum PluginHandle;
typedef std::vector<PluginHandle> PluginHandleList ;

class PluginLoader;

class PluginManager 
{
protected:
    void clean ();

public:
    // Exceptions
    class FindException {};

    virtual ~PluginManager ();

    virtual void loadPlugins ( const QString& path );
    virtual void unloadPlugins ();
    virtual void unloadPlugin ( const PluginHandle& );
    virtual Plugin& findPlugin ( const PluginHandle& ) throw (FindException);

    virtual PluginHandleList loadedPlugins () const;
    virtual QStringList loadedPluginsNames () const;
    virtual PluginHandleList loadedPluginsOfType ( const PluginType& ) const;

private:
    typedef QMap<PluginHandle, PluginLoader*> PluginsMap;
    typedef PluginsMap::iterator PluginsMapIter;
    typedef PluginsMap::const_iterator PluginsMapConstIter;

    PluginsMap plugins;
};

#endif //PLUGINMANAGER_H
