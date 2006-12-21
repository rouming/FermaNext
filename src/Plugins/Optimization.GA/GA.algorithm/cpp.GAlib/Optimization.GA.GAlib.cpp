
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.GAlib.h"
#include "Log4CXX.h"

/*****************************************************************************
 * GAlib plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimization)

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("plugin.GAOptimizationAlgorithm") );

/*****************************************************************************/

float GAOptimizationObjective ( GAGenome& g )
{
    GAOptimization* that = PLUGIN_INSTANCE;
    return that->objectiveFunction( g );
}

StringUUIDObject genomeToXml( const GARealGenome& g )
{
    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF-8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement rootElem = doc.createElement( "GAGenomes" );    
    doc.appendChild( rootElem );

    for ( int i = 0; i < g.length(); ++i ) {
        QDomElement elem = doc.createElement( "GAGenome" );    
        elem.setAttribute( "floatValue", g.gene(i) );
        rootElem.appendChild( elem );
    }
    return StringUUIDObject( doc.toString() );
}

float resultToFitness ( const Plugin::ExecutionResult& res )
{
    LOG4CXX_DEBUG(logger, QString("resultToFitness: fitness value %1").
                  arg(res.data).toStdString() );

    if ( res.status != Plugin::OkStatus ) {
        LOG4CXX_WARN(logger, "Execution results of fitness plugin have NOT OK"
                     " status." );
        return 0.0f;
    }
    
    bool ok = false;
    float fitness = res.data.toFloat(&ok);
    if ( ok ) 
        return fitness;
    else {
        LOG4CXX_WARN(logger, "Execution results of fitness plugin does not "
                     "have correct float fitness value." );
        return 0.0f;
    }
}

/*****************************************************************************/

GAOptimization::GAOptimization ( PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path),
    fitnessPlugin(0)
{}

GAOptimization::~GAOptimization ()
{}

float GAOptimization::objectiveFunction ( GAGenome& g )
{
    Q_ASSERT(trussArg);
    Q_ASSERT(fitnessPlugin);

    GARealGenome* realGenome = dynamic_cast<GARealGenome*>(&g);
    if ( realGenome == 0 ) {
        LOG4CXX_ERROR(logger, "Can't cast GAGenome to GARealGenome." );
        // Cast error. Just return zero.
        return 0.0f;
    }

    StringUUIDObject realGenomeXml = ::genomeToXml( *realGenome );
    QList<UUIDObject*> args;
    args << trussArg;
    args << &realGenomeXml;

    // Execute fitness plugin with specified genome
    ExecutionResult fitnessXml = fitnessPlugin->execute( args );

    return ::resultToFitness( fitnessXml );
}

Plugin::ExecutionResult GAOptimization::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (WrongExecutionArgsException,
             DependenciesAreNotResolvedException)*/
{
    LOG4CXX_DEBUG(logger, "specificExecute" );

    if ( args.size() != 1 ) {
        LOG4CXX_WARN(logger, "Wrong number of passed arguments." );
        throw WrongExecutionArgsException();
    }

    TrussUnit* truss = dynamic_cast<TrussUnit*>(args[0]);
    if ( truss == 0 ) {
        LOG4CXX_WARN(logger, "Execution argument should be an instance of "
                             "TrussUnit." );
        throw WrongExecutionArgsException();
    }

    if ( deps.size() != 1 ) {
        LOG4CXX_WARN(logger, "Dependence is wrong: should be one plugin." );
        DependenciesAreNotResolvedException e;
        e.unresolvedTypes = requiredPluginTypes();
        throw e;
    }
    if ( deps.values()[0].size() != 1 ) {
        LOG4CXX_WARN(logger, "Dependence is wrong: should be one plugin in "
                             "list." );
        DependenciesAreNotResolvedException e;
        e.unresolvedTypes = requiredPluginTypes();
        throw e;
    }

    // Init some inner members for GA objective calling 
    trussArg = truss;
    fitnessPlugin = deps.values()[0][0];

    // Thinking these MIN and MAX are suitable
    GARealAlleleSet alleles( 0, 300 );
    GARealGenome genome( 2, alleles, GAOptimizationObjective );

    GASteadyStateGA ga(genome);
    GASigmaTruncationScaling scale;      
    ga.set( gaNpReplacement, 0.5 );

    // by default we want to minimize the objective
    ga.minimize();
    // set the scaling method to our sharing
    ga.scaling(scale);
    // how many individuals in the population
    ga.populationSize( 50 );
    // number of generations to evolve
    ga.nGenerations( 25 );
    // likelihood of mutating new offspring
    ga.pMutation( 0.001 );
    // likelihood of crossing over parents
    ga.pCrossover( 0.9 );
    ga.selectScores( GAStatistics::AllScores );

    ga.initialize();
    ga.evolve();

    // Zero 
    trussArg = 0;
    fitnessPlugin = 0;    

    return Plugin::ExecutionResult( OkStatus, QString() );
}

const PluginInfo& GAOptimization::pluginInfo () const
{
    static PluginInfo inf( "Optimization.GA.GAlib",
                         "Genetic optimization algorithm.<br>"
                         "(with support of GAlib, http://lancet.mit.edu/ga/ )",
                         "optimization.ga.algorithm" );
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
    static QStringList types( QStringList() << "optimization.ga.fitness.function.*" );
    return types;
}

/*****************************************************************************/
