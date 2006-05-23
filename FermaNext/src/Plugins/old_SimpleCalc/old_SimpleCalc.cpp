
#include <QFile>
#include <QRegExp>

#include "old_SimpleCalc.h"
#include "NativePluginFrontEnd.h"
#include "Truss.h"
#include "TrussCalcData.h"
#include "FRMWriter.h"
#include "VYVReader.h"

#if defined _WIN32 || defined WIN32  
  #include "win_SimpleCalc.h"
#else
  #include "unix_SimpleCalc.h"
#endif

/*****************************************************************************
 * Old Simple Calculation plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(os_dependent_SimpleCalcPlugin)

/*****************************************************************************/

SimpleCalcPlugin::SimpleCalcPlugin ( PluginManager& mng, 
                                     const QString& path ) :
    NativePlugin( mng, path )
{ 
    createTempFile(); 
}

SimpleCalcPlugin::~SimpleCalcPlugin ()
{ 
    destroyTempFile(); 
}

Plugin::Status SimpleCalcPlugin::pluginStatusCode () const
{
    return OkStatus;
}

QString SimpleCalcPlugin::pluginStatusMsg () const
{
    return QString();
}

const PluginInfo& SimpleCalcPlugin::pluginInfo () const
{ 
    static PluginInfo inf( "SimpleOldCalcPlugin",
                           "Just calculation",
                           "calculation.simple.native" );
    return inf; 
}

void SimpleCalcPlugin::execute ( const QList<UUIDObject*>& )
{
    /* //TODO
    QString frmFile( tempFileName() );    
    QString vyvFile( frmFile + fermaResExt );
    FRMWriter frm(truss);
    frm.write( frmFile );
    QFile::remove( vyvFile );
    startCalculation( frmFile );
    VYVReader vyv(data);
    vyv.read( vyvFile );
    QFile::remove( vyvFile );
    */
}

void SimpleCalcPlugin::createTempFile ()
{  
    tempFile = tmpnam(0);    
    // Remove possible trailing extension
    tempFile.remove( QRegExp("\\..*$") );
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
