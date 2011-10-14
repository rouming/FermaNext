
#include "PluginManager.h"
#include "JavaPlugin.h"
#include "Log4CXX.h"

#include "JavaPluginArgumentRegistrator.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("loader.JavaPlugin") );

/*****************************************************************************
 * Java Plugin
 *****************************************************************************/

JavaPlugin::JavaPlugin ( JavaVirtualMachine& jvm, 
                         const JObject& javaPlgInst,
                         PluginManager& mng, 
                         const QString& path ) :
    Plugin( mng, path ),
    javaVM(jvm),
    javaPluginInst( javaVM.newGlobalRef(javaPlgInst) )
{
    LOG4CXX_DEBUG(logger, "JavaPlugin constructor");

    // Find class
    JClass jPlgClass = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(jPlgClass);

    // Find ctor
    JMethodID jSetUuidId = javaVM.getMethodID( jPlgClass, "setUUID", 
                                               "(Ljava/lang/String;)V" );
    Q_ASSERT(jSetUuidId);

    // Set uuid
    JString jStrUUID = javaVM.newStringUTF( getUUID().toUtf8().data() );
    javaVM.callVoidMethod( javaPluginInst, jSetUuidId, jStrUUID );
}

JavaPlugin::~JavaPlugin ()
{
    LOG4CXX_DEBUG(logger, "JavaPlugin destructor");
    javaVM.deleteGlobalRef( javaPluginInst );
}

