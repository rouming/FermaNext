
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.h"

/*****************************************************************************
 * Old Simple Calculation plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimization)

/*****************************************************************************/

GAOptimization::GAOptimization ( PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path)
{}

GAOptimization::~GAOptimization ()
{}

Plugin::ExecutionResult GAOptimization::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (WrongExecutionArgsException,
             DependenciesAreNotResolvedException)*/
{
    qWarning("Optimization.GA::execute");

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

const PluginInfo& GAOptimization::pluginInfo () const
{
    static PluginInfo inf( "Optimization.GA",
                           "Genetic optimization algorithm",
                           "optimization.ga" );
    return inf;
}

Plugin::Status GAOptimization::pluginStatusCode () const
{ return OkStatus; }

QString GAOptimization::pluginStatusMsg () const
{ return QString(); }

void GAOptimization::tryToAcceptParams ( const PluginExecutionParams& ) const
{ /* nothing */ }

Plugin::DependenceMode GAOptimization::dependenceMode () const
{ return MultiDependent; }

const QStringList& GAOptimization::requiredPluginTypes () const
{ 
    static QStringList types( QStringList() << "calculation.*" );
    return types;
}

/*****************************************************************************/
