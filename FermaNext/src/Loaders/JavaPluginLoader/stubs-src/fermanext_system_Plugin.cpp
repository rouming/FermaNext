
#include "include/fermanext_system_Plugin.h"

#include "Plugin.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.system.native_Plugin") );

/*****************************************************************************/

/*
 * Class:     fermanext_system_Plugin
 * Method:    execute
 * Signature: ([Lfermanext/system/UUIDObject;)Lfermanext/system/Plugin$ExecutionResult;
 */
jobject JNICALL Java_fermanext_system_Plugin_execute
  (JNIEnv* env, jobject self, jobjectArray jUuidObjArr )
{
    LOG4CXX_DEBUG( logger, "execute" );

    QList<UUIDObject*> args;

    if ( jUuidObjArr ) {
        jsize jArrSize = env->GetArrayLength( jUuidObjArr );
        for ( jsize i = 0; i < jArrSize; ++i ) {
            jobject jObj = env->GetObjectArrayElement( jUuidObjArr, i );
            Q_ASSERT(jObj);

            UUIDObject* obj = JavaPluginArgumentRegistrator::
                getRegisteredByJavaObj( env, jObj );
            Q_ASSERT(obj);

            // Append plugin to args list
            args.append(obj);
        }
    }

    Plugin* thisPlugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(thisPlugin);

    Plugin::ExecutionResult execRes;
    try { execRes = thisPlugin->execute( args ); }
    catch ( Plugin::ContextIsNotValidException& e ) { 
        LOG4CXX_INFO(logger, "ContextIsNotValidException has been occured, "
                     "create and throw java instance");

        // Find class
        jclass jContExcpCls = env->FindClass( 
                      "Lfermanext/system/Plugin$ContextIsNotValidException;" );
        Q_ASSERT(jContExcpCls);

        // Find ctor
        jmethodID jCtor = env->GetMethodID( jContExcpCls, "<init>",
                                            "(Ljava/lang/String;)V" );
        Q_ASSERT(jCtor);

        // Create java string
        jstring jUuid = env->NewStringUTF( e.pluginUuid.toUtf8().data() );
        Q_ASSERT(jUuid);

        // Create exception
        jthrowable jExcp = (jthrowable)env->NewObject( jContExcpCls, jCtor, 
                                                       jUuid );
        Q_ASSERT(jExcp);

        // Throw exception
        env->Throw( jExcp );

        // After exception throwing value is not important for Java side
        return 0;
    }
    catch ( Plugin::ParamsAreNotAcceptedException& e ) {
        LOG4CXX_INFO(logger, "ParamsAreNotAcceptedException has been occured, "
                     "create and throw java instance");

        // Find class
        jclass jParamsExcpCls = env->FindClass( 
                   "Lfermanext/system/Plugin$ParamsAreNotAcceptedException;" );
        Q_ASSERT(jParamsExcpCls);

        // Find ctor
        jmethodID jCtor = env->GetMethodID( jParamsExcpCls, "<init>",
                                            "(Ljava/lang/String;)V" );
        Q_ASSERT(jCtor);

        // Create java string
        jstring jUuid = env->NewStringUTF( e.pluginUuid.toUtf8().data() );
        Q_ASSERT(jUuid);

        // Create exception
        jthrowable jExcp = (jthrowable)env->NewObject( jParamsExcpCls, jCtor, 
                                                       jUuid );
        Q_ASSERT(jExcp);

        // Find field
        jfieldID jProblemLstId = env->GetFieldID( jParamsExcpCls, 
                                                  "problemList", 
                                                  "Ljava/util/TreeMap;" );
        Q_ASSERT(jProblemLstId);

        // Get field
        jobject jTree = env->GetObjectField( jExcp, jProblemLstId );
        Q_ASSERT(jTree);

        // Find class
        jclass jTreeCls = env->GetObjectClass( jTree );
        Q_ASSERT(jTreeCls);

        // Get put method
        jmethodID jPutId = env->GetMethodID( jTreeCls, "put",
                                             "(Ljava/lang/Object;"
                                             "Ljava/lang/Object;)"
                                             "Ljava/lang/Object;" );
        Q_ASSERT(jPutId);

        typedef QPair<QString, QString> ListPair;
        foreach ( ListPair pair, e.problemList ) {
            QString logMsg( "  %1 (var) -- %2 (problem)" );
            logMsg = logMsg.arg(pair.first).arg(pair.second);
            LOG4CXX_INFO(logger, logMsg.toStdString());

            jstring jFir = env->NewStringUTF( pair.first.toUtf8().data() );
            Q_ASSERT(jFir);
            jstring jSec = env->NewStringUTF( pair.second.toUtf8().data() );
            Q_ASSERT(jSec);

            // Call put method
            env->CallObjectMethod( jTree, jPutId, jFir, jSec );
        }       
        
        // Throw exception
        env->Throw( jExcp );

        // After exception throwing value is not important for Java side
        return 0;
    }
    catch ( Plugin::DependenciesAreNotResolvedException& e ) {
        LOG4CXX_INFO(logger, "DependenciesAreNotResolvedException has been "
                     "occured, create and throw java instance");

        // Find class
        jclass jDepExcpCls = env->FindClass( 
              "Lfermanext/system/Plugin$DependenciesAreNotResolvedException;" );
        Q_ASSERT(jDepExcpCls);

        // Find ctor
        jmethodID jCtor = env->GetMethodID( jDepExcpCls, "<init>",
                                            "(Ljava/lang/String;)V" );
        Q_ASSERT(jCtor);

        // Create java string
        jstring jUuid = env->NewStringUTF( e.pluginUuid.toUtf8().data() );
        Q_ASSERT(jUuid);

        // Create exception
        jthrowable jExcp = (jthrowable)env->NewObject( jDepExcpCls, jCtor, 
                                                       jUuid );
        Q_ASSERT(jExcp);

        // Find field
        jfieldID jUnresTypesId = env->GetFieldID( jDepExcpCls, 
                                                  "unresolvedTypes", 
                                                  "Ljava/util/Vector;" );
        Q_ASSERT(jUnresTypesId);

        // Get field
        jobject jVec = env->GetObjectField( jExcp, jUnresTypesId );
        Q_ASSERT(jVec);

        // Find class
        jclass jVecCls = env->GetObjectClass( jVec );
        Q_ASSERT(jVecCls);

        // Get put method
        jmethodID jAddId = env->GetMethodID( jVecCls, "add",
                                             "(Ljava/lang/Object;)Z" );
        Q_ASSERT(jAddId);

        foreach ( QString type, e.unresolvedTypes ) {
            QString logMsg( "  type %1 is unresolved" );
            LOG4CXX_INFO(logger, logMsg.arg(type).toStdString());

            jstring jType = env->NewStringUTF( type.toUtf8().data() );
            Q_ASSERT(jType);

            // Call put method
            env->CallBooleanMethod( jVec, jAddId, jType );
        }

        // Throw exception
        env->Throw( jExcp );

        // After exception throwing value is not important for Java side
        return 0;
    }
    catch ( Plugin::WrongExecutionArgsException& e ) {
        LOG4CXX_INFO(logger, "WrongExecutionArgsException has been occured, "
                     "create and throw java instance");

        // Find class
        jclass jWrongExcpCls = env->FindClass( 
                     "Lfermanext/system/Plugin$WrongExecutionArgsException;" );
        Q_ASSERT(jWrongExcpCls);

        // Find ctor
        jmethodID jCtor = env->GetMethodID( jWrongExcpCls, "<init>",
                                            "(Ljava/lang/String;)V" );
        Q_ASSERT(jCtor);

        // Create java string
        jstring jUuid = env->NewStringUTF( e.pluginUuid.toUtf8().data() );
        Q_ASSERT(jUuid);

        // Create exception
        jthrowable jExcp = (jthrowable)env->NewObject( jWrongExcpCls, jCtor, 
                                                       jUuid );
        Q_ASSERT(jExcp);

        // Throw exception
        env->Throw( jExcp );

        // After exception throwing value is not important for Java side
        return 0;
    }
    catch ( ... ) {
        LOG4CXX_ERROR(logger, "Unexpected native exception has been occured");

        execRes.status = Plugin::InternalErrStatus;
        execRes.data = "Unexpected native exception has been occured";
    }
    
    // Find enum class
    jclass jEnumCls = env->FindClass( "Ljava/lang/Enum;" );
    Q_ASSERT(jEnumCls);

    // Find plugin$status class
    jclass jStatusCls = env->FindClass( "Lfermanext/system/Plugin$Status;" );
    Q_ASSERT(jStatusCls);

    // Find static valueOf method
    jmethodID jValueOfId = env->GetStaticMethodID( jEnumCls, "valueOf",
                                                   "(Ljava/lang/Class;"
                                                   "Ljava/lang/String;)"
                                                   "Ljava/lang/Enum;" );
    Q_ASSERT(jValueOfId);

    // Create java string
    jstring jElem = 0;

    switch ( execRes.status ) {
    case Plugin::UnknownStatus : {
        LOG4CXX_INFO(logger, "'UnknownStatus' after execution");
        jElem = env->NewStringUTF( QString("UnknownStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::OkStatus : {
        LOG4CXX_INFO(logger, "'OkStatus' after execution");
        jElem = env->NewStringUTF( QString("OkStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::InternalErrStatus : {
        LOG4CXX_INFO(logger, "'InternalErrStatus' after execution");
        jElem = 
            env->NewStringUTF( QString("InternalErrStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::UnresolvedDependStatus : {
        LOG4CXX_INFO(logger, "'UnresolvedDependStatus' after execution");
        jElem = env->NewStringUTF( 
                          QString("UnresolvedDependStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    default : {
        LOG4CXX_ERROR(logger, "Status variable is unknown after execution!");
        jElem = env->NewStringUTF( QString("UnknownStatus").toUtf8().data() );
        Q_ASSERT(jElem);
    }
    }

    // Call static method
    jobject jStatus = env->CallStaticObjectMethod( jEnumCls, jValueOfId, 
                                                   jStatusCls, jElem );
    Q_ASSERT(jStatus);

    // Find class
    jclass jExecResCls = 
        env->FindClass( "Lfermanext/system/Plugin$ExecutionResult;" );
    Q_ASSERT(jExecResCls);

    // Find ctor
    jmethodID jCtor = env->GetMethodID( jExecResCls, "<init>",
                                        "(Lfermanext/system/Plugin$Status;"
                                        "Ljava/lang/String;)V" );
    Q_ASSERT(jCtor);

    LOG4CXX_INFO(logger, QString("execution result data: %1").
                         arg(execRes.data).toStdString());

    // Create java string
    jstring jDataStr = env->NewStringUTF( execRes.data.toUtf8().data() );
    Q_ASSERT(jDataStr);

    // Create new object
    jobject jExecRes = env->NewObject( jExecResCls, jCtor,
                                       jStatus, jDataStr );
    Q_ASSERT(jExecRes);

    return jExecRes;
}

/*
 * Class:     fermanext_system_Plugin
 * Method:    isValid
 * Signature: ()Z
 */
jboolean JNICALL Java_fermanext_system_Plugin_isValid
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "isValid" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    return (plugin == 0 ? false : true);
}

/*****************************************************************************/