Plugin::ExecutionResult JavaPlugin::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (ContextIsNotValidException,
             ParamsAreNotAcceptedException,
             DependenciesAreNotResolvedException,
             WrongExecutionArgsException)*/
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::specificExecute");

    JClass plgArgCls = javaVM.findClass( "Ljava/lang/String;" );
    Q_ASSERT( plgArgCls );

    // Create reference
    plgArgCls = (JClass)javaVM.newGlobalRef( plgArgCls );
    Q_ASSERT( plgArgCls );

    JObjectArray jArray = javaVM.newObjectArray( args.size(), plgArgCls, 0 );
    Q_ASSERT( jArray );

    // Create reference
    jArray = (JObjectArray)javaVM.newGlobalRef( jArray );
    Q_ASSERT( jArray );

    // Push new argument context
    JavaPluginArgumentRegistrator::push();

    // Fill array with uuids as strings and register arguments
    for ( int i = 0; i < args.size(); ++i ) {
        UUIDObject* obj = args.at(i);
        JavaPluginArgumentRegistrator::registerArgument( obj );
        JString jStr = javaVM.newStringUTF( obj->getUUID().toUtf8().data() );
        Q_ASSERT(jStr);
        javaVM.setObjectArrayElement( jArray, i, jStr );
    }
    
    JClass plgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT( plgInstCls );

    // Create reference
    plgInstCls = (JClass)javaVM.newGlobalRef( plgInstCls );
    Q_ASSERT( plgInstCls );

    JMethodID plgExecuteMethod = 
        javaVM.getMethodID( plgInstCls, "nativeCall_specificExecute",
                            "(Lfermanext/system/PluginExecutionParams;"
                            "[Ljava/lang/String;"
                            "Ljava/util/Hashtable;)"
                            "Lfermanext/system/Plugin$ExecutionResult;" );
    Q_ASSERT( plgExecuteMethod );

    // Create java exec params
    JObject jExecParams = executionParamsToJava( params );
    Q_ASSERT(jExecParams);

    // Create dependencies hash table
    JObject jHashDeps = dependenciesHashToJava( deps );
    Q_ASSERT(jHashDeps);

    // JNI plugin execute call
    JObject execResObj = 
        javaVM.callObjectMethod( javaPluginInst, plgExecuteMethod,
                                 jExecParams, jArray, jHashDeps );

    // We have pending exceptions
    if ( javaVM.exceptionCheck() ) {
        JThrowable jExcp = javaVM.exceptionOccurred();
        Q_ASSERT(jExcp);

        // Clears pending exception
        javaVM.exceptionClear();

        // Create reference
        jExcp = (JThrowable)javaVM.newGlobalRef(jExcp);
        Q_ASSERT(jExcp);

        // Find exception class
        JClass jExcpCls = javaVM.getObjectClass( jExcp );
        Q_ASSERT(jExcpCls);

        // Find base exception class
        JClass jBaseExcpCls = javaVM.findClass(
                                 "Lfermanext/system/Plugin$PluginException;" );
        Q_ASSERT(jBaseExcpCls);

        // Exception must be assignable from base plugin exception
        if ( ! javaVM.isAssignableFrom(jExcpCls, jBaseExcpCls) ) {
            // Find toString method
            JMethodID jToStringId = javaVM.getMethodID( jExcpCls, "toString",
                                                      "()Ljava/lang/String;" );
            Q_ASSERT( jToStringId );

            JString jMsg = (JString)javaVM.callObjectMethod( jExcp, 
                                                             jToStringId );
            Q_ASSERT( jMsg );

            const char* msgChars = javaVM.getStringUTFChars( jMsg, 0 );
            Q_ASSERT( msgChars );

            QString excpDesc = QString::fromUtf8( msgChars );

            // Clean
            javaVM.releaseStringUTFChars( jMsg, msgChars );

            QString errMsg = QString( "Unexpected exception has been occured "
                                      "from JNI side: %1." ).arg(excpDesc);

            LOG4CXX_ERROR(logger, errMsg.toStdString());

            // Free global ref
            javaVM.deleteGlobalRef( plgInstCls );
            javaVM.deleteGlobalRef( jArray );
            javaVM.deleteGlobalRef( plgArgCls );
            javaVM.deleteGlobalRef( jExcp );

            // Pop argument context, return to previous one
            JavaPluginArgumentRegistrator::pop();

            return Plugin::ExecutionResult( Plugin::InternalErrStatus,
                                            errMsg );
        }

        // Get pluginUuid field
        JFieldID jPlgUuidId = javaVM.getFieldID( jExcpCls, "pluginUuid",
                                                 "Ljava/lang/String;" );
        Q_ASSERT(jPlgUuidId);

        // Get uuid from fields
        JString jUuid = (JString)javaVM.getObjectField( jExcp, jPlgUuidId);
        Q_ASSERT(jUuid);

        // Get native string
        const char* uuidChars = javaVM.getStringUTFChars( jUuid, 0 );
        Q_ASSERT(uuidChars);
        QString uuid = QString::fromUtf8(uuidChars);
        javaVM.releaseStringUTFChars( jUuid, uuidChars );


        // Find  context exception class
        JClass jContExcpCls = javaVM.findClass(
                      "Lfermanext/system/Plugin$ContextIsNotValidException;" );
        Q_ASSERT(jContExcpCls);

        // Find  params exception class
        JClass jParamsExcpCls = javaVM.findClass(
                   "Lfermanext/system/Plugin$ParamsAreNotAcceptedException;" );
        Q_ASSERT(jParamsExcpCls);

        // Find  wrong exec args exception class
        JClass jWrongArgsExcpCls = javaVM.findClass(
                     "Lfermanext/system/Plugin$WrongExecutionArgsException;" );
        Q_ASSERT(jWrongArgsExcpCls);

        // Find dependencies exception
        JClass jDepExcpCls = javaVM.findClass(
             "Lfermanext/system/Plugin$DependenciesAreNotResolvedException;" );
        Q_ASSERT(jDepExcpCls);

        // Check if exception can be casted to wrong args exception
        if ( javaVM.isAssignableFrom(jExcpCls, jWrongArgsExcpCls) ) {
            LOG4CXX_INFO(logger, "'WrongExecutionArgsException' exception "
                         "has been occured from JNI side.");

            // Free global ref
            javaVM.deleteGlobalRef( plgInstCls );
            javaVM.deleteGlobalRef( jArray );
            javaVM.deleteGlobalRef( plgArgCls );
            javaVM.deleteGlobalRef( jExcp );

            // Pop argument context, return to previous one
            JavaPluginArgumentRegistrator::pop();

            // Throw exception
            throw Plugin::WrongExecutionArgsException(uuid);
        }
        // Check if exception can be casted to dependencies exception
        else if ( javaVM.isAssignableFrom(jExcpCls, jDepExcpCls) ) {
            LOG4CXX_INFO(logger, "'DependenciesAreNotResolvedException' "
                         "exception has been occured from JNI side.");

            // Create native exception
            Plugin::DependenciesAreNotResolvedException excp(uuid);

            // Get field ids
            JFieldID jUnresTypesId = 
                javaVM.getFieldID( jExcpCls, "unresolvedTypes", 
                                   "Ljava/util/Vector;" );
            Q_ASSERT(jUnresTypesId);

            // Get objects from fields
            JObject jUnresTypes = javaVM.getObjectField( jExcp, jUnresTypesId);
            Q_ASSERT(jUnresTypes);
            
            // Find class
            JClass jVecCls = javaVM.getObjectClass( jUnresTypes );
            Q_ASSERT(jVecCls);

            // Find toArray method
            JMethodID jToArrayId = javaVM.getMethodID( jVecCls, "toArray",
                                                     "()[Ljava/lang/Object;" );
            Q_ASSERT(jToArrayId);

            // Get array
            JObjectArray jUnresArr = 
                (JObjectArray)javaVM.callObjectMethod( jUnresTypes,jToArrayId);
            Q_ASSERT(jUnresArr);

            jsize jArrSize = javaVM.getArrayLength( jUnresArr );

            for ( jsize i = 0; i < jArrSize; ++i ) {
                JString jType = 
                    (JString)javaVM.getObjectArrayElement( jUnresArr, i );
                Q_ASSERT(jType);
                
                const char* typeChars = javaVM.getStringUTFChars( jType, 0 );
                Q_ASSERT( typeChars );
                QString type = QString::fromUtf8(typeChars);
                // Clean
                javaVM.releaseStringUTFChars( jType, typeChars );

                LOG4CXX_INFO(logger, QString("   unresolved type: %1").
                               arg(type).toStdString());

                // Append type to native string list
                excp.unresolvedTypes.append(type);
            }

            // Free global ref
            javaVM.deleteGlobalRef( plgInstCls );
            javaVM.deleteGlobalRef( jArray );
            javaVM.deleteGlobalRef( plgArgCls );
            javaVM.deleteGlobalRef( jExcp );

            // Pop argument context, return to previous one
            JavaPluginArgumentRegistrator::pop();
            
            // Throw formed exception
            throw excp;
        }
        // Check if exception can be casted to context exception
        else if ( javaVM.isAssignableFrom(jExcpCls, jContExcpCls) ) {
            LOG4CXX_INFO(logger, "'ContextIsNotValidException' "
                         "exception has been occured from JNI side.");

            // Free global ref
            javaVM.deleteGlobalRef( plgInstCls );
            javaVM.deleteGlobalRef( jArray );
            javaVM.deleteGlobalRef( plgArgCls );
            javaVM.deleteGlobalRef( jExcp );

            // Pop argument context, return to previous one
            JavaPluginArgumentRegistrator::pop();

            // Throw exception
            throw Plugin::ContextIsNotValidException(uuid);
        }
        // Check if exception can be casted to params exception
        else if ( javaVM.isAssignableFrom(jExcpCls, jParamsExcpCls) ) {
            LOG4CXX_INFO(logger, "'ParamsAreNotAcceptedException' "
                         "exception has been occured from JNI side.");
            
            // Create native exception
            Plugin::ParamsAreNotAcceptedException excp(uuid);

            // Get field ids
            JFieldID jPrblLstFld = javaVM.getFieldID( jExcpCls, 
                                                      "problemList", 
                                                      "Ljava/util/TreeMap;" );
            Q_ASSERT(jPrblLstFld);

            // Get field values
            JObject jTree = javaVM.getObjectField( jExcp, jPrblLstFld );
            Q_ASSERT(jTree);

            // Find class
            JClass jTreeCls = javaVM.getObjectClass( jTree );
            Q_ASSERT(jTreeCls);

            // Find entry set method
            JMethodID jEntrySetId = javaVM.getMethodID( jTreeCls, "entrySet",
                                                        "()Ljava/util/Set;" );
            Q_ASSERT(jEntrySetId);
    
            // Get entry set
            JObject jSet = javaVM.callObjectMethod( jTree, jEntrySetId );
            Q_ASSERT(jSet);

            // Find class
            JClass jSetCls = javaVM.getObjectClass( jSet );
            Q_ASSERT(jSetCls);

            // Find toArray method
            JMethodID jToArrayId = javaVM.getMethodID( jSetCls, "toArray",
                                                     "()[Ljava/lang/Object;" );
            Q_ASSERT(jToArrayId);

            // Get array
            JObjectArray jArr = 
                (JObjectArray)javaVM.callObjectMethod( jSet, jToArrayId );
            Q_ASSERT(jArr);

            jsize jArrSize = javaVM.getArrayLength( jArr );

            LOG4CXX_INFO(logger, 
                         QString("  exception with %1 problems was occured").
                         arg(jArrSize).toStdString());

            for ( jsize i = 0; i < jArrSize; ++i ) {
                JObject jMapEntry = javaVM.getObjectArrayElement( jArr, i );
                Q_ASSERT(jMapEntry);
        
                // Find class
                JClass jMapEntryCls = javaVM.getObjectClass( jMapEntry );
                Q_ASSERT(jMapEntryCls);

                // Find getKey method
                JMethodID jGetKeyId = javaVM.getMethodID( jMapEntryCls, 
                                                          "getKey",
                                                      "()Ljava/lang/Object;" );
                Q_ASSERT(jGetKeyId);

                // Find getValue method
                JMethodID jGetValId = javaVM.getMethodID( jMapEntryCls, 
                                                          "getValue",
                                                      "()Ljava/lang/Object;" );
                Q_ASSERT(jGetValId);

                // Call getKey method
                JString jKey = (JString)javaVM.callObjectMethod( jMapEntry, 
                                                                 jGetKeyId );
                Q_ASSERT(jKey);
        
                // Call getValue method
                JString jVal = (JString)javaVM.callObjectMethod( jMapEntry, 
                                                                 jGetValId );
                Q_ASSERT(jVal);

                // Get chars
                const char* keyChars = javaVM.getStringUTFChars( jKey, 0 );
                const char* valChars = javaVM.getStringUTFChars( jVal, 0 );

                // Init native key and val
                QString key = QString::fromUtf8( keyChars );
                QString val = QString::fromUtf8( valChars );

                // Free chars
                javaVM.releaseStringUTFChars( jKey, keyChars );
                javaVM.releaseStringUTFChars( jVal, valChars );

                LOG4CXX_INFO(logger, 
                             QString("  %1 for variable '%2' problem is '%3'").
                             arg(i).arg(key).arg(val).toStdString());

                // Append problem from Java side
                excp.problemList.append( QPair<QString, QString>(key, val) );
            }

            // Free global ref
            javaVM.deleteGlobalRef( plgInstCls );
            javaVM.deleteGlobalRef( jArray );
            javaVM.deleteGlobalRef( plgArgCls );
            javaVM.deleteGlobalRef( jExcp );

            // Pop argument context, return to previous one
            JavaPluginArgumentRegistrator::pop();

            // Throw formed exception
            throw excp;
        }

        // Unreachable line, should be checked by base plugin exception class
        Q_ASSERT(0);       
    }

    // Create reference
    execResObj = javaVM.newGlobalRef( execResObj );
    Q_ASSERT( execResObj );

    JClass execResCls = javaVM.getObjectClass( execResObj );
    Q_ASSERT( execResCls );

    // Create reference
    execResCls = (JClass)javaVM.newGlobalRef( execResCls );
    Q_ASSERT( execResCls );

    // Get field ids
    JFieldID statusField = javaVM.getFieldID( execResCls, "status", 
                                          "Lfermanext/system/Plugin$Status;" );
    Q_ASSERT( statusField );

    JFieldID dataField = javaVM.getFieldID( execResCls, "data", 
                                          "Ljava/lang/String;" );
    Q_ASSERT( dataField );

    
    // Get objects from fields
    JObject statusObj = javaVM.getObjectField( execResObj, statusField );
    Q_ASSERT( statusObj );

    JString dataStr = (JString)javaVM.getObjectField( execResObj, dataField );
    Q_ASSERT( dataStr );

    JClass statusCls = javaVM.getObjectClass( statusObj );
    Q_ASSERT( statusCls );

    JMethodID ordinalMth = javaVM.getMethodID( statusCls, "ordinal", "()I" );
    JInt order = javaVM.callIntMethod( statusObj, ordinalMth );

    // Get chars
    const char* dataChars = javaVM.getStringUTFChars( dataStr, 0 );

    // Create QString
    QString data( dataChars );

    // Free chars
    javaVM.releaseStringUTFChars( dataStr, dataChars );

    // Clear all references
    javaVM.deleteGlobalRef( plgInstCls );
    javaVM.deleteGlobalRef( jArray );
    javaVM.deleteGlobalRef( plgArgCls );
    javaVM.deleteGlobalRef( execResCls );
    javaVM.deleteGlobalRef( execResObj );

    // Pop argument context, return to previous one
    JavaPluginArgumentRegistrator::pop();

    Plugin::Status status = (Plugin::Status)order;

    return ExecutionResult( status, data );
}

