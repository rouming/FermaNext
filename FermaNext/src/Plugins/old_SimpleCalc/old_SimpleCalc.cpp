
#include <QFile>
#include <QRegExp>
#include <QMessageBox>

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

Plugin::ExecutionResult SimpleCalcPlugin::specificExecute ( 
    const QList<UUIDObject*>& )
    /*throw (WrongExecutionArgsException)*/
{
    QMessageBox::information( 0, "Qt4 native driver", 
                              "This is a native driver.\n"
                              "Will be implemented in the near future.",
                              0 );
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

    // FIXME: add real results
    return ExecutionResult( OkStatus, "Nothing for now!" );
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
