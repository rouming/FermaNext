
#ifndef OPTIMIZATION_GA_GALIB_H
#define OPTIMIZATION_GA_GALIB_H

#include "NativePlugin.h"

class GAOptimization : public NativePlugin
{
public:
    GAOptimization ( PluginManager& mng, const QString& path );
    virtual ~GAOptimization ();

    virtual Plugin::ExecutionResult specificExecute ( 
                              const PluginExecutionParams& params,
                              const QList<UUIDObject*>& args,
                              const QHash< QString, QList<Plugin*> >& deps  )
        /*throw (WrongExecutionArgsException,
                 DependenciesAreNotResolvedException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Plugin::Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const;
    virtual Plugin::DependenceMode dependenceMode () const;
    virtual const QStringList& requiredPluginTypes () const;
};

#endif //OPTIMIZATION_GA_GALIB_H
