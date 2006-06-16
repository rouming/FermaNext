
#include <stdlib.h>
#include <QMessageBox>
#include <QFile>
#include <QApplication>

#include "PluginLoaderFrontEnd.h"
#include "JavaPluginLoader.h"
#include "JavaPlugin.h"
#include "PluginManager.h"
#include "Plugin.h"
#include "Config.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Plugin Loader hook
 *****************************************************************************/

FERMA_NEXT_PLUGIN_LOADER(JavaPluginLoader, PluginManager::NormalPriority)

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("loader.JavaPluginLoader") );

/*****************************************************************************/

static QString javaErrorMsg( "<<< Java exception caught >>>\n%1\n"
                             "<<< Java exception >>>" );

/*****************************************************************************/

JavaPluginLoader::JavaPluginLoader ( PluginManager& plgMng, 
                                     const QString& path ) :
    PluginLoader( plgMng, path ),
    javaVM(0),
    fnClass(0),
    fnObject(0),
    plgLoaderObject(0)
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

    // Varibles for options
    QStringList options;
    QStringList classPaths;
    QStringList libPaths;

    // Read config
    Config& cfg = pluginManager().config();
    ConfigNode rootNode = cfg.rootNode();
    ConfigNodeList nodes = rootNode.findChildNodes( "JavaPluginLoader" );
    if ( nodes.size() != 0 ) {
        ConfigNode javaLoaderNode = nodes.at(0);

        // Options setting
        ConfigNodeList jvmOptions = 
            javaLoaderNode.findChildNodes( "JVMOption" );

        foreach ( ConfigNode jvmOption, jvmOptions ) {
            NodeAttributeList attrs = jvmOption.getAttributes();
            if ( attrs.size() == 0 )
                continue;
            NodeAttribute attr = attrs.at(0);
            if ( attr.first != "option" )
                continue;
            options.push_back( attr.second );
        }

        // Class path setting
        ConfigNodeList jvmClassPaths = 
            javaLoaderNode.findChildNodes( "JVMClassPath" );

        foreach ( ConfigNode jvmClassPath, jvmClassPaths ) {
            NodeAttributeList attrs = jvmClassPath.getAttributes();
            if ( attrs.size() == 0 )
                continue;
            NodeAttribute attr = attrs.at(0);
            if ( attr.first != "path" )
                continue;
            QString path = attr.second;
            // Replace special word '%APPDIR%'
            path.replace("%APPDIR%", appPath);
            // Append to paths
            classPaths.push_back( path );
        }

        // Class path setting
        ConfigNodeList jvmLibPaths = 
            javaLoaderNode.findChildNodes( "JVMLibraryPath" );

        foreach ( ConfigNode jvmLibPath, jvmLibPaths ) {
            NodeAttributeList attrs = jvmLibPath.getAttributes();
            if ( attrs.size() == 0 )
                continue;
            NodeAttribute attr = attrs.at(0);
            if ( attr.first != "path" )
                continue;
            QString path = attr.second;
            // Replace special word '%APPDIR%'
            path.replace("%APPDIR%", appPath);
            // Append to paths
            libPaths.push_back( path );
        }
    }

    // Set options
    options.push_back("-Djava.class.path=" + classPaths.join(pathDelim));
    options.push_back("-Djava.library.path=" + libPaths.join(pathDelim));

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
                                  "'fermanext.system.FermaNext' doesn't exist")
                              );
        delete javaVM;
        javaVM = 0;
        return;
    }

    // Create global Java ref
    fnClass = (JClass)javaVM->newGlobalRef( fnClass );
    if ( fnClass == 0 ) {
        // Clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");

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
        javaVM->deleteGlobalRef( fnClass );
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
                                          "fermanext.system.FermaNext'")
                              );
        javaVM->deleteGlobalRef( fnClass );
        fnClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }

    // Create global Java ref
    fnObject = javaVM->newGlobalRef( fnObject );
    if ( fnObject == 0 ) {
        // Clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");

        javaVM->deleteGlobalRef( fnClass );
        fnClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }
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
            QString msg( "Can't find 'disposeAllFrames' while destructing!\n"
                         "Seems 'fermanext.system.FermaNext' is not a valid"
                         "class\n" );
            QString e = javaVM->getAndClearPendingException();            
            msg = msg + javaErrorMsg.arg(e);
            LOG4CXX_ERROR(logger, msg.toStdString());
        }
        javaVM->deleteGlobalRef( fnClass );
        javaVM->deleteGlobalRef( fnObject );
        if ( plgLoaderObject )
            javaVM->deleteGlobalRef( plgLoaderObject );
        delete javaVM;
    }
}

const QString& JavaPluginLoader::pluginExtension () const
{ static QString javaExt("jar"); return javaExt; }

JavaPluginLoader::Status JavaPluginLoader::pluginLoaderStatusCode () const
{ return javaVM ? OkStatus : InternalErrStatus; }

QString JavaPluginLoader::pluginLoaderStatusMsg () const
{ return javaVM ? QString() : "Can't load JVM!";  }

