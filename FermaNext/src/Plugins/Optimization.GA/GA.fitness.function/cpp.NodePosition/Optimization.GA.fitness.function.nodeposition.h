
#ifndef OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H
#define OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H

#include "NativePlugin.h"
#include "TrussUnit.h"

class GAOptimizationFitnessFunctionNodePosition: public NativePlugin
{
public:
    GAOptimizationFitnessFunctionNodePosition ( PluginManager& mng, 
                                                const QString& path );
    virtual ~GAOptimizationFitnessFunctionNodePosition ();

    /** Changes node position according to GA genome */
    bool changeNodePosition ( TrussUnit*, StringUUIDObject* ) const;

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

#endif //OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H
