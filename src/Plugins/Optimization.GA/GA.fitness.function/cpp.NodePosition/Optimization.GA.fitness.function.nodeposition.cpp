
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.fitness.function.nodeposition.h"
#include "Log4CXX.h"

/*****************************************************************************
 * GA fitness function plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimizationFitnessFunctionNodePosition)

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("plugin.GAOptimizationFitness_cppNodePosition") );

/*****************************************************************************/

GAOptimizationFitnessFunctionNodePosition::GAOptimizationFitnessFunctionNodePosition ( 
    PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path)
{}

GAOptimizationFitnessFunctionNodePosition::~GAOptimizationFitnessFunctionNodePosition ()
{}

bool GAOptimizationFitnessFunctionNodePosition::changeNodePosition (
    TrussUnit* truss, 
    StringUUIDObject* xmlGenome ) const
{
    Q_ASSERT(truss);
    Q_ASSERT(xmlGenome);

    QDomDocument doc;
    if ( !doc.setContent(xmlGenome->getData()) ) {
        LOG4CXX_WARN(logger, "Can't parse genomes from GA.algorithm.");
        return false;
    }
    
    QDomElement rootElem = doc.documentElement();
    if ( rootElem.isNull() ) {
        LOG4CXX_WARN( logger, "Wrong genomes XML from GA.algorithm." );
        return false;
    }

    QDomNodeList genomes = rootElem.elementsByTagName( "GAGenome" );
    if ( genomes.count() == 0 ) {
        LOG4CXX_WARN( logger, 
                      QString("Genomes list from GA.algorithm is empty: %1.").
                        arg(xmlGenome->getData()).toStdString() );
        return false;
    }

    TrussUnit::NodeList nodes = truss->getNodeList();

    for ( int i = 0; i < genomes.count(); ++i ) {
        QDomNode genome = genomes.item(i);
        if ( ! genome.isElement() ) {
            LOG4CXX_WARN( logger, "Wrong genomes XML from GA.algorithm." );
            return false;
        }

        QDomElement genomeElem = genome.toElement();
        if ( ! genomeElem.hasAttribute( "floatValue" ) ) {
            LOG4CXX_WARN( logger, "Genome must have float value attribute." );
            return false;
        }

        bool ok = false;
        QString floatValue = genomeElem.attribute( "floatValue" );
        float value = floatValue.toFloat(&ok);
        if ( !ok ) {
            LOG4CXX_WARN( logger, "Can't cast genome value to float." );
            return false;
        }

        // TODO: CHOOSE NODE INDEX BY WIDGET
        int nodeIndex = 2;//int(i/2);
        bool x = (i % 2 == 0 ? true : false);


        const DoublePoint& point = nodes[nodeIndex]->getPoint();

       
        // Set nodes position
        if ( x ) {
            LOG4CXX_DEBUG(logger, 
                          QString("Set X axis: %1").arg(value).toStdString() );
            nodes[nodeIndex]->setPoint( value, point.getY() );
        }
        else {
            LOG4CXX_DEBUG(logger, 
                          QString("Set Y axis: %1").arg(value).toStdString() );
            nodes[nodeIndex]->setPoint( point.getX(), value );
        }
    }
    return true;
}

Plugin::ExecutionResult GAOptimizationFitnessFunctionNodePosition::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (WrongExecutionArgsException,
             DependenciesAreNotResolvedException)*/
{
    LOG4CXX_DEBUG(logger, "specificExecute" );

    if ( args.size() != 2 ) {
        LOG4CXX_WARN(logger, "Wrong number of passed arguments." );
        throw WrongExecutionArgsException();
    }

    TrussUnit* truss = dynamic_cast<TrussUnit*>(args[0]);
    if ( truss == 0 ) {
        LOG4CXX_WARN(logger, "First execution argument should be an instance "
                     "of TrussUnit." );
        throw WrongExecutionArgsException();
    }

    StringUUIDObject* xmlGenome = dynamic_cast<StringUUIDObject*>(args[1]);
    if ( xmlGenome == 0 ) {
        LOG4CXX_WARN(logger, "Second execution argument should be an instance "
                     "of xml string genome." );
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

    // Change nodes position according to genome
    bool ok = changeNodePosition( truss, xmlGenome );
    if ( !ok )
        return ExecutionResult( InternalErrStatus );

    // Create execution args for GA.criteria plugin
    QList<UUIDObject*> criteriaArgs;
    criteriaArgs << truss;

    // Execute GA.criteria plugin
    ExecutionResult res = deps.values()[0][0]->execute( criteriaArgs );

    // Return the same results
    return res;
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
