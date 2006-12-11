
#ifndef OPTIMIZATION_GA_GALIB_H
#define OPTIMIZATION_GA_GALIB_H

#include "NativePlugin.h"
#include "TrussUnit.h"

#include <ga/ga.h>
#include <ga/GARealGenome.h>
#include <ga/GARealGenome.C>

/** GA objective function */
float GAOptimizationObjective ( GAGenome& g );

/** Converts GA real genome to XML string */
StringUUIDObject genomeToXml( const GARealGenome& );

/** Converts execution result to fitness value */
float resultToFitness ( const Plugin::ExecutionResult& );

class GAOptimization : public NativePlugin
{
public:
    GAOptimization ( PluginManager& mng, const QString& path );
    virtual ~GAOptimization ();

    // GA objective function
    float objectiveFunction ( GAGenome& );

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

private:
    Plugin* fitnessPlugin;
    TrussUnit* trussArg;
};

#endif //OPTIMIZATION_GA_GALIB_H
