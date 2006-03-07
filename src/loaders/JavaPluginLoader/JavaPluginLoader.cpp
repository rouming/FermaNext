
#include <stdlib.h>
#include <QMessagebox>
#include <QFile>

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
    PluginLoader( plgMng ),
    javaVM(0)
{
    const char* envJavaHome = getenv("JAVA_HOME");
    QString jvmLibPath;
    QString jvmLibDir;
    QString jvmLibi386Dir;
    if ( envJavaHome == 0 ) {
        QMessageBox::warning( 0, "Can't find JAVA_HOME environment", 
                              "Please, specify JAVA_HOME environment\n"
                              "if you want to use Java plugins\n\n"
                              "Java loader has been disabled");
        return;
    } else {
        const QString envJavaHomeStr( envJavaHome );
#if defined _WIN32 || defined WIN32
        jvmLibi386Dir = envJavaHomeStr + "/jre/bin/client";
        jvmLibDir     = jvmLibi386Dir;
        jvmLibPath    = jvmLibDir + "/jvm.dll";
#else
        jvmLibi386Dir = envJavaHomeStr + "/jre/lib/i386";
        jvmLibDir     = jvmLibi386Dir + "/client";
        jvmLibPath    = jvmLibDir + "/libjvm.so";
#endif
        if ( ! QFile::exists( jvmLibPath ) ) {
            QMessageBox::warning( 0, "Can't find JVM lib",
                           QString("JAVA_HOME environment is set to \"%1\"\n").
                                  arg(envJavaHomeStr) + 
                           QString("but can't find JVM lib there: \"%1\"\n\n").
                                  arg(jvmLibPath) + 
                           "Java loader has been disabled!" );
            return;
        }
    }

    QString envJavaHomeStr( envJavaHome );
    QStringList options;
    options.push_back("-verbose:jni");

    try { javaVM = new JavaVirtualMachine( jvmLibPath,
                                           JavaVirtualMachine::v1_4,
                                           options ); } 
    catch ( ... ) {        
        QMessageBox::warning( 0, "Can't start JVM", 
                              QString("JVM \"%1\" can't be started!\n").
                                  arg(jvmLibPath)
#if !defined _WIN32 && !defined WIN32
                              + QString("Maybe you have forgot to specify "
                                        "LD_LIBRARY_PATH?\nIt should be set "
                                        "to\n  1. %1\n  2. %2\n").
                                    arg(jvmLibi386Dir).arg(jvmLibDir)
#endif
                              + "\nJava loader has been disabled!"
                              );        
    }
}

JavaPluginLoader::~JavaPluginLoader ()
{ delete javaVM; }

const QString& JavaPluginLoader::pluginExtension () const
{ static QString javaExt("jar"); return javaExt; }

JavaPluginLoader::Status JavaPluginLoader::pluginLoaderStatusCode () const
{ return OkStatus; }

QString JavaPluginLoader::pluginLoaderStatusMsg () const
{ return QString(); }

Plugin& JavaPluginLoader::specificLoadPlugin ( const QString& )
    throw (PluginLoadException)
{ return *((Plugin*)0); }

void JavaPluginLoader::specificUnloadPlugin ( Plugin& )
{}

void JavaPluginLoader::specificUnloadPlugins ()
{}

/*****************************************************************************/
