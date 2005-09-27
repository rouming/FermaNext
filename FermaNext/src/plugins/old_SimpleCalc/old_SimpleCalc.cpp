
#include "old_SimpleCalc.h"
#include "Plugin.h"
#include <stdio.h>

#if defined WINDOWS || defined WIN32  
  #include "win_SimpleCalc.h"
#else
  #include "unix_SimpleCalc.h"
#endif

/*****************************************************************************
 * Old Simple Calculation plugin (main init/fini routines)
 *****************************************************************************/

PluginInfo inf = { "SimpleOldCalc", "Just calculation", CALCULATION_PLUGIN };
FERMA_NEXT_PLUGIN(inf)

PluginExport void StandardCall calculation ( TrussTopology& truss,
                                             CalcData& data  )
{
    plugin.calculate( truss, data );
}

/*****************************************************************************/