void JavaPlugin::stopExecution ()
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::stopExecution");

    // Find class
    JClass jPlgCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT( jPlgCls );

    // Find stopExecution method
    JMethodID jStopId = javaVM.getMethodID( jPlgCls, "stopExecution", "()V" );
    Q_ASSERT( jStopId );

    // Call stopExecution method
    javaVM.callVoidMethod( javaPluginInst, jStopId );
}

const PluginInfo& JavaPlugin::pluginInfo () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::pluginInfo");

    JClass plgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT( plgInstCls );

    // Create reference
    plgInstCls = (JClass)javaVM.newGlobalRef( plgInstCls );
    Q_ASSERT( plgInstCls );

    JMethodID plgInfoMethod = javaVM.getMethodID( plgInstCls, "pluginInfo",
                                           "()Lfermanext/system/PluginInfo;" );
    Q_ASSERT( plgInfoMethod );

    JObject plgInfoObj = 
        javaVM.callObjectMethod( javaPluginInst, plgInfoMethod );
    Q_ASSERT( plgInfoObj );

    // Create reference
    plgInfoObj = javaVM.newGlobalRef( plgInfoObj );
    Q_ASSERT( plgInfoObj );

    JClass plgInfoCls = javaVM.getObjectClass( plgInfoObj );
    Q_ASSERT( plgInfoCls );

    // Create reference
    plgInfoCls = (JClass)javaVM.newGlobalRef( plgInfoCls );
    Q_ASSERT( plgInfoCls );

    // Get field ids
    JFieldID nameField = javaVM.getFieldID( plgInfoCls, "name", 
                                            "Ljava/lang/String;" );
    Q_ASSERT( nameField );

    JFieldID descField = javaVM.getFieldID( plgInfoCls, "description", 
                                            "Ljava/lang/String;" );
    Q_ASSERT( descField );

    JFieldID typeField = javaVM.getFieldID( plgInfoCls, "type", 
                                            "Ljava/lang/String;" );
    Q_ASSERT( typeField );

    // Get field values
    JString nameStr = (JString)javaVM.getObjectField( plgInfoObj,
                                                      nameField );
    Q_ASSERT( nameStr );

    JString descStr = (JString)javaVM.getObjectField( plgInfoObj,
                                                      descField );
    Q_ASSERT( descStr );

    JString typeStr = (JString)javaVM.getObjectField( plgInfoObj,
                                                      typeField );
    Q_ASSERT( typeStr );

    // Get chars
    const char* nameChars = javaVM.getStringUTFChars( nameStr, 0 );
    const char* descChars = javaVM.getStringUTFChars( descStr, 0 );
    const char* typeChars = javaVM.getStringUTFChars( typeStr, 0 );

    // Init native plugin info struct
    pluginInf.name = QString::fromUtf8( nameChars );
    pluginInf.description = QString::fromUtf8( descChars );
    pluginInf.type = QString::fromUtf8( typeChars );

    // Free chars
    javaVM.releaseStringUTFChars( nameStr, nameChars );
    javaVM.releaseStringUTFChars( descStr, descChars );
    javaVM.releaseStringUTFChars( typeStr, typeChars );

    // Clear all references
    javaVM.deleteGlobalRef( plgInstCls );
    javaVM.deleteGlobalRef( plgInfoObj );
    javaVM.deleteGlobalRef( plgInfoCls );

    return pluginInf;
}

