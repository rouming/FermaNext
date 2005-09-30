
#include "old_SimpleCalc.h"
#include "Plugin.h"

#if defined WINDOWS || defined WIN32  
  #include "win_SimpleCalc.h"
#else
  #include "unix_SimpleCalc.h"
#endif

/*****************************************************************************
 * Old Simple Calculation plugin (main init/fini routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(os_dependent_SimpleCalcPlugin)

    /*
PluginExport void StandardCall calculation ( TrussTopology& truss,
                                             CalcData& data  )
{
    plugin.calculate( truss, data );
}
*/
/*****************************************************************************/
