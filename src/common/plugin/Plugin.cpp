
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

PluginManager& Plugin::pluginManager ()
{ return plgMng; }

const QString& Plugin::pluginPath () const
{ return plgPath; }

const QStringList& Plugin::requiredPluginTypes () const
{ static QStringList emptyList; return emptyList; }

Plugin::ResolvingMode Plugin::resolvingMode () const
{ return Immediately; }

bool Plugin::dependsOnPluginType ( const QString& type ) const
{ return (requiredPluginTypes().contains( type ) > 0); }

void Plugin::resolveDependence () throw (RequiredPluginIsNotResolvedException)
{  
    requiredPluginsMap =
        pluginManager().resolveDependence( *this, requiredPluginTypes() ); 
}

void Plugin::startWizardSetup ()
{}

void Plugin::setStatus ( Status s )
{ status = s; }

/*****************************************************************************/
