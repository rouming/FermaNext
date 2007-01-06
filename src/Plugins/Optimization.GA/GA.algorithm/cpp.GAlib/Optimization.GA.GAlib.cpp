
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
    /*throw (ContextIsNotValidException,
             ParamsAreNotAcceptedException,
             DependenciesAreNotResolvedException,
             WrongExecutionArgsException)*/
{
    LOG4CXX_DEBUG(logger, "specificExecute" );

    if ( args.size() != 1 ) {
        LOG4CXX_WARN(logger, "Wrong number of passed arguments." );
        throw WrongExecutionArgsException(getUUID());
    }

    TrussUnit* truss = dynamic_cast<TrussUnit*>(args[0]);
    if ( truss == 0 ) {
        LOG4CXX_WARN(logger, "Execution argument should be an instance of "
                             "TrussUnit." );
        throw WrongExecutionArgsException(getUUID());
    }

    if ( deps.size() != 1 ) {
        LOG4CXX_WARN(logger, "Dependence is wrong: should be one plugin." );
        DependenciesAreNotResolvedException e(getUUID());
        e.unresolvedTypes = requiredPluginTypes();
        throw e;
    }
    if ( deps.values()[0].size() != 1 ) {
        LOG4CXX_WARN(logger, "Dependence is wrong: should be one plugin in "
                             "list." );
        DependenciesAreNotResolvedException e(getUUID());
        e.unresolvedTypes = requiredPluginTypes();
        throw e;
    }

    const QHash<QString, QVariant>& hashParams = params.getParams();

    // Init some inner members for GA objective calling 
    trussArg = truss;
    fitnessPlugin = deps.values()[0][0];

    // Thinking these MIN and MAX are suitable
    GARealAlleleSet alleles( hashParams["minValue"].toDouble(), 
                             hashParams["maxValue"].toDouble() );
    GARealGenome genome( hashParams["genes"].toInt(), 
                         alleles, GAOptimizationObjective );

    int gaType = hashParams["gaType"].toInt();

    GAGeneticAlgorithm* ga = 0;

    if ( gaType == GASteady )
        ga = new GASteadyStateGA(genome);
    else if ( gaType == GADCrowding )
        ga = new GADCrowdingGA(genome);
    else if ( gaType == GAIncremental  )
        ga = new GAIncrementalGA(genome);
    else if ( gaType == GAIncremental  )
        ga = new GAIncrementalGA(genome);
    else if ( gaType == GADeme )
        ga = new GADemeGA(genome);
    else 
        ga = new GASimpleGA(genome);

    GASigmaTruncationScaling scale;      
    ga->set( gaNpReplacement, 0.5 );

    // by default we want to minimize the objective
    ga->minimize();
    // set the scaling method to our sharing
    ga->scaling(scale);
    // how many individuals in the population
    ga->populationSize( hashParams["populations"].toInt() );
    // number of generations to evolve
    ga->nGenerations( hashParams["generations"].toInt() );
    // likelihood of mutating new offspring
    ga->pMutation( hashParams["mutation"].toDouble() );
    // likelihood of crossing over parents
    ga->pCrossover( hashParams["crossing"].toDouble() );
    ga->selectScores( GAStatistics::AllScores );

    ga->initialize();
    ga->evolve();

    delete ga;

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

void GAOptimization::tryToAcceptParams ( 
    const PluginExecutionParams& params ) const
    /*throw (Plugin::ParamsAreNotAcceptedException)*/
{
    const QHash<QString, QVariant>& hashParams = params.getParams();

    ParamsAreNotAcceptedException e(getUUID());
    
    // Check ga type
    if ( ! hashParams.contains("gaType") )
        e.problemList.append( QPair<QString, QString>("gaType", 
                                                      "Does not exist") );
    else if ( hashParams["gaType"].type() != QVariant::Int )
        e.problemList.append( QPair<QString, QString>("gaType", 
                                                      "Is not integer") );
    else if ( hashParams["gaType"].toInt() < 0 || 
              hashParams["gaType"].toInt() > 4 )
        e.problemList.append( QPair<QString, QString>("gaType", 
                                                      "Should be [0, 4]") );

    // Check generations
    if ( ! hashParams.contains("generations") )
        e.problemList.append( QPair<QString, QString>("generations", 
                                                      "Does not exist") );

    else if ( hashParams["generations"].type() != QVariant::Int )
        e.problemList.append( QPair<QString, QString>("generations", 
                                                      "Is not integer") );
    else if ( hashParams["generations"].toInt() < 100 || 
              hashParams["generations"].toInt() > 1000 )
        e.problemList.append( QPair<QString, QString>("generations", 
                                                    "Should be [100, 1000]") );

    // Check populations
    if ( ! hashParams.contains("populations") )
        e.problemList.append( QPair<QString, QString>("populations", 
                                                      "Does not exist") );

    else if ( hashParams["populations"].type() != QVariant::Int )
        e.problemList.append( QPair<QString, QString>("populations", 
                                                      "Is not integer") );
    else if ( hashParams["populations"].toInt() < 50 || 
              hashParams["populations"].toInt() > 500 )
        e.problemList.append( QPair<QString, QString>("populations", 
                                                    "Should be [50, 500]") );


    // Check mutation
    if ( ! hashParams.contains("mutation") )
        e.problemList.append( QPair<QString, QString>("mutation", 
                                                      "Does not exist") );

    else if ( hashParams["mutation"].type() != QVariant::Double )
        e.problemList.append( QPair<QString, QString>("mutation", 
                                                      "Is not double") );
    else if ( hashParams["mutation"].toDouble() < 0.001 || 
              hashParams["mutation"].toDouble() > 99.99 )
        e.problemList.append( QPair<QString, QString>("mutation", 
                                                 "Should be [0.001, 99.99]") );

    // Check crossing
    if ( ! hashParams.contains("crossing") )
        e.problemList.append( QPair<QString, QString>("crossing", 
                                                      "Does not exist") );

    else if ( hashParams["crossing"].type() != QVariant::Double )
        e.problemList.append( QPair<QString, QString>("crossing", 
                                                      "Is not double") );
    else if ( hashParams["crossing"].toDouble() < 0.1 || 
              hashParams["crossing"].toDouble() > 99.9 )
        e.problemList.append( QPair<QString, QString>("crossing", 
                                                 "Should be [0.1, 99.9]") );


    // Check genes
    if ( ! hashParams.contains("genes") )
        e.problemList.append( QPair<QString, QString>("genes", 
                                                      "Does not exist") );

    else if ( hashParams["genes"].type() != QVariant::Int )
        e.problemList.append( QPair<QString, QString>("genes", 
                                                      "Is not integer") );
    else if ( hashParams["genes"].toInt() < 2 || 
              hashParams["genes"].toInt() > 100 )
        e.problemList.append( QPair<QString, QString>("genes", 
                                                      "Should be [2, 100]") );

    // Check minValue
    if ( ! hashParams.contains("minValue") )
        e.problemList.append( QPair<QString, QString>("minValue", 
                                                      "Does not exist") );

    else if ( hashParams["minValue"].type() != QVariant::Double )
        e.problemList.append( QPair<QString, QString>("minValue", 
                                                      "Is not double") );
    else if ( hashParams["minValue"].toDouble() < 0.0 || 
              hashParams["minValue"].toDouble() > 1000.0 )
        e.problemList.append( QPair<QString, QString>("minValue", 
                                                 "Should be [0.0, 1000.0]") );

    // Check maxValue
    if ( ! hashParams.contains("maxValue") )
        e.problemList.append( QPair<QString, QString>("maxValue", 
                                                      "Does not exist") );

    else if ( hashParams["maxValue"].type() != QVariant::Double )
        e.problemList.append( QPair<QString, QString>("maxValue", 
                                                      "Is not double") );
    else if ( hashParams["maxValue"].toDouble() < 0.0 || 
              hashParams["maxValue"].toDouble() > 1000.0 )
        e.problemList.append( QPair<QString, QString>("maxValue", 
                                                 "Should be [0.0, 1000.0]") );

    
    if ( ! e.problemList.isEmpty() )
        throw e;
}

Plugin::DependenceMode GAOptimization::dependenceMode () const
{ return MultiDependent; }

const QStringList& GAOptimization::requiredPluginTypes () const
{ 
    static QStringList types( QStringList() << "optimization.ga.fitness.function.*" );
    return types;
}

/*****************************************************************************/
