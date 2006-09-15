
#include <QFile>
#include <QRegExp>
#include <QMessageBox>

#include "old_SimpleCalc.h"
#include "NativePluginFrontEnd.h"
#include "TrussUnit.h"
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
    const QList<UUIDObject*>& argsList )
    /*throw (WrongExecutionArgsException)*/
{
    if ( argsList.size() != 1 )
        throw WrongExecutionArgsException();
    
    TrussUnit* truss = dynamic_cast<TrussUnit*>(argsList[0]);
    if ( truss == 0 )
        throw WrongExecutionArgsException();
        
    QString frmFile( tempFileName() );    
    QString vyvFile( frmFile + fermaResExt );

    // To be sure
    QFile::remove( frmFile );
    QFile::remove( vyvFile );

    FRMWriter frm(truss);
    frm.write( frmFile );
    startCalculation( frmFile );
    VYVReader vyv;
    vyv.read( vyvFile );

    // Last clean
    QFile::remove( frmFile );
    QFile::remove( vyvFile );

    // FIXME: add real status
    return ExecutionResult( OkStatus, vyv.toXMLString() );
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
