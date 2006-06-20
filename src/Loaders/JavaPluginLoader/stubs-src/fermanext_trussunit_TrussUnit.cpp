
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

jboolean JNICALL Java_fermanext_trussunit_TrussUnit_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussUnit* truss = getTrussUnitByUUID( env, self );
    return (truss == 0 ? false : true);
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
        return -1;
    }
    return truss->countNodes();
}

jint JNICALL Java_fermanext_trussunit_TrussUnit_countPivots
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countPivots" );
    TrussUnit* truss = getTrussUnitByUUID( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return truss->countPivots();
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    createNode
 * Signature: (DD)Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_createNode__DD
  (JNIEnv* env, jobject self, jdouble x, jdouble y)
{
    LOG4CXX_DEBUG( logger, "createNode(double, double)" );
    TrussUnit* truss = getTrussUnitByUUID( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode& node = truss->createNode( x, y );
    // Register argument
    JavaPluginArgumentRegistrator::registerArgument( &node );

    jclass nodeClass = env->FindClass("fermanext/trussunit/TrussNode");
    Q_ASSERT( nodeClass );
    // Create reference
    nodeClass = (jclass)env->NewGlobalRef( nodeClass );
    Q_ASSERT( nodeClass );

    jmethodID nodeCtor = env->GetMethodID( nodeClass, "<init>", "()V" );
    Q_ASSERT( nodeCtor );

    jobject jNode = env->NewObject( nodeClass, nodeCtor );
    Q_ASSERT( jNode );
    // Create reference
    jNode = env->NewGlobalRef( jNode );
    Q_ASSERT( jNode );

    jmethodID setUUIDMethod = env->GetMethodID( nodeClass, "setUUID", 
                                                "(Ljava/lang/String;)V" );
    Q_ASSERT( setUUIDMethod );

    jstring jStrUUID = env->NewStringUTF( qPrintable(node.getUUID()) );
    env->CallVoidMethod( jNode, setUUIDMethod, jStrUUID );

    // Clear all references
    env->DeleteGlobalRef( nodeClass );

    env->NewLocalRef( jNode );

    return jNode;
}

/*****************************************************************************/