JavaPlugin::Status JavaPlugin::pluginStatusCode () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::pluginStatusCode");

    // Find class
    JClass jPlgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(jPlgInstCls);

    // Find method
    JMethodID jStatusId = javaVM.getMethodID( jPlgInstCls, "pluginStatusCode", 
                                        "()Lfermanext/system/Plugin$Status;" );
    Q_ASSERT(jStatusId);

    // Call pluginStatusCode
    JObject jStatus = javaVM.callObjectMethod( javaPluginInst, jStatusId );
    Q_ASSERT(jStatus);

    // Find class
    JClass jStatusCls = javaVM.getObjectClass( jStatus );
    Q_ASSERT(jStatusCls);

    // Find ordinal method
    JMethodID jOrdinalId = javaVM.getMethodID( jStatusCls, "ordinal", "()I" );
    Q_ASSERT(jOrdinalId);

    // Call ordinal method
    jint order = javaVM.callIntMethod( jStatus, jOrdinalId );

    return (Plugin::Status)order;
}

QString JavaPlugin::pluginStatusMsg () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::pluginStatusMsg");

    QString plgStatusMsg;

    // Find class
    JClass plgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(plgInstCls);

    // Find method
    JMethodID jPlgStatusMsgId = javaVM.getMethodID( plgInstCls, 
                                                    "pluginStatusMsg",
                                                    "()Ljava/lang/String;" );
    Q_ASSERT(jPlgStatusMsgId);

    // Call java pluginStatisMsg
    JString jPlgStatusMsg = (JString)javaVM.callObjectMethod( javaPluginInst, 
                                                             jPlgStatusMsgId );
    Q_ASSERT(jPlgStatusMsg);

    // Get chars
    const char* msgChars = javaVM.getStringUTFChars( jPlgStatusMsg, 0 );

    // Init native string
    plgStatusMsg = QString::fromUtf8( msgChars );

    // Free chars
    javaVM.releaseStringUTFChars( jPlgStatusMsg, msgChars );

    return plgStatusMsg;
}

