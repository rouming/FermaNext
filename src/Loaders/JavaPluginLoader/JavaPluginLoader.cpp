
#include <stdlib.h>
#include <QMessageBox>
#include <QFile>
#include <QApplication>

#include "PluginLoaderFrontEnd.h"
#include "JavaPluginLoader.h"
#include "PluginManager.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin Loader hook
 *****************************************************************************/

FERMA_NEXT_PLUGIN_LOADER(JavaPluginLoader, PluginManager::NormalPriority)

/*****************************************************************************/

JavaPluginLoader::JavaPluginLoader ( PluginManager& plgMng ) :
    PluginLoader( plgMng ),
    javaVM(0),
    fnClass(0),
    fnObject(0)
{
    const char* envJavaHome = getenv("JAVA_HOME");
    QString jvmLibPath;
    QString jvmLibDir;
    QString jvmLibi386Dir;
    if ( envJavaHome == 0 ) {
        QMessageBox::warning( 
                  0, QObject::tr("Can't find JAVA_HOME environment"), 
                  QObject::tr("Please, specify JAVA_HOME environment\n"
                              "if you want to use Java plugins\n\n"
                              "Java loader has been disabled") );
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
            QMessageBox::warning( 
                     0, QObject::tr("Can't find JVM lib"),
                     QObject::tr("JAVA_HOME environment is set to \"%1\"\n").
                          arg(envJavaHomeStr) + 
                     QObject::tr("but can't find JVM lib there: \"%1\"\n\n").
                          arg(jvmLibPath) + 
                     QObject::tr("Java loader has been disabled!") );
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
    classPaths.push_back( appPath + "/plugins/loaders/fermanext.jar" );

    QString envJavaHomeStr( envJavaHome );
    QStringList options;
    options.push_back("-Djava.class.path=" + classPaths.join(pathDelim));
    options.push_back("-Djava.library.path=" + 
                      QString(appPath + "/plugins/loaders"));
    options.push_back("-verbose:jni");
    options.push_back("-Xcheck:jni");

    try { javaVM = new JavaVirtualMachine( jvmLibPath,
                                           JavaVirtualMachine::v1_4,
                                           options ); }
    catch ( ... ) {        
        QMessageBox::warning( 
                     0, QObject::tr("Can't start JVM"), 
                     QObject::tr("JVM \"%1\" can't be started!\n").
                         arg(jvmLibPath)
#if !defined _WIN32 && !defined WIN32
                     + QObject::tr("Maybe you have forgot to specify "
                                   "LD_LIBRARY_PATH?\nIt should be set "
                                   "to\n  1. %1\n  2. %2\n").
                         arg(jvmLibi386Dir).arg(jvmLibDir)
#endif
                     + QObject::tr("\nJava loader has been disabled!")
                              );        
    }

    // Try to load main 'FermaNext' java class
    fnClass = javaVM->findClass("fermanext/system/FermaNext");
    if ( fnClass == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();

        QMessageBox::warning( 
                      0, QObject::tr("Can't start JavaPluginLoader"), 
                      QObject::tr("JavaPluginLoader can't be started\n"
                                  "because subsidiary system class\n"
                                  "'fermanext.FermaNext' doesn't exist")
                              );
        delete javaVM;
        javaVM = 0;
        return;
    }

    JMethodID fnInstance =
        javaVM->getStaticMethodID( fnClass, "instance", 
                                   "()Lfermanext/system/FermaNext;" );

    if ( fnInstance == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();

        QMessageBox::warning( 0, QObject::tr("Can't start JavaPluginLoader"), 
                              QObject::tr("Method 'instance' doesn't "
                                          "exist\nin system class "
                                          "'fermanext.system.FermaNext'")
                              );
        fnClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }
    
    fnObject = javaVM->callStaticObjectMethod( fnClass, fnInstance );
    if ( fnObject == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();

        QMessageBox::warning( 0, QObject::tr("Can't start JavaPluginLoader"), 
                              QObject::tr("Error has occured while calling\n"
                                          "'instance' method of 'fermanext."
                                          "system.FermaNext'")
                              );

        fnClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }

//===========================================================================
// Remove in the future! Only for debug purposes!
//===========================================================================

    JClass trussUnitCls = javaVM->findClass("fermanext/trussunit/TrussUnit");
    if ( trussUnitCls == 0 ) {
        javaVM->exceptionDescribe();
        // Clears pending exception
        javaVM->exceptionClear();
        
        qWarning("no class!");
        return;
    }

    JMethodID trussUnitConstructor = 
        javaVM->getMethodID( trussUnitCls, "<init>", "()V" );
    if ( trussUnitConstructor == 0 ) {
        javaVM->exceptionDescribe();
        // Clears pending exception
        javaVM->exceptionClear();
        
        qWarning("no constructor!");
        return;
    }

    JObject trussUnit = 
        javaVM->newObject( trussUnitCls, trussUnitConstructor );
    if ( trussUnit == 0 ) {
        javaVM->exceptionDescribe();
        // Clears pending exception
        javaVM->exceptionClear();
        
        qWarning("can't construct object!");
        return;
    }

    JMethodID countNodes = 
        javaVM->getMethodID( trussUnitCls, "countNodes", 
                             "()I" );
    if ( countNodes == 0 ) {
        javaVM->exceptionDescribe();
        // Clears pending exception
        javaVM->exceptionClear();
        
        qWarning("can't find method!");
        return;
    }
    
    JInt nodesNum = javaVM->callIntMethod( trussUnit, countNodes  );
    QString str = QString("This is return val: %1").arg(nodesNum);
    qWarning( str.toAscii().data() );

}

JavaPluginLoader::~JavaPluginLoader ()
{
    // Calls unload method of base class
    unloadPlugins();

    // Try to unload JVM
    if ( javaVM ) {
        Q_ASSERT(fnClass);
        Q_ASSERT(fnObject);

        // Find dispose method for cleanly exit
        JMethodID disposeAllFrames = 
            javaVM->getMethodID( fnClass, "disposeAllFrames", "()V" );

        if ( disposeAllFrames )
            javaVM->callVoidMethod( fnObject, disposeAllFrames );
        else {
            // Clears pending exception
            javaVM->exceptionClear();
            qWarning( "Can't find 'disposeAllFrames' while destructing!\n"
                      "Seems 'fermanext.system.FermaNext' is not a correct"
                      "class" );
        }
        delete javaVM;
    }
}

void JavaPluginLoader::findMainAppJavaPackage ()
{
}

const QString& JavaPluginLoader::pluginExtension () const
{ static QString javaExt("jar"); return javaExt; }

JavaPluginLoader::Status JavaPluginLoader::pluginLoaderStatusCode () const
{ return javaVM ? OkStatus : InternalErrStatus; }

QString JavaPluginLoader::pluginLoaderStatusMsg () const
{ return javaVM ? QString() : "Can't load JVM!";  }

Plugin& JavaPluginLoader::specificLoadPlugin ( const QString& )
    throw (PluginLoadException)
{ return *((Plugin*)0); }

void JavaPluginLoader::specificUnloadPlugin ( Plugin& )
{}

void JavaPluginLoader::specificUnloadPlugins ()
{}

/*****************************************************************************/
