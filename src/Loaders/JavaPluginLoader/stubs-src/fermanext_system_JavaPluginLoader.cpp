
#include "include/fermanext_system_JavaPluginLoader.h"

#include "JavaPluginLoader.h"
#include "PluginManager.h"
#include "JavaVariableRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.system.native_JavaPluginLoader") );

/*****************************************************************************/

jobject JNICALL Java_fermanext_system_JavaPluginLoader_instance
  (JNIEnv* env, jclass jClass, jstring jUuid)
{
    LOG4CXX_DEBUG( logger, "instance" );
    
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );
    Q_ASSERT( uuidChars );

    QString uuid( uuidChars );

    LOG4CXX_INFO( logger, QString("JavaPluginLoader uuid is '%1'").
                            arg(uuid).toStdString() );

    env->ReleaseStringUTFChars( jUuid, uuidChars );

    JavaPluginLoader* loader = ::getRegisteredAndCast<JavaPluginLoader>(uuid);

    if ( loader == 0 ) {
        LOG4CXX_ERROR( logger, "Loader is not registered!" );
        return 0;    
    }

    PluginManager& plgManager = loader->pluginManager();
    const QString& loaderPath = loader->pluginLoaderPath();

    // Register plugin manager and create java wrapper
    jobject jPlgMng = JavaVariableRegistrator::registerVariable( &plgManager, 
                                      env, "fermanext/system/PluginManager" );

    Q_ASSERT(jPlgMng);

    // Create java JavaPluginLoader wrapper
    jmethodID ctor = env->GetMethodID( jClass, "<init>", 
                     "(Lfermanext/system/PluginManager;Ljava/lang/String;)V" );
    Q_ASSERT( ctor );

    // Create Java string: loader path
    jstring jLoaderPath = env->NewStringUTF( loaderPath.toUtf8().data() );
    Q_ASSERT(jLoaderPath);
       
    jobject jObj = env->NewObject( jClass, ctor, jPlgMng, jLoaderPath );
    Q_ASSERT( jObj );

    return jObj;
}

/*****************************************************************************/
