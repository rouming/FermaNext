
const char* PLUGIN_INFO_CALL = "get_plugin_info";

enum PluginType { CALCULATION_PLUGIN = 0, 
                  OPTIMIZATION_PLUGIN };

struct PluginInfo
{
    const char* name;
    const char* description;
    PluginType type;
};

FermaNextPlugin.h

typedef PluginHandle uint;
typedef PluginHandleList std::vector<PluginHandle>;

class PluginLoader 
{
public:
    virtual void findAndLoadPlugins ( const QString& path );
    virtual void unloadPlugins ();
    virtual PluginHandle loadPlugin ( const QString& pluginName );
    virtual void unloadPlugin ( const QString& pluginName );

    virtual PluginHandleList loadedPlugins () const;
    virtual PluginHandleList loadedPluginsOfType( const PluginType& ) const;
};
