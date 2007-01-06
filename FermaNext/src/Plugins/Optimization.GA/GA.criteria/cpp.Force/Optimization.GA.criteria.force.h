
#ifndef OPTIMIZATION_GA_CRITERIA_FORCE_H
#define OPTIMIZATION_GA_CRITERIA_FORCE_H

#include "NativePlugin.h"

class GAOptimizationCriteriaForce : public NativePlugin
{
public:
    GAOptimizationCriteriaForce ( PluginManager& mng, const QString& path );
    virtual ~GAOptimizationCriteriaForce ();

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
    /** Returns true and fills fw param if can parse execution results */
    bool getForceWeight ( float& fw, const Plugin::ExecutionResult& );
};

#endif //OPTIMIZATION_GA_CRITERIA_FORCE_H
