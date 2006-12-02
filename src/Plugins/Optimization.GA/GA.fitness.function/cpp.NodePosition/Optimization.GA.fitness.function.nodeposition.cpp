
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.fitness.function.nodeposition.h"

/*****************************************************************************
 * GA fitness function plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimizationFitnessFunctionNodePosition)

/*****************************************************************************/

GAOptimizationFitnessFunctionNodePosition::GAOptimizationFitnessFunctionNodePosition ( 
    PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path)
{}

GAOptimizationFitnessFunctionNodePosition::~GAOptimizationFitnessFunctionNodePosition ()
{}

Plugin::ExecutionResult GAOptimizationFitnessFunctionNodePosition::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (WrongExecutionArgsException,
             DependenciesAreNotResolvedException)*/
{
    qWarning("Optimization.GA.fitness.function.nodeposition::execute");

    if ( deps.size() == 0 ) {
        DependenciesAreNotResolvedException e;
        e.unresolvedTypes = requiredPluginTypes();
        throw e;
    }

    foreach ( QString key, deps.keys() ) {
        qWarning( "key = %s", qPrintable(key) );
        foreach ( Plugin* plugin, deps[key] ) {
            qWarning( "   plugin = %s", 
                      qPrintable(plugin->pluginInfo().name) );
            plugin->execute( args );
        }
    }

    return Plugin::ExecutionResult( OkStatus, QString() );
}

const PluginInfo& GAOptimizationFitnessFunctionNodePosition::pluginInfo () const
{
    static PluginInfo inf( "Optimization.GA.fitness.function.nodeposition",
                           "Genetic optimization node position fitness function.",
                           "optimization.ga.fitness.function.nodeposition" );
    return inf;
}

Plugin::Status GAOptimizationFitnessFunctionNodePosition::pluginStatusCode () const
{ return OkStatus; }

QString GAOptimizationFitnessFunctionNodePosition::pluginStatusMsg () const
{ return QString(); }

void GAOptimizationFitnessFunctionNodePosition::tryToAcceptParams ( const PluginExecutionParams& ) const
{ /* nothing */ }

Plugin::DependenceMode GAOptimizationFitnessFunctionNodePosition::dependenceMode () const
{ return MultiDependent; }

const QStringList& GAOptimizationFitnessFunctionNodePosition::requiredPluginTypes () const
{ 
    static QStringList types( QStringList() << "optimization.ga.criteria.*" );
    return types;
}

/*****************************************************************************/
