
#include <stdlib.h>
#include <QMessageBox>
#include <QFile>
#include <QApplication>

#include "PluginLoaderFrontEnd.h"
#include "JavaPluginLoader.h"
#include "JavaVariableRegistrator.h"
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
    jloaderClass(0),
    jloaderObject(0)
{
    // Subsidiary stuff
    QString appPath(".");
    if ( qApp) 
        appPath = qApp->applicationDirPath();

#if defined _WIN32 || defined WIN32
    QString pathDelim = ";";
#else
    QString pathDelim = ":";
#endif

    // Main loader config node
    ConfigNode javaLoaderCfgNode;

    // Read config
    Config& cfg = pluginManager().config();
    ConfigNode rootNode = cfg.rootNode();
    ConfigNodeList nodes = rootNode.findChildNodes( "JavaPluginLoader" );
    if ( nodes.size() != 0 ) {
        javaLoaderCfgNode = nodes.at(0);
    } else {
        QMessageBox::warning( 
                  0, QObject::tr("Config is wrong"), 
                  QObject::tr("Please, use original Java loader configuration"
                              "\n\nJava loader has been disabled") );
        return;
    }

    QString jrePath;
    QString jvmLibPath;
    QString jvmLibDir;
    QString jvmLibi386Dir;

    // Try to find JRE path in JAVA_HOME env
    if ( jrePath.isEmpty() ) {
        const char* envJavaHome = getenv("JAVA_HOME");
        if ( envJavaHome != 0 ) {
            jrePath = QString(envJavaHome) + "/jre";
        }
    }
    // Try to find JRE path in JRE_HOME env
    if ( jrePath.isEmpty() ) {
        jrePath = getenv("JRE_HOME");
    }
    // Try to find JRE path in config
    if ( jrePath.isEmpty() ) {
        ConfigNodeList jrePaths = 
            javaLoaderCfgNode.findChildNodes( "JREPath" );
        foreach ( ConfigNode path, jrePaths ) {
            NodeAttributeList attrs = path.getAttributes();
            if ( attrs.size() == 0 )
                continue;
            NodeAttribute attr = attrs.at(0);
            if ( attr.first != "path" )
                continue;
            jrePath = attr.second;
            // Replace special word '%APPDIR%'
            jrePath.replace("%APPDIR%", appPath);
            break;
        }
    }

    // Check JRE path existence
    if ( jrePath.isEmpty() ) {
        QMessageBox::warning( 
                  0, QObject::tr("JRE path is empty"), 
                  QObject::tr("Please, specify JAVA_HOME environment\n"
                              "JRE_HOME environment or "
                              "<JREPath path=\"path_to_jre\"> in your "
                              "config\nif you want to use Java plugins\n\n"
                              "Java loader has been disabled") );
        return;
    } 

#if defined _WIN32 || defined WIN32
    jvmLibi386Dir = jrePath + "/bin/client";
    jvmLibDir     = jvmLibi386Dir;
    jvmLibPath    = jvmLibDir + "/jvm.dll";
#else
    jvmLibi386Dir = jrePath + "/lib/i386";
    jvmLibDir     = jvmLibi386Dir + "/client";
    jvmLibPath    = jvmLibDir + "/libjvm.so";
#endif
    if ( ! QFile::exists( jvmLibPath ) ) {
        QMessageBox::warning( 
                     0, QObject::tr("Can't find JVM lib"),
                     QObject::tr("JRE path is set to \"%1\"\n").
                          arg(jrePath) + 
                     QObject::tr("but can't find JVM lib there: \"%1\"\n\n").
                          arg(jvmLibPath) + 
                     QObject::tr("Java loader has been disabled!") );
        return;
    }
    
    // Varibles for options
    QStringList options;
    QStringList classPaths;
    QStringList libPaths;

    // Read java config node
    if ( ! javaLoaderCfgNode.isNull() ) {
        // Options setting
        ConfigNodeList jvmOptions = 
            javaLoaderCfgNode.findChildNodes( "JVMOption" );

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
            javaLoaderCfgNode.findChildNodes( "JVMClassPath" );

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
            javaLoaderCfgNode.findChildNodes( "JVMLibraryPath" );

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

    // Great, JavaVM init is complete. 
    // Register loader to have access to it from jni side.
    JavaVariableRegistrator::registerVariable( this );

    // Try to load main 'JavaPluginLoader' java class
    jloaderClass = javaVM->findClass("fermanext/system/JavaPluginLoader");
    if ( jloaderClass == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();

        QMessageBox::warning( 
                      0, QObject::tr("Wrong JavaPluginLoader package"), 
                      QObject::tr("Subsidiary system class\n"
                                  "'fermanext.system.JavaPluginLoader' "
                                  "doesn't exist"
                                   "\n\nJava loader has been disabled!")
                      );
        delete javaVM;
        javaVM = 0;
        return;
    }

    // Create global Java ref
    jloaderClass = (JClass)javaVM->newGlobalRef( jloaderClass );
    if ( jloaderClass == 0 ) {
        // Clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");

        delete javaVM;
        javaVM = 0;
        return;        
    }

    JMethodID jloaderInstance = javaVM->getStaticMethodID( 
                   jloaderClass, "instance", 
                   "(Ljava/lang/String;)Lfermanext/system/JavaPluginLoader;" );

    if ( jloaderInstance == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();

        QMessageBox::warning( 0, QObject::tr("Wrong JavaPluginLoader package"),
                              QObject::tr("Method 'instance' doesn't "
                                          "exist in \nsystem class "
                                          "'fermanext.system.JavaPluginLoader'"
                                          "\n\nJava loader has been disabled!")
                              );
        javaVM->deleteGlobalRef( jloaderClass );
        jloaderClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }

    // Create Java string
    JString jUuid = javaVM->newStringUTF( getUUID().toUtf8().data() );
    if ( jUuid == 0 ) {
        // Clears pending exception
        javaVM->exceptionClear();
        
        QMessageBox::warning( 0, QObject::tr("Wrong JavaPluginLoader package"),
                              QObject::tr("Error has occured while creating\n"
                                          "java string.") );

        javaVM->deleteGlobalRef( jloaderClass );
        jloaderClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }
    
    jloaderObject = javaVM->callStaticObjectMethod( jloaderClass, 
                                                    jloaderInstance,
                                                    jUuid );
    if ( jloaderObject == 0 ) {
        QString e = javaVM->getAndClearPendingException();            
        QString errMsg = "Java exception while  calling method 'instance' of "
                         "'fermanext.system.JavaPluginLoader': " + 
                         javaErrorMsg.arg(e) + "\n";
        LOG4CXX_ERROR(logger, errMsg.toStdString());

        QMessageBox::warning( 0, QObject::tr("Wrong JavaPluginLoader package"),
                              QObject::tr("Error has occured while calling\n"
                                          "method 'instance' of "
                                          "'fermanext.system.JavaPluginLoader'"
                                          "\n\nJava loader has been disabled!")
                              );
        javaVM->deleteGlobalRef( jloaderClass );
        jloaderClass = 0;
        delete javaVM;
        javaVM = 0;
        return;
    }

    // Create global Java ref
    jloaderObject = javaVM->newGlobalRef( jloaderObject );
    if ( jloaderObject == 0 ) {
        // Clears pending exception
        javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, "out of memory");

        javaVM->deleteGlobalRef( jloaderClass );
        jloaderClass = 0;
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
        Q_ASSERT(jloaderClass);
        Q_ASSERT(jloaderObject);

        // Find dispose method for cleanly exit
        JMethodID disposeAllFrames = 
            javaVM->getMethodID( jloaderClass, "disposeAllFrames", "()V" );

        if ( disposeAllFrames )
            javaVM->callVoidMethod( jloaderObject, disposeAllFrames );
        else {
            QString msg( "Can't find 'disposeAllFrames' while destructing!\n"
                         "Seems 'fermanext.system.FermaNext' is not a valid"
                         "class\n" );
            QString e = javaVM->getAndClearPendingException();            
            msg = msg + javaErrorMsg.arg(e);
            LOG4CXX_ERROR(logger, msg.toStdString());
        }
        javaVM->deleteGlobalRef( jloaderObject );
        javaVM->deleteGlobalRef( jloaderClass );
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

    JClass loaderClass = javaVM->getObjectClass( jloaderObject );
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
    JString pluginPathJStr = javaVM->newStringUTF( pluginPath.toUtf8().data());
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

    JObject javaPlgInst = javaVM->callObjectMethod( jloaderObject, 
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

    return *new JavaPlugin( *javaVM, javaPlgInst, pluginManager(), pluginPath);
}

void JavaPluginLoader::specificUnloadPlugin ( Plugin& plugin )
{
    LOG4CXX_DEBUG(logger, "specificUnloadPlugin: " + 
                  plugin.pluginPath().toStdString() );

    JavaPlugin* javaPlugin = dynamic_cast<JavaPlugin*>( &plugin );
    // Check casting
    if ( javaPlugin == 0 )
        return;

    JClass loaderClass = javaVM->getObjectClass( jloaderObject );
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

    javaVM->callVoidMethod( jloaderObject, unloadPluginMethod,
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

    JClass loaderClass = javaVM->getObjectClass( jloaderObject );
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
    javaVM->callVoidMethod( jloaderObject, unloadPluginsMethod );

    if ( javaVM->exceptionCheck() ) {
        QString e = javaVM->getAndClearPendingException();
        LOG4CXX_ERROR(logger, javaErrorMsg.arg(e).toStdString());
    }
}

/*****************************************************************************/