void JavaPlugin::tryToAcceptParams ( const PluginExecutionParams& p ) const
    /*throw(Plugin::ParamsAreNotAcceptedException)*/
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::tryToAcceptParams");

    JObject jParams = executionParamsToJava(p);
    Q_ASSERT(jParams);

    // Find class
    JClass jPlgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(jPlgInstCls);

    // Find method
    JMethodID jTryToAcceptParamsId = javaVM.getMethodID( 
                               jPlgInstCls, "tryToAcceptParams",
                               "(Lfermanext/system/PluginExecutionParams;)V" );
    Q_ASSERT(jTryToAcceptParamsId);

    // Call java tryToAcceptParams
    javaVM.callVoidMethod( javaPluginInst, jTryToAcceptParamsId, jParams );

    // If there is no any pending exceptions
    if ( ! javaVM.exceptionCheck() ) {
        LOG4CXX_DEBUG(logger, "  no any pending exceptions");
        // Evrth is ok!
        return;
    }

    JThrowable jExcp = javaVM.exceptionOccurred();
    Q_ASSERT(jExcp);

    // Clears pending exception
    javaVM.exceptionClear();

    // Create reference
    jExcp = (JThrowable)javaVM.newGlobalRef(jExcp);

    // Find exception class
    JClass jExcpCls = javaVM.getObjectClass( jExcp );
    Q_ASSERT(jExcpCls);

    // Find basic exception class
    JClass jBaseExcpCls = javaVM.findClass(
                   "Lfermanext/system/Plugin$ParamsAreNotAcceptedException;" );
    Q_ASSERT(jBaseExcpCls);

    // Check if exception can be casted to base exception
    jboolean canBeCasted = javaVM.isAssignableFrom( jExcpCls, jBaseExcpCls );

    if ( ! canBeCasted ) {
        LOG4CXX_ERROR(logger, "Exception has been occured, but it is not "
                              "ParamsAreNotAcceptedException exception.");
        javaVM.deleteGlobalRef(jExcp);                    
        // We can't cast to params exception, so we take uuid of this plugin
        throw Plugin::ParamsAreNotAcceptedException(getUUID());
    }

    // Get pluginUuid field
    JFieldID jPlgUuidId = javaVM.getFieldID( jExcpCls, "pluginUuid",
                                             "Ljava/lang/String;" );
    Q_ASSERT(jPlgUuidId);

    // Get uuid from fields
    JString jUuid = (JString)javaVM.getObjectField( jExcp, jPlgUuidId);
    Q_ASSERT(jUuid);

    // Get native string
    const char* uuidChars = javaVM.getStringUTFChars( jUuid, 0 );
    Q_ASSERT(uuidChars);
    QString uuid = QString::fromUtf8(uuidChars);
    javaVM.releaseStringUTFChars( jUuid, uuidChars );

    // Create native exception
    Plugin::ParamsAreNotAcceptedException excp(uuid);

    // Get field ids
    JFieldID jPrblLstFld = javaVM.getFieldID( jBaseExcpCls, "problemList", 
                                              "Ljava/util/TreeMap;" );
    Q_ASSERT(jPrblLstFld);

    // Get field values
    JObject jTree = javaVM.getObjectField( jExcp, jPrblLstFld );
    Q_ASSERT(jTree);

    // Find class
    JClass jTreeCls = javaVM.getObjectClass( jTree );
    Q_ASSERT(jTreeCls);

    // Find entry set method
    JMethodID jEntrySetId = javaVM.getMethodID( jTreeCls, "entrySet",
                                                "()Ljava/util/Set;" );
    Q_ASSERT(jEntrySetId);
    
    // Get entry set
    JObject jSet = javaVM.callObjectMethod( jTree, jEntrySetId );
    Q_ASSERT(jSet);

    // Find class
    JClass jSetCls = javaVM.getObjectClass( jSet );
    Q_ASSERT(jSetCls);

    // Find toArray method
    JMethodID jToArrayId = javaVM.getMethodID( jSetCls, "toArray",
                                               "()[Ljava/lang/Object;" );
    Q_ASSERT(jToArrayId);

    // Get array
    JObjectArray jArr = 
        (JObjectArray)javaVM.callObjectMethod( jSet, jToArrayId );
    Q_ASSERT(jArr);

    jsize jArrSize = javaVM.getArrayLength( jArr );

    LOG4CXX_INFO(logger, QString("  exception with %1 problems was occured").
                     arg(jArrSize).toStdString());        

    for ( jsize i = 0; i < jArrSize; ++i ) {
        JObject jMapEntry = javaVM.getObjectArrayElement( jArr, i );
        Q_ASSERT(jMapEntry);
        
        // Find class
        JClass jMapEntryCls = javaVM.getObjectClass( jMapEntry );
        Q_ASSERT(jMapEntryCls);

        // Find getKey method
        JMethodID jGetKeyId = javaVM.getMethodID( jMapEntryCls, "getKey",
                                                  "()Ljava/lang/Object;" );
        Q_ASSERT(jGetKeyId);

        // Find getValue method
        JMethodID jGetValId = javaVM.getMethodID( jMapEntryCls, "getValue",
                                                  "()Ljava/lang/Object;" );
        Q_ASSERT(jGetValId);

        // Call getKey method
        JString jKey = (JString)javaVM.callObjectMethod( jMapEntry, 
                                                         jGetKeyId );
        Q_ASSERT(jKey);

        // Call getValue method
        JString jVal = (JString)javaVM.callObjectMethod( jMapEntry, 
                                                         jGetValId );
        Q_ASSERT(jVal);

        // Get chars
        const char* keyChars = javaVM.getStringUTFChars( jKey, 0 );
        const char* valChars = javaVM.getStringUTFChars( jVal, 0 );

        // Init native key and val
        QString key = QString::fromUtf8( keyChars );
        QString val = QString::fromUtf8( valChars );

        // Free chars
        javaVM.releaseStringUTFChars( jKey, keyChars );
        javaVM.releaseStringUTFChars( jVal, valChars );

        LOG4CXX_INFO(logger, 
                     QString("    %1 for variable '%2' problem is '%3'").
                     arg(i).arg(key).arg(val).toStdString());

        // Append problem from Java side
        excp.problemList.append( QPair<QString, QString>(key, val) );
    }    
    javaVM.deleteGlobalRef(jExcp);

    // Throw formed exception
    throw excp;
}

