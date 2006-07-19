
#include "JavaPlugin.h"

#include "JavaPluginArgumentRegistrator.h"

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
    /*** Get plugin info from Java plugin ***/

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
}

JavaPlugin::~JavaPlugin ()
{
    javaVM.deleteGlobalRef( javaPluginInst );
}

JavaPlugin::ExecutionResult JavaPlugin::specificExecute ( 
    const QList<UUIDObject*>& args )
    /*throw (WrongExecutionArgsException)*/
{
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
        JString jStr = javaVM.newStringUTF( obj->getUUID().toAscii().data() );
        javaVM.setObjectArrayElement( jArray, i, jStr );
    }
    
    JClass plgInstCls = javaVM.getObjectClass( javaPluginInst );
    Q_ASSERT( plgInstCls );

    // Create reference
    plgInstCls = (JClass)javaVM.newGlobalRef( plgInstCls );
    Q_ASSERT( plgInstCls );

    JMethodID plgExecuteMethod = javaVM.getMethodID( plgInstCls, "execute",
            "([Ljava/lang/String;)Lfermanext/system/Plugin$ExecutionResult;" );
    Q_ASSERT( plgExecuteMethod );

    JObject execResObj = 
        javaVM.callObjectMethod( javaPluginInst, plgExecuteMethod, jArray );

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

const PluginInfo& JavaPlugin::pluginInfo () const
{ return pluginInf; }


JavaPlugin::Status JavaPlugin::pluginStatusCode () const
{
    // TODO
    return OkStatus; 
}

QString JavaPlugin::pluginStatusMsg () const
{ 
    // TODO
    return "";
}

const QStringList& JavaPlugin::requiredPluginTypes () const
{ 
    // TODO
    static QStringList list; return list; 
}

JavaPlugin::ResolvingMode JavaPlugin::resolvingMode () const
{  
    // TODO
    return Immediately;
}

JObject JavaPlugin::javaPluginInstance () const
{ return javaPluginInst; }

/*****************************************************************************/
