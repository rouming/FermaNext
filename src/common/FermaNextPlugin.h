
#ifndef FERMANEXTPLUGIN_H
#define FERMANEXTPLUGIN_H

enum PluginType { CALCULATION_PLUGIN = 0, 
                  OPTIMIZATION_PLUGIN };

struct PluginInfo
{
    const char* name;
    const char* description;
    PluginType type;
};

// Plugin signature. Plugin name should be short and unique.
const char PLUGIN_INFO_CALL[] = "ferma_next_plugin_info";
typedef PluginInfo& (* PluginInfoCall ) ();

#endif //FERMANEXTPLUGIN_H