Plugin::DependenceMode JavaPlugin::dependenceMode () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::dependenceMode");

    // Find class
    JClass jPlgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(jPlgInstCls);

    // Find method
    JMethodID jDepModeId = javaVM.getMethodID( jPlgInstCls, "dependenceMode", 
                               "()Lfermanext/system/Plugin$DependenceMode;" );
    Q_ASSERT(jDepModeId);

    // Call dependenceMode
    JObject jDepMode = javaVM.callObjectMethod( javaPluginInst, jDepModeId );
    Q_ASSERT(jDepMode);

    // Find class
    JClass jDepModeCls = javaVM.getObjectClass( jDepMode );
    Q_ASSERT(jDepModeCls);

    // Find ordinal method
    JMethodID jOrdinalId = javaVM.getMethodID( jDepModeCls, "ordinal", "()I" );
    Q_ASSERT(jOrdinalId);

    // Call ordinal method
    jint order = javaVM.callIntMethod( jDepMode, jOrdinalId );

    return (Plugin::DependenceMode)order;
}

const QStringList& JavaPlugin::requiredPluginTypes () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::requiredPluginTypes");

    requiredPlgTypes.clear();

    // Find class
    JClass jPlgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT(jPlgInstCls);

    // Find method
    JMethodID jReqPlgTypes = javaVM.getMethodID( jPlgInstCls, 
                                                 "requiredPluginTypes", 
                                                 "()[Ljava/lang/String;" );
    Q_ASSERT(jReqPlgTypes);

    // Call requiredPluginTypes method
    JObjectArray jStrings = 
        (JObjectArray)javaVM.callObjectMethod( javaPluginInst, jReqPlgTypes );
    Q_ASSERT(jStrings);

    jsize jArrSize = javaVM.getArrayLength( jStrings );
    for ( jsize i = 0; i < jArrSize; ++i ) {
        JString jString = (JString)javaVM.getObjectArrayElement( jStrings, i );
        Q_ASSERT(jString);

        // Get chars
        const char* typeChars = javaVM.getStringUTFChars( jString, 0 );

        // Append to native list
        requiredPlgTypes.append( QString::fromUtf8(typeChars) );

        // Free chars
        javaVM.releaseStringUTFChars( jString, typeChars );
    }     
    return requiredPlgTypes;
}

