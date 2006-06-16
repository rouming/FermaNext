
#include "include/fermanext_trussunit_TrussUnit.h"

#include "TrussUnit.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussUnit") );

/*****************************************************************************/

TrussUnit* getTrussUnitByUUID ( JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "getTrussUnitByUUID" );

    jclass trussInstCls = env->GetObjectClass( self );
    Q_ASSERT( trussInstCls );

    // Create reference
    trussInstCls = (jclass)env->NewGlobalRef( trussInstCls );
    Q_ASSERT( trussInstCls );

    jmethodID trussUUIDMethod = env->GetMethodID( trussInstCls, "getUUID",
                                                  "()Ljava/lang/String;" );
    Q_ASSERT( trussUUIDMethod );

    jstring jUuid = (jstring)env->CallObjectMethod( self, trussUUIDMethod );
    Q_ASSERT( jUuid );

     // Get chars
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );

    QString uuid = QString::fromUtf8( uuidChars );

    // Free chars
    env->ReleaseStringUTFChars( jUuid, uuidChars );
    
    // Clear all references
    env->DeleteGlobalRef( trussInstCls );

    UUIDObject* truss = JavaPluginArgumentRegistrator::getRegistered( uuid );
    // Try to cast
    return dynamic_cast<TrussUnit*>(truss);
}

/*****************************************************************************/

jint JNICALL Java_fermanext_trussunit_TrussUnit_countNodes
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countNodes" );
    TrussUnit* truss = getTrussUnitByUUID( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 666;
    }
    return truss->countNodes();
}

jboolean JNICALL Java_fermanext_trussunit_TrussUnit_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussUnit* truss = getTrussUnitByUUID( env, self );
    return (truss == 0 ? false : true);
}

/*****************************************************************************/
