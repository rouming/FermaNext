
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
        /*throw (ContextIsNotValidException,
                 ParamsAreNotAcceptedException,
                 DependenciesAreNotResolvedException,
                 WrongExecutionArgsException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const
        /*throw(ParamsAreNotAcceptedException)*/;
    virtual Plugin::DependenceMode dependenceMode () const;

    virtual const QStringList& requiredPluginTypes () const;
    virtual bool isUserExecutable () const;

    JObject javaPluginInstance () const;

private:
    /** 
     * Converts params to java instance.
     * @return 0 if smth goes wrong, valid java object otherwise
     */
    JObject executionParamsToJava ( const PluginExecutionParams& ) const;

    /**
     * Converts dependencies hashtable to java instance.
     * @return 0 if smth goes wrong, valid java object otherwise
     */
    JObject dependenciesHashToJava ( 
        const QHash<QString, QList<Plugin*> >& ) const;

private:
    /** JVM instance */
    JavaVirtualMachine& javaVM;
    /** Java plugin instance */
    JObject javaPluginInst;
    /** Plugin info */
    mutable PluginInfo pluginInf;
    /** Required plugin types */
    mutable QStringList requiredPlgTypes;
};

#endif //JAVAPLUGIN_H
