
#include "PluginLoaderFrontEnd.h"
#include "JavaPluginLoader.h"
#include "PluginManager.h"
#include "Plugin.h"

#include "JavaVM/JavaVirtualMachine.h"

/*****************************************************************************
 * Plugin Loader hook
 *****************************************************************************/

FERMA_NEXT_PLUGIN_LOADER(JavaPluginLoader, PluginManager::NormalPriority)

/*****************************************************************************/

JavaPluginLoader::JavaPluginLoader ( PluginManager& plgMng ) :
    PluginLoader( plgMng )
{
    QStringList options;
    options.push_back("-verbose:jni");
    javaVM = new JavaVirtualMachine(
            //  "/home/roman/soft/j2sdk1.4.2_10/jre/lib/i386/client/libjvm.so",
              "e:/java1.4.1/jre/bin/client/jvm.dll",
               JavaVirtualMachine::v1_4,
               options );
}

JavaPluginLoader::~JavaPluginLoader ()
{
    // BUG: wrong destruction of a dynamic lib
    //delete javaVM;
}

const QString& JavaPluginLoader::pluginExtension () const
{ static QString javaExt("jar"); return javaExt; }

JavaPluginLoader::Status JavaPluginLoader::pluginLoaderStatusCode () const
{ return OkStatus; }

QString JavaPluginLoader::pluginLoaderStatusMsg () const
{ return QString(); }

Plugin& JavaPluginLoader::specificLoadPlugin ( const QString& pathToPlugin )
    throw (PluginLoadException)
{ return *((Plugin*)0); }

void JavaPluginLoader::specificUnloadPlugin ( Plugin& plugin )
{}

void JavaPluginLoader::specificUnloadPlugins ()
{}

/*****************************************************************************/
