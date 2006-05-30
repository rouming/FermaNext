
#include "JavaPlugin.h"

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

void JavaPlugin::execute ( const QList<UUIDObject*>& )
    /*throw (WrongExecutionArgsException)*/
{
    //TODO
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

void JavaPlugin::startWizardSetup ()
{
    // TODO
}

JObject JavaPlugin::javaPluginInstance () const
{ return javaPluginInst; }

/*****************************************************************************/
