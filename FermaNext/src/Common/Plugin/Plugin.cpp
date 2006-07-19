
#include "PluginManager.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin
 *****************************************************************************/

Plugin::Plugin ( PluginManager& mng, const QString& path ) : 
    status(OkStatus), plgMng(mng), plgPath(path)
{}

Plugin::~Plugin ()
{}

Plugin::ExecutionResult Plugin::execute ( const QList<UUIDObject*>& args ) 
    /*throw (WrongExecutionArgsException)*/
{
    emit beforeExecution( *this );
    ExecutionResult result = specificExecute( args );
    emit afterExecution( *this, result );
    return result;
}

PluginManager& Plugin::pluginManager ()
{ return plgMng; }

const QString& Plugin::pluginPath () const
{ return plgPath; }

const QStringList& Plugin::requiredPluginTypes () const
{ static QStringList emptyList; return emptyList; }

Plugin::ResolvingMode Plugin::resolvingMode () const
{ return Immediately; }

bool Plugin::dependsOnPluginType ( const QString& type ) const
{ return requiredPluginTypes().contains( type ); }

void Plugin::resolveDependence () 
    /*throw (RequiredPluginIsNotResolvedException)*/
{  
    requiredPluginsMap =
        pluginManager().resolveDependence( *this, requiredPluginTypes() ); 
}

void Plugin::setStatus ( Status s )
{ status = s; }

/*****************************************************************************/
