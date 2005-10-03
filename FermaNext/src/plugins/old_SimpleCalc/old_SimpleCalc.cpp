
#include "old_SimpleCalc.h"
#include "TrussTopology.h"
#include "TrussCalcData.h"
#include "FRMWriter.h"
#include "VYVReader.h"
#include <qfile.h>

#if defined WINDOWS || defined WIN32  
  #include "win_SimpleCalc.h"
#else
  #include "unix_SimpleCalc.h"
#endif

/*****************************************************************************
 * Old Simple Calculation plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(os_dependent_SimpleCalcPlugin)

/*****************************************************************************/

SimpleCalcPlugin::SimpleCalcPlugin ()
{ 
    createTempFile(); 
}

SimpleCalcPlugin::~SimpleCalcPlugin ()
{ 
    destroyTempFile(); 
}

const PluginInfo& SimpleCalcPlugin::pluginInfo () const
{ 
    static PluginInfo inf = { "SimpleOldCalcPlugin", "Just calculation" };
    return inf; 
}

PluginType SimpleCalcPlugin::pluginType () const
{ 
    return CALCULATION_PLUGIN; 
}

bool SimpleCalcPlugin::dependsOn ( PluginType ) const
{ 
    return false; 
}

void SimpleCalcPlugin::calculate ( TrussTopology& truss, 
                                   TrussCalcData& data ) const
{
    FRMWriter frm(truss);
    frm.write( tempFileName() );
    startCalculation( tempFileName() );
    VYVReader vyv(data);
    vyv.read( tempFileName() + fermaResExt );
}

void SimpleCalcPlugin::createTempFile ()
{  
    tempFile = tmpnam(0); 
}

void SimpleCalcPlugin::destroyTempFile () 
{ 
    QFile::remove( tempFile + fermaResExt );
    QFile::remove( tempFile ); 
}

const QString& SimpleCalcPlugin::tempFileName () const
{ 
    return tempFile; 
}

/*****************************************************************************/