bool JavaPlugin::isUserExecutable () const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::isUserExecutable");

    // Find class
    JClass jPlgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT( jPlgInstCls );

    // Find method
    JMethodID jIsUserExec = javaVM.getMethodID( 
                                      jPlgInstCls, "isUserExecutable", "()Z" );
    Q_ASSERT(jIsUserExec);

    // Call isUserExecutable
    return javaVM.callBooleanMethod( javaPluginInst, jIsUserExec );
}

JObject JavaPlugin::javaPluginInstance () const
{ return javaPluginInst; }

JObject JavaPlugin::executionParamsToJava ( 
    const PluginExecutionParams& params ) const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::executionParamsToJava");

    // Find plugin execution params class
    JClass jExecParamsCls = javaVM.findClass( 
                                  "Lfermanext/system/PluginExecutionParams;" );
    Q_ASSERT( jExecParamsCls );

    // Find variant class
    JClass jVariantCls = javaVM.findClass( "Lfermanext/system/Variant;" );
    Q_ASSERT( jVariantCls );

    // Find exec params ctor
    JMethodID jExecCtor = javaVM.getMethodID( jExecParamsCls, "<init>","()V" );
    Q_ASSERT( jExecCtor );

    // Create exec params object
    JObject jExecParams = javaVM.newObject( jExecParamsCls, jExecCtor );
    Q_ASSERT( jExecParams );

    // Find addParam method
    JMethodID jAddParam = javaVM.getMethodID( jExecParamsCls, "addParam", 
                                              "(Ljava/lang/String;"
                                              "Lfermanext/system/Variant;)V" );
    Q_ASSERT( jAddParam );

    const QHash<QString, QVariant>& hash = params.getParams();

    foreach ( QString key, hash.keys() ) {
        const QVariant& value = hash[key];
        JObject jVar = 0;

        LOG4CXX_INFO(logger, 
                     QString("create java Variant for key '%1'").arg(key).
                       toStdString() );

        switch ( value.type() ) {
        case QVariant::Bool : {
            LOG4CXX_INFO(logger, "QVariant type is Bool!");
            JMethodID jCtor = javaVM.getMethodID( jVariantCls, "<init>", 
                                                  "(Z)V" );
            Q_ASSERT(jCtor);
            jVar = javaVM.newObject( jVariantCls, jCtor, value.toBool() );
            Q_ASSERT(jVar);
            break;
        }
        case QVariant::Char : {
            LOG4CXX_INFO(logger, "QVariant type is Char!");
            JMethodID jCtor = javaVM.getMethodID( jVariantCls, "<init>", 
                                                  "(C)V" );
            Q_ASSERT(jCtor);
            jVar = javaVM.newObject( jVariantCls, jCtor, 
                                     value.toChar().toAscii() );
            Q_ASSERT(jVar);
            break;
        }
        case QVariant::Double : {
            LOG4CXX_INFO(logger, "QVariant type is Double!");
            JMethodID jCtor = javaVM.getMethodID( jVariantCls, "<init>", 
                                                  "(D)V" );
            Q_ASSERT(jCtor);
            jVar = javaVM.newObject( jVariantCls, jCtor, value.toDouble() );
            Q_ASSERT(jVar);
            break;
        }
        case QVariant::Int : {
            LOG4CXX_INFO(logger, "QVariant type is Int!");
            JMethodID jCtor = javaVM.getMethodID( jVariantCls, "<init>", 
                                                  "(I)V" );
            Q_ASSERT(jCtor);
            jVar = javaVM.newObject( jVariantCls, jCtor, value.toInt() );
            Q_ASSERT(jVar);
            break;
        }
        case QVariant::String : {
            LOG4CXX_INFO(logger, "QVariant type is String!");
            JMethodID jCtor = javaVM.getMethodID( jVariantCls, "<init>", 
                                               "(Ljava/lang/String;)V" );
            Q_ASSERT(jCtor);
            JString jStr = (JString)javaVM.newStringUTF( 
                                            value.toString().toUtf8().data() );
            jVar = javaVM.newObject( jVariantCls, jCtor, jStr );
            Q_ASSERT(jVar);
            break;
        }
        case QVariant::Invalid : {
            LOG4CXX_WARN(logger, "QVariant type is invalid!");
            continue;
        }
        default : {
            LOG4CXX_WARN(logger, "QVariant type is unknown!");
            continue;
        }
        }

        JString jStr = javaVM.newStringUTF( key.toUtf8().data() );
        Q_ASSERT(jStr);
        
        // JNI call
        javaVM.callVoidMethod( jExecParams, jAddParam, jStr, jVar );
    }
    
    return jExecParams;
}

