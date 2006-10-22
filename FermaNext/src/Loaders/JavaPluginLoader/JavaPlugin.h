
#ifndef JAVAPLUGIN_H
#define JAVAPLUGIN_H

#include "Plugin.h"

#include "JavaVM/JavaVirtualMachine.h"

/** 
 * Java plugin class. Is a bridge between Ferma calls
 * and real Java plugins. See #Plugin for detailed 
 * description of whole plugin interface.
 */    
class JavaPlugin : public Plugin
{
public:
    JavaPlugin ( JavaVirtualMachine& javaVM,
                 const JObject& javaPlgInst,
                 PluginManager& mng, 
                 const QString& path );
    virtual ~JavaPlugin ();

    virtual ExecutionResult specificExecute ( 
                              const PluginExecutionParams& params,
                              const QList<UUIDObject*>& args,
                              const QHash< QString, QList<Plugin*> >& deps  )
        /*throw (WrongExecutionArgsException,
                 DependenciesAreNotResolvedException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const
        /*throw(Plugin::ParamsAreNotAcceptedException)*/;

    virtual const QStringList& requiredPluginTypes () const;

    JObject javaPluginInstance () const;

private:
    /** JVM instance */
    JavaVirtualMachine& javaVM;
    /** Java plugin instance */
    JObject javaPluginInst;
    /** Plugin info */
    PluginInfo pluginInf;
};

#endif //JAVAPLUGIN_H
