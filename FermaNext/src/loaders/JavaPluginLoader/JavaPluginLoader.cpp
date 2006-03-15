
#include "PluginLoaderFrontEnd.h"
#include "JavaPluginLoader.h"
#include "PluginManager.h"
#include "Plugin.h"

#include "JavaVM/JavaVirtualMachine.h"

#include <qmessagebox.h>
#include <qfile.h>
#include <stdlib.h>
#include <qapplication.h>

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

    
    QString appPath(".");
    if ( qApp) 
        appPath = qApp->applicationDirPath();

#if defined _WIN32 || defined WIN32
    QString pathDelim = ";";
#else
    QString pathDelim = ":";
#endif

    QStringList classPaths;
    classPaths.push_back( appPath );
    classPaths.push_back( appPath + "/fermanext.jar" );
    classPaths.push_back( appPath + "/plugins/loaders" );    

    QString envJavaHomeStr( envJavaHome );
    QStringList options;
    options.push_back("-Djava.class.path=" + classPaths.join(pathDelim));
    options.push_back("-verbose:jni");
    options.push_back("-Xcheck:jni");

    try { javaVM = new JavaVirtualMachine( jvmLibPath,
                                           JavaVirtualMachine::v1_4,
                                           options ); 
        JClass progCls = javaVM->findClass("Prog");
        if ( progCls == 0 ) {
            qWarning( "Can't find Prog class" );
            return;
        }

        JMethodID progInit = javaVM->getMethodID( progCls, "<init>", "(I)V" );
        if ( progInit == 0 ) {
            qWarning( "Can't find Prog.<init>" );
            return;
        }

        JValue param;
        param.i = 666;
        JObject progObj = javaVM->newObjectA( progCls, progInit, &param );
        if ( progObj == 0 ) {
            qWarning( "Can't construct prog obj" );
            return;
        }          
        
    }
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
{
    // Dispose all frames for clean exit
    JClass fnCls = javaVM->findClass("fermanext/FermaNext");
    if ( fnCls ) {
        JMethodID disposeAllFrames = 
            javaVM->getStaticMethodID( fnCls, "disposeAllFrames", "()V" );

        if ( disposeAllFrames )
            javaVM->callStaticVoidMethod( fnCls, disposeAllFrames );
    }
    delete javaVM;
}

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
