
#include "include/fermanext_system_NativePlugin.h"

#include "Plugin.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.system.native_NativePlugin") );

/*****************************************************************************/

/*
 * Class:     fermanext_system_NativePlugin
 * Method:    pluginInfo
 * Signature: ()Lfermanext/system/PluginInfo;
 */
jobject JNICALL Java_fermanext_system_NativePlugin_pluginInfo
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "pluginInfo" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    const PluginInfo& info = plugin->pluginInfo();

    // Find class
    jclass jPlgInfoCls = env->FindClass( "fermanext/system/PluginInfo" );
    Q_ASSERT(jPlgInfoCls);

    // Find ctor
    jmethodID jCtor = env->GetMethodID( jPlgInfoCls, "<init>", 
                 "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    Q_ASSERT(jCtor);

    // Create java strings
    jstring jName = env->NewStringUTF( info.name.toUtf8().data() );
    Q_ASSERT(jName);
    jstring jDesc = env->NewStringUTF( info.description.toUtf8().data() );
    Q_ASSERT(jDesc);
    jstring jType = env->NewStringUTF( info.type.toUtf8().data() );
    Q_ASSERT(jType);

    // Create java info
    jobject jPlgInfo = 
        env->NewObject( jPlgInfoCls, jCtor, jName, jDesc, jType );
    Q_ASSERT(jPlgInfo);
    
    return jPlgInfo;
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    pluginStatusCode
 * Signature: ()Lfermanext/system/Plugin$Status;
 */
jobject JNICALL Java_fermanext_system_NativePlugin_pluginStatusCode
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "pluginStatusCode" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    Plugin::Status status = plugin->pluginStatusCode();

    // Find enum class
    jclass jEnumCls = env->FindClass( "java/lang/Enum" );
    Q_ASSERT(jEnumCls);

    // Find plugin$status class
    jclass jStatusCls = env->FindClass( "fermanext/system/Plugin$Status" );
    Q_ASSERT(jStatusCls);

    // Find static valueOf method
    jmethodID jValueOfId = env->GetStaticMethodID( jEnumCls, "valueOf",
                                                   "(Ljava/lang/Class;"
                                                   "Ljava/lang/String;)"
                                                   "Ljava/lang/Enum;" );
    Q_ASSERT(jValueOfId);

    // Create java string
    jstring jElem = 0;

    switch ( status ) {
    case Plugin::UnknownStatus : {
        LOG4CXX_INFO(logger, "'UnknownStatus' was choosed");
        jElem = env->NewStringUTF( QString("UnknownStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::OkStatus : {
        LOG4CXX_INFO(logger, "'OkStatus' was choosed");
        jElem = env->NewStringUTF( QString("OkStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::InternalErrStatus : {
        LOG4CXX_INFO(logger, "'InternalErrStatus' was choosed");
        jElem = 
            env->NewStringUTF( QString("InternalErrStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::UnresolvedDependStatus : {
        LOG4CXX_INFO(logger, "'UnresolvedDependStatus' was choosed");
        jElem = env->NewStringUTF( 
                          QString("UnresolvedDependStatus").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::ExecutionStopped : {
        LOG4CXX_INFO(logger, "'ExecutionStopped' was choosed");
        jElem = env->NewStringUTF(QString("ExecutionStopped").toUtf8().data());
        Q_ASSERT(jElem);
        break;
    }
    default : {
        LOG4CXX_ERROR(logger, "Status variable is unknown, "
                              "UnknownStatus has been choosed by default!");
        jElem = env->NewStringUTF( QString("UnknownStatus").toUtf8().data() );
        Q_ASSERT(jElem);
    }
    }

    // Call static method
    jobject jStatus = env->CallStaticObjectMethod( jEnumCls, jValueOfId, 
                                                   jStatusCls, jElem );
    Q_ASSERT(jStatus);

    return jStatus;
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    pluginStatusMsg
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL Java_fermanext_system_NativePlugin_pluginStatusMsg
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "pluginStatusMsg" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    jstring jStatusMsg = 
        env->NewStringUTF( plugin->pluginStatusMsg().toUtf8().data() );
    Q_ASSERT(jStatusMsg);

    return jStatusMsg;
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    tryToAcceptParams
 * Signature: (Lfermanext/system/PluginExecutionParams;)V
 */
void JNICALL Java_fermanext_system_NativePlugin_tryToAcceptParams
  (JNIEnv* env, jobject self, jobject jParams)
{
    LOG4CXX_DEBUG(logger, "tryToAcceptParams");

    if ( jParams == 0 ) {
        LOG4CXX_WARN(logger, "executable params are null");
        return;
    }

    // Find class 
    jclass jParamsCls = env->GetObjectClass( jParams );
    Q_ASSERT(jParamsCls);

    // Get getParams method
    jmethodID jGetParamsId = env->GetMethodID( jParamsCls, "getParams",
                                               "()Ljava/util/Map;" );
    Q_ASSERT(jGetParamsId);

    // Call getParams method
    jobject jMap = env->CallObjectMethod( jParams, jGetParamsId );
    Q_ASSERT(jMap);

    // Find class
    jclass jMapCls = env->GetObjectClass( jMap );
    Q_ASSERT(jMapCls);

    // Get entrySet method
    jmethodID jEntrySetId = env->GetMethodID( jMapCls, "entrySet",
                                              "()Ljava/util/Set;" );
    Q_ASSERT(jEntrySetId);

    // Call entrySet method
    jobject jSet = env->CallObjectMethod( jMap, jEntrySetId );
    Q_ASSERT(jSet);
    
    // Find class
    jclass jSetCls = env->GetObjectClass( jSet );
    Q_ASSERT(jSetCls);
    
    // Get toArray method
    jmethodID jToArrayId = env->GetMethodID( jSetCls, "toArray",
                                              "()[Ljava/lang/Object;" );
    Q_ASSERT(jToArrayId);

    // Call toArray method
    jobjectArray jArr = 
        (jobjectArray)env->CallObjectMethod( jSet, jToArrayId );
    Q_ASSERT(jArr);

    jsize jArrSize = env->GetArrayLength( jArr );

    PluginExecutionParams params;
    for ( jsize i = 0; i < jArrSize; ++i ) {
        // Get array element
        jobject jMapEntry = env->GetObjectArrayElement( jArr, i );
        Q_ASSERT(jMapEntry);

        // Find class
        jclass jMapEntryCls = env->GetObjectClass( jMapEntry );
        Q_ASSERT(jMapEntryCls);

        // Find getKey method
        jmethodID jGetKeyId = env->GetMethodID( jMapEntryCls, "getKey",
                                                "()Ljava/lang/Object;" );
        Q_ASSERT(jGetKeyId);

        // Find getValue method
        jmethodID jGetValId = env->GetMethodID( jMapEntryCls, "getValue",
                                                "()Ljava/lang/Object;" );
        Q_ASSERT(jGetValId);

        // Call getKey method
        jstring jKey = (jstring)env->CallObjectMethod( jMapEntry, 
                                                       jGetKeyId );
        Q_ASSERT(jKey);

        // Get chars
        const char* keyChars = env->GetStringUTFChars( jKey, 0 );
        // Init native key and val
        QString key = QString::fromUtf8( keyChars );
        // Free chars
        env->ReleaseStringUTFChars( jKey, keyChars );

        LOG4CXX_INFO(logger, 
                     QString("variable name is '%1'").arg(key).toStdString() );


        // Call getValue method
        jobject jVar = env->CallObjectMethod( jMapEntry, jGetValId );
        Q_ASSERT(jVar);

        // Find class
        jclass jVarCls = env->GetObjectClass( jVar );
        Q_ASSERT(jVarCls);

        // Get getType method
        jmethodID jGetTypeId = env->GetMethodID( jVarCls, "getType", "()I" );
        Q_ASSERT(jGetTypeId);

        // Call getType
        jint jVarType = env->CallIntMethod( jVar, jGetTypeId );

        QVariant var;

        switch ( jVarType ) {
        case  1 : { //BOOLEAN
            // Find method
            jmethodID jGetId = env->GetMethodID( jVarCls, "getBoolean","()Z" );
            Q_ASSERT(jGetId);
            // Call
            bool res = env->CallBooleanMethod( jVar, jGetId );
            var = res;

            LOG4CXX_INFO(logger, QString("fermanext.system.Variant type is "
                                         "Boolean, value is '%1'").arg(res).
                                   toStdString());
            break;
        }
        case  3 : { //CHAR
            // Find method
            jmethodID jGetId = env->GetMethodID( jVarCls, "getChar", "()C" );
            Q_ASSERT(jGetId);
            // Call
            char res = env->CallCharMethod( jVar, jGetId );
            var = res;

            LOG4CXX_INFO(logger, QString("fermanext.system.Variant type is "
                                         "Char, value is '%1'").arg(res).
                                   toStdString());
            break;
        }
        case  5 : { //INT
            // Find method
            jmethodID jGetId = env->GetMethodID( jVarCls, "getInt", "()I" );
            Q_ASSERT(jGetId);
            // Call
            int res = env->CallIntMethod( jVar, jGetId );
            var = res;

            LOG4CXX_INFO(logger, QString("fermanext.system.Variant type is "
                                         "Int, value is '%1'").arg(res).
                                   toStdString());
            break;
        }
        case  8 : { //DOUBLE
            // Find method
            jmethodID jGetId = env->GetMethodID( jVarCls, "getDouble", "()D" );
            Q_ASSERT(jGetId);
            // Call
            double res = env->CallDoubleMethod( jVar, jGetId );
            var = res;

            LOG4CXX_INFO(logger, QString("fermanext.system.Variant type is "
                                         "Double, value is '%1'").arg(res).
                                   toStdString());
            break;
        }
        case 11 : { //STRING
            LOG4CXX_INFO(logger, "fermanext.system.Variant type is String!");
            // Find method
            jmethodID jGetId = env->GetMethodID( jVarCls, "getString", 
                                                 "()Ljava/lang/String;" );
            Q_ASSERT(jGetId);
            // Call
            jstring jStr = (jstring)env->CallObjectMethod( jVar, jGetId );
            if ( jStr == 0 ) {
                var = QString();

                LOG4CXX_WARN(logger, "fermanext.system.Variant type is"
                                     " String, but object is null");
                break;
            }
            // Get chars
            const char* chars = env->GetStringUTFChars( jStr, 0 );
            // Init native key and val
            QString res = QString::fromUtf8( chars );
            // Free chars
            env->ReleaseStringUTFChars( jStr, chars );

            var = res;

            LOG4CXX_INFO(logger, QString("fermanext.system.Variant type is "
                                         "String, value is '%1'").arg(res).
                                   toStdString());
            break;
        }
        case  0 :   //VOID
        case -1 : { //EMPTY or UNKNOWN 
            LOG4CXX_WARN(logger, "fermanext.system.Variant type VOID, EMPRY or"
                         " UNKNOWN!");
            // Just do nothing, type is incorrect
        }
        default : {
            LOG4CXX_WARN(logger, "Unexpected fermanext.system.Variant type!");
            // Just do nothing, type is incorrect
        }
        }

        // Add param to native params instance
        params.addParam( key, var );
    }

    // Get this plugin
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    // Native call

    try { plugin->tryToAcceptParams(params); }
    catch ( Plugin::ParamsAreNotAcceptedException& e ) {
        LOG4CXX_INFO(logger, "ParamsAreNotAcceptedException has been occured, "
                     "create and throw java instance");

        // Find class
        jclass jParamsExcpCls = env->FindClass( 
                     "fermanext/system/Plugin$ParamsAreNotAcceptedException" );
        Q_ASSERT(jParamsExcpCls);

        // Find ctor
        jmethodID jCtor = env->GetMethodID( jParamsExcpCls, "<init>",
                                            "(Ljava/lang/String;)V" );
        Q_ASSERT(jCtor);

        // Create java string
        jstring jMsg = env->NewStringUTF( QString("Params are not accepted").
                                          toUtf8().data() );
        Q_ASSERT(jMsg);

        // Create exception
        jthrowable jExcp = (jthrowable)env->NewObject( jParamsExcpCls, jCtor, 
                                                       jMsg );
        Q_ASSERT(jExcp);

        // Find field
        jfieldID jProblemLstId = env->GetFieldID( jParamsExcpCls, 
                                                  "problemList", 
                                                  "java/util/TreeMap" );
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
    }
    catch ( ... ) {
        LOG4CXX_ERROR(logger, "Unexpected native exception has been occured");
        // Nothing to do
    }
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    dependenceMode
 * Signature: ()Lfermanext/system/Plugin$DependenceMode;
 */
jobject JNICALL Java_fermanext_system_NativePlugin_dependenceMode
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "dependenceMode" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    Plugin::DependenceMode depMode = plugin->dependenceMode();

    // Find enum class
    jclass jEnumCls = env->FindClass( "java/lang/Enum" );
    Q_ASSERT(jEnumCls);

    // Find plugin$dependencemode class
    jclass jDepCls = 
        env->FindClass( "fermanext/system/Plugin$DependenceMode" );
    Q_ASSERT(jDepCls);

    // Find static valueOf method
    jmethodID jValueOfId = env->GetStaticMethodID( jEnumCls, "valueOf",
                                                   "(Ljava/lang/Class;"
                                                   "Ljava/lang/String;)"
                                                   "Ljava/lang/Enum;" );
    Q_ASSERT(jValueOfId);

    // Create java string
    jstring jElem = 0;

    switch ( depMode ) {
    case Plugin::Independent : {
        LOG4CXX_INFO(logger, "'Independent' mode was choosed");
        jElem = env->NewStringUTF( QString("Independent").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::SingleDependent : {
        LOG4CXX_INFO(logger, "'SingleDependent' mode was choosed");
        jElem = env->NewStringUTF( QString("SingleDependent").toUtf8().data());
        Q_ASSERT(jElem);
        break;
    }
    case Plugin::MultiDependent : {
        LOG4CXX_INFO(logger, "'MultiDependent' was choosed");
        jElem = env->NewStringUTF( QString("MultiDependent").toUtf8().data() );
        Q_ASSERT(jElem);
        break;
    }
    default : {
        LOG4CXX_ERROR(logger, "Dependence mode variable is unknown,"
                              "Independent has been choosed by default!");
        jElem = env->NewStringUTF( QString("Independent").toUtf8().data() );
        Q_ASSERT(jElem);
    }
    }

    // Call static method
    jobject jDep = env->CallStaticObjectMethod( jEnumCls, jValueOfId, 
                                                jDepCls, jElem );
    Q_ASSERT(jDep);

    return jDep;
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    requiredPluginTypes
 * Signature: ()[Ljava/lang/String;
 */
jobjectArray JNICALL Java_fermanext_system_NativePlugin_requiredPluginTypes
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "requiredPluginTypes" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);

    const QStringList& requiredTypes = plugin->requiredPluginTypes();

    // Find class
    jclass jStrCls = env->FindClass( "java/lang/String" );
    Q_ASSERT(jStrCls);

    // Create array
    jobjectArray jTypesArr = env->NewObjectArray( requiredTypes.size(), 
                                                  jStrCls, 0 );

    for ( int i = 0; i < requiredTypes.size(); ++i ) {
        QString type = requiredTypes[i];
        LOG4CXX_INFO(logger, QString("  type '%1' is required").arg(type).
                       toStdString());

        jstring jType = env->NewStringUTF( type.toUtf8().data() );
        Q_ASSERT(jType);
        
        // Add elem to java array
        env->SetObjectArrayElement( jTypesArr, i, jType );
    }

    return jTypesArr;
}


/*
 * Class:     fermanext_system_NativePlugin
 * Method:    isUserExecutable
 * Signature: ()Z
 */
jboolean JNICALL Java_fermanext_system_NativePlugin_isUserExecutable
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "isUserExecutable" );
    Plugin* plugin = ::getRegisteredByJavaObjAndCast<Plugin>( env, self );
    Q_ASSERT(plugin);
    return plugin->isUserExecutable();
}


/*****************************************************************************/
