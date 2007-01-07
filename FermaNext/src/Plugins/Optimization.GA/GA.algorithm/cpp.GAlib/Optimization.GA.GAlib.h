
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
        /*throw (ContextIsNotValidException,
                 ParamsAreNotAcceptedException,
                 DependenciesAreNotResolvedException,
                 WrongExecutionArgsException)*/;

    virtual void stopExecution ();

    bool isExecutionStopped ();

    virtual const PluginInfo& pluginInfo () const;
    virtual Plugin::Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const;
    virtual Plugin::DependenceMode dependenceMode () const;
    virtual const QStringList& requiredPluginTypes () const;

private:
    // Exception for internal use
    class ExecutionStoppedException {};

    enum GAType { GASimple = 0, 
                  GASteady = 1, 
                  GADCrowding = 2, 
                  GAIncremental = 3, 
                  GADeme = 4 };

    Plugin* fitnessPlugin;
    TrussUnit* trussArg;
    volatile bool executionStopped;
};

#endif //OPTIMIZATION_GA_GALIB_H
