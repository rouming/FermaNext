
#include "JavaPlugin.h"

/*****************************************************************************
 * Java Plugin
 *****************************************************************************/

JavaPlugin::JavaPlugin ( PluginManager& mng, const QString& path ) :
    Plugin( mng, path )
{
    // TODO
}

JavaPlugin::~JavaPlugin ()
{}

const PluginInfo& JavaPlugin::pluginInfo () const
{
    // TODO
    static PluginInfo inf( "TODO",
                           "TODO",
                           "TODO" );
    return inf;
}


JavaPlugin::Status JavaPlugin::pluginStatusCode () const
{
    // TODO
    return OkStatus; 
}

QString JavaPlugin::pluginStatusMsg () const
{ 
    // TODO
    return "";
}

const QStringList& JavaPlugin::requiredPluginTypes () const
{ 
    // TODO
    static QStringList list; return list; 
}

JavaPlugin::ResolvingMode JavaPlugin::resolvingMode () const
{  
    // TODO
    return Immediately;
}

void JavaPlugin::startWizardSetup ()
{
    // TODO
}

/*****************************************************************************/
