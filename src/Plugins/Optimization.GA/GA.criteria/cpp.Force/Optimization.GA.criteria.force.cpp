
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.criteria.force.h"

/*****************************************************************************
 * GA criteria force plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimizationCriteriaForce)

/*****************************************************************************/

GAOptimizationCriteriaForce::GAOptimizationCriteriaForce ( 
    PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path)
{}

GAOptimizationCriteriaForce::~GAOptimizationCriteriaForce ()
{}

Plugin::ExecutionResult GAOptimizationCriteriaForce::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (WrongExecutionArgsException,
             DependenciesAreNotResolvedException)*/
{
    qWarning("Optimization.GA.criteria.force::execute");

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

const PluginInfo& GAOptimizationCriteriaForce::pluginInfo () const
{
    static PluginInfo inf( "Optimization.GA.criteria.force",
                           "Genetic optimization force criteria.",
                           "optimization.ga.criteria.force" );
    return inf;
}

Plugin::Status GAOptimizationCriteriaForce::pluginStatusCode () const
{ return OkStatus; }

QString GAOptimizationCriteriaForce::pluginStatusMsg () const
{ return QString(); }

void GAOptimizationCriteriaForce::tryToAcceptParams ( const PluginExecutionParams& ) const
{ /* nothing */ }

Plugin::DependenceMode GAOptimizationCriteriaForce::dependenceMode () const
{ return MultiDependent; }

const QStringList& GAOptimizationCriteriaForce::requiredPluginTypes () const
{ 
    static QStringList types( QStringList() << "calculation.*" );
    return types;
}

/*****************************************************************************/