Plugin& JavaPluginLoader::specificLoadPlugin ( const QString& pluginPath )
    /*throw (PluginLoadException)*/
{
    LOG4CXX_DEBUG(logger, "specificLoadPlugin(QString): " + 
                  pluginPath.toStdString() );

    // Suspended init
    if ( plgLoaderObject == 0 ) {
        JMethodID plgLoaderMethod = 
            javaVM->getMethodID( fnClass, "pluginLoader", 
                                 "()Lfermanext/system/JavaPluginLoader;" );
        if ( plgLoaderMethod == 0 ) {
            // Returns and clears pending exception
            QString e = javaVM->getAndClearPendingException();
            LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
            throw PluginLoadException();
        }
        plgLoaderObject = javaVM->callObjectMethod( fnObject, plgLoaderMethod );
        if ( plgLoaderObject == 0 ) {
            // Returns and clears pending exception
            QString e = javaVM->getAndClearPendingException();
            LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
            throw PluginLoadException();
        }        
        // Create global Java ref
        plgLoaderObject = javaVM->newGlobalRef( plgLoaderObject );
        if ( plgLoaderObject == 0 ) {
            // Clears pending exception
            javaVM->getAndClearPendingException();
            LOG4CXX_ERROR(logger, "out of memory");
            throw PluginLoadException();
        }
    }

    JClass loaderClass = javaVM->getObjectClass( plgLoaderObject );
    if ( loaderClass == 0 ) {
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        throw PluginLoadException();
    }
    loaderClass = (JClass)javaVM->newGlobalRef( loaderClass );
    if ( loaderClass == 0 ) {
        // Returns and clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");
        throw PluginLoadException();
    }
    
    JMethodID loadPluginMethod = javaVM->getMethodID( 
                   loaderClass,
                   "loadPlugin",
                   "(Ljava/lang/String;)Lfermanext/system/JavaPlugin;" );
    if ( loadPluginMethod == 0 ) {
        javaVM->deleteGlobalRef( loaderClass );
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        throw PluginLoadException();
    }

    // Create Java string
    JString pluginPathJStr = javaVM->newStringUTF( pluginPath.toUtf8().data() );
    if ( pluginPathJStr == 0 ) {
        javaVM->deleteGlobalRef( loaderClass );
        // Returns and clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "Can't create Java String");
        throw PluginLoadException();
    }
    
    pluginPathJStr = (JString)javaVM->newGlobalRef( pluginPathJStr );
    if ( pluginPathJStr == 0 ) {
        javaVM->deleteGlobalRef( loaderClass );
        // Returns and clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");
        throw PluginLoadException();
    }

    JObject javaPlgInst = javaVM->callObjectMethod( plgLoaderObject, 
                                                    loadPluginMethod, 
                                                    pluginPathJStr );

    javaVM->deleteGlobalRef( loaderClass );
    javaVM->deleteGlobalRef( pluginPathJStr );

    if ( javaPlgInst == 0 ) {
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        throw PluginLoadException();
    }

    return *new JavaPlugin( *javaVM, javaPlgInst, pluginManager(), pluginPath );
}

void JavaPluginLoader::specificUnloadPlugin ( Plugin& plugin )
{
    LOG4CXX_DEBUG(logger, "specificUnloadPlugin: " + 
                  plugin.pluginPath().toStdString() );

    // Nothing was loaded
    if ( plgLoaderObject == 0 )
        return;

    JavaPlugin* javaPlugin = dynamic_cast<JavaPlugin*>( &plugin );
    // Check casting
    if ( javaPlugin == 0 )
        return;

    JClass loaderClass = javaVM->getObjectClass( plgLoaderObject );
    if ( loaderClass == 0 ) {
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        return;
    }
    loaderClass = (JClass)javaVM->newGlobalRef( loaderClass );
    if ( loaderClass == 0 ) {
        // Returns and clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");
        return;
    }

    JMethodID unloadPluginMethod = javaVM->getMethodID( 
                                           loaderClass,
                                           "unloadPlugin",
                                           "(Lfermanext/system/JavaPlugin;)V" );
    if ( unloadPluginMethod == 0 ) {
        javaVM->deleteGlobalRef( loaderClass );
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        return;
    }

    javaVM->callVoidMethod( plgLoaderObject, unloadPluginMethod,
                            javaPlugin->javaPluginInstance() );

    javaVM->deleteGlobalRef( loaderClass );

    if ( javaVM->exceptionCheck() ) {
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        return;
    }                            
}

void JavaPluginLoader::specificUnloadPlugins ()
{
    LOG4CXX_DEBUG(logger, "specificUnloadPlugins" );

    // Nothing was loaded
    if ( plgLoaderObject == 0 )
        return;


    JClass loaderClass = javaVM->getObjectClass( plgLoaderObject );
    if ( loaderClass == 0 ) {
        // Returns and clears pending exception
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        return;
    }

    JMethodID unloadPluginsMethod = javaVM->getMethodID( loaderClass,
                                                        "unloadPlugins",
                                                        "()V" );
    if ( unloadPluginsMethod == 0 ) {
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
        return;
    }

    // Call 
    javaVM->callVoidMethod( plgLoaderObject, unloadPluginsMethod );

    if ( javaVM->exceptionCheck() ) {
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
    }
}

/*****************************************************************************/