JObject JavaPlugin::dependenciesHashToJava ( 
    const QHash<QString, QList<Plugin*> >& deps ) const
{
    LOG4CXX_DEBUG(logger, "JavaPlugin::dependenciesHashToJava");

    // Find plugin class
    JClass jPlgCls = javaVM.findClass( "Lfermanext/system/Plugin;" );
    Q_ASSERT( jPlgCls );

    // Find native plugin class
    JClass jNativePlgCls = javaVM.findClass("Lfermanext/system/NativePlugin;");
    Q_ASSERT( jNativePlgCls );

    // Find native plugin ctor
    JMethodID jNativePlgCtor = javaVM.getMethodID( jNativePlgCls, "<init>", 
                                        "(Lfermanext/system/PluginManager;"
                                        "Ljava/lang/String;)V" );
    Q_ASSERT( jNativePlgCtor );

    // Find hash class
    JClass jHashCls = javaVM.findClass( "Ljava/util/Hashtable;" );
    Q_ASSERT( jHashCls );

    // Find hash ctor
    JMethodID jHashCtor = javaVM.getMethodID( jHashCls, "<init>", "()V" );
    Q_ASSERT( jHashCtor );

    // Find hash put method
    JMethodID jHashPut = javaVM.getMethodID( jHashCls, "put", 
                                             "(Ljava/lang/Object;"
                                             "Ljava/lang/Object;)"
                                             "Ljava/lang/Object;" );
    Q_ASSERT( jHashPut );

    // Create hash object
    JObject jHash = javaVM.newObject( jHashCls, jHashCtor );
    Q_ASSERT( jHash );

    foreach ( QString key, deps.keys() ) {

        LOG4CXX_INFO(logger, QString("Create plugins list for key '%1'").
                               arg(key).toStdString() );

        const QList<Plugin*>& plugins = deps[key];

        // Create plugins array
        JObjectArray jPlgArr = javaVM.newObjectArray( plugins.size(), 
                                                      jPlgCls, 0 );
        Q_ASSERT( jPlgArr );

        // Fill array with uuids as strings and register arguments
        for ( int i = 0; i < plugins.size(); ++i ) {
            Plugin* plugin = plugins.at(i);

            LOG4CXX_INFO(logger, QString("   add plugin '%1' to array").
                         arg(plugin->pluginInfo().name).toStdString() );

            // Register plugin
            JavaPluginArgumentRegistrator::registerArgument( plugin );

            JObject jPlgInst = 0;

#ifndef _MAC_
            // Try to cast to java plugin
            JavaPlugin* javaPlugin = dynamic_cast<JavaPlugin*>( plugin );
#else
            //TODO: I don't know how to link main FermaNext binary on MAC with exporting
            //      all dynamic symbols. On real Unix '-export-dynamic' is used.
            JavaPlugin* javaPlugin = static_cast<JavaPlugin*>( plugin );
#endif
            if ( javaPlugin ) {
                LOG4CXX_INFO(logger, QString("  plugin '%1' is java plugin").
                             arg(plugin->pluginInfo().name).toStdString() );

                // Get instance from java plugin
                jPlgInst = javaPlugin->javaPluginInstance();
                Q_ASSERT( jPlgInst );
            } 
            else {
                LOG4CXX_INFO(logger, QString("  plugin '%1' is native plugin").
                             arg(plugin->pluginInfo().name).toStdString() );

                // Register plugin manager and create java wrapper
                JObject jPlgMng = JavaPluginArgumentRegistrator::
                    registerArgument( &plugin->pluginManager(), javaVM,
                                      "Lfermanext/system/PluginManager;" );

                Q_ASSERT(jPlgMng);

                // Create java string: plugin path
                JString jPlgPath = javaVM.newStringUTF( 
                                        plugin->pluginPath().toUtf8().data() );
                Q_ASSERT(jPlgPath);               
               
                // Create native plugin instance
                jPlgInst = javaVM.newObject( jNativePlgCls, jNativePlgCtor,
                                             jPlgMng, jPlgPath );
                Q_ASSERT( jPlgInst );

                JMethodID jSetUuid = javaVM.getMethodID( 
                                                    jNativePlgCls, "setUUID", 
                                                    "(Ljava/lang/String;)V" );
                Q_ASSERT( jSetUuid );

                JString jStrUuid = 
                    javaVM.newStringUTF( plugin->getUUID().toUtf8().data() );
                Q_ASSERT(jStrUuid);

                javaVM.callVoidMethod( jPlgInst, jSetUuid, jStrUuid );
            }           

            javaVM.setObjectArrayElement( jPlgArr, i, jPlgInst );
        }

        JString jStr = javaVM.newStringUTF( key.toUtf8().data() );
        Q_ASSERT(jStr);
                
        // Call hash.put(key, list)
        javaVM.callObjectMethod( jHash, jHashPut, jStr, jPlgArr );
    }

    return jHash;
}

/*****************************************************************************/
