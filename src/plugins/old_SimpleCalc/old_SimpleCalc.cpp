
#include "old_SimpleCalc.h"
#include "Truss.h"
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
    QString vyvFile( tempFileName() + fermaResExt );
    FRMWriter frm(truss);
    frm.write( tempFileName() );
    QFile::remove( vyvFile );
    startCalculation( tempFileName() );
    VYVReader vyv(data);
    vyv.read( vyvFile );
    QFile::remove( vyvFile );
}

void SimpleCalcPlugin::createTempFile ()
{  
    tempFile = tmpnam(0);
}

void SimpleCalcPlugin::destroyTempFile () 
{ 
    QFile::remove( tempFile ); 
}

const QString& SimpleCalcPlugin::tempFileName () const
{ 
    return tempFile; 
}

/*****************************************************************************/
