
#ifndef OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H
#define OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H

#include <QPair>

#include "NativePlugin.h"
#include "TrussUnit.h"

class GAOptimizationFitnessFunctionNodePosition: public NativePlugin
{
public:
    GAOptimizationFitnessFunctionNodePosition ( PluginManager& mng, 
                                                const QString& path );
    virtual ~GAOptimizationFitnessFunctionNodePosition ();

    virtual Plugin::ExecutionResult specificExecute ( 
                              const PluginExecutionParams& params,
                              const QList<UUIDObject*>& args,
                              const QHash< QString, QList<Plugin*> >& deps  )
        /*throw (ContextIsNotValidException,
                 ParamsAreNotAcceptedException,
                 DependenciesAreNotResolvedException,
                 WrongExecutionArgsException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Plugin::Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const;
    virtual Plugin::DependenceMode dependenceMode () const;
    virtual const QStringList& requiredPluginTypes () const;

    /** Is not user executable, so returns false */
    virtual bool isUserExecutable () const;

private:
    /** Convert params to x, y */
    QHash< int, QPair<bool, bool> > paramsToOptAxis( 
                              const PluginExecutionParams& ) const;

    /** Changes node position according to GA genome */
    bool changeNodePosition ( TrussUnit*, 
                              const QHash< int, QPair<bool, bool> >&,
                              StringUUIDObject* ) const;
};

#endif //OPTIMIZATION_GA_FITNESS_FUNCTION_NODEPOSITION_H
