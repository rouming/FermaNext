
#include "include/fermanext_system_PluginManager.h"

#include "PluginManager.h"
#include "JavaVariableRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.system.native_PluginManager") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_system_PluginManager_isValid
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "isValid" );

    jclass instCls = env->GetObjectClass( self );
    Q_ASSERT( instCls );

    jmethodID getUUIDMethod = env->GetMethodID( instCls, "getUUID",
                                                "()Ljava/lang/String;" );
    Q_ASSERT( getUUIDMethod );

    jstring jUuid = (jstring)env->CallObjectMethod( self, getUUIDMethod );
    Q_ASSERT( jUuid );

     // Get chars
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );

    QString uuid = QString::fromUtf8( uuidChars );

    // Free chars
    env->ReleaseStringUTFChars( jUuid, uuidChars );
    
    PluginManager* plgMng = ::getRegisteredAndCast<PluginManager>( uuid );

    return (plgMng == 0 ? false : true);
}

/*****************************************************************************/
