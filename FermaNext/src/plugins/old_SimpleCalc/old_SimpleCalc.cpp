
#include "FermaNextPlugin.h"

/*****************************************************************************
 * Old Simple Calculation plugin
 *****************************************************************************/

PluginInfo inf = { "SimpleOldCalc", 
                   "Some description goes here", 
                   CALCULATION_PLUGIN 
                 };

PluginExport
PluginInfo& ferma_next_plugin_info ()
{
    return inf;
}

/*****************************************************************************/
