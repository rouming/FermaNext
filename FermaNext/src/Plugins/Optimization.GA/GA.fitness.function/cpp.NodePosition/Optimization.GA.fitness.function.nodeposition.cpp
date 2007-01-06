
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

QHash< int, QPair<bool, bool> > 
GAOptimizationFitnessFunctionNodePosition::paramsToOptAxis( 
    const PluginExecutionParams& params ) const
{
    LOG4CXX_DEBUG(logger, "paramsToOptAxis");

    QHash< int, QPair<bool, bool> > res;

    const QHash<QString, QVariant>& paramsHash = params.getParams();

    QList<QString> names = paramsHash.keys();
    qSort(names.begin(), names.end());

    foreach ( QString name, names ) {
        QVariant var = paramsHash[name];
        bool ok = false;
        int val = var.toInt(&ok);
        LOG4CXX_INFO(logger, QString("param name %1, param value %2, is %3").
                     arg(name).arg(val).arg( (ok ? "OK" : "NOT OK") ).
                     toStdString() );
        if ( !ok )
            continue;

        QRegExp regExp("node(\\d{1,2})_([XY])");
        int pos = regExp.indexIn(name);
        if ( pos == -1 ) {
            LOG4CXX_ERROR(logger, QString("Wrong param with name %1").
                          arg(name).toStdString() );
            continue;
        }

        int index = regExp.cap(1).toInt();
        QString axis = regExp.cap(2);

        if ( ! res.contains(index) )
            res[index] = QPair<bool, bool>(false, false);

        QPair<bool, bool>& axisXY = res[index];
        
        if ( axis == "X" )
            axisXY.first = val;
        else
            axisXY.second = val;        
    }

    return res;
}

bool GAOptimizationFitnessFunctionNodePosition::changeNodePosition (
    TrussUnit* truss,
    const QHash< int, QPair<bool, bool> >& optAxis,
    StringUUIDObject* xmlGenome ) const
{
    Q_ASSERT(truss);
    Q_ASSERT(xmlGenome);

    LOG4CXX_DEBUG(logger, QString("changeNodePosition (string uuid obj : %1").
                            arg(xmlGenome->getData()).toStdString());

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

        int nodeIndex = int(i/2);
        bool x = (i % 2 == 0 ? true : false);

        if ( ! optAxis.contains(nodeIndex + 1) ) {
            LOG4CXX_INFO(logger, QString("Node index %1 does not exist").
                         arg(nodeIndex + 1).toStdString());
            continue;
        }
        
        const DoublePoint& point = nodes[nodeIndex]->getPoint();

        const QPair<bool, bool>& axisXY = optAxis[nodeIndex + 1];
       
        // Set nodes position
        if ( x && axisXY.first ) {
            LOG4CXX_DEBUG(logger, QString("Set X axis (node %1): %2").
                          arg(nodeIndex).arg(value).toStdString() );
            nodes[nodeIndex]->setPoint( value, point.getY() );
        }
        else if ( !x && axisXY.second ) {
            LOG4CXX_DEBUG(logger, QString("Set Y axis (node %1): %2").
                          arg(nodeIndex).arg(value).toStdString() );
            nodes[nodeIndex]->setPoint( point.getX(), value );
        }
    }
    return true;
}

Plugin::ExecutionResult GAOptimizationFitnessFunctionNodePosition::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& args,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (ContextIsNotValidException,
             ParamsAreNotAcceptedException,
             DependenciesAreNotResolvedException,
             WrongExecutionArgsException)*/
{
    LOG4CXX_DEBUG(logger, "specificExecute" );

    if ( args.size() != 2 ) {
        LOG4CXX_WARN(logger, "Wrong number of passed arguments." );
        throw WrongExecutionArgsException(getUUID());
    }

    TrussUnit* truss = dynamic_cast<TrussUnit*>(args[0]);
    if ( truss == 0 ) {
        LOG4CXX_WARN(logger, "First execution argument should be an instance "
                     "of TrussUnit." );
        throw WrongExecutionArgsException(getUUID());
    }

    StringUUIDObject* xmlGenome = dynamic_cast<StringUUIDObject*>(args[1]);
    if ( xmlGenome == 0 ) {
        LOG4CXX_ERROR(logger, 
                      "Second execution argument should be an instance "
                      "of xml string genome." );
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

    QHash< int, QPair<bool, bool> > optAxis = paramsToOptAxis(params);

    // Change nodes position according to genome
    bool ok = changeNodePosition( truss, optAxis, xmlGenome );
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

void GAOptimizationFitnessFunctionNodePosition::tryToAcceptParams ( 
    const PluginExecutionParams& params ) const
    /*throw (Plugin::ParamsAreNotAcceptedException)*/
{
    const QHash<QString, QVariant>& hashParams = params.getParams();

    ParamsAreNotAcceptedException e(getUUID());
    
    // Check selected nodes and axises

    QRegExp regExp("node(\\d{1,2})_([XY])");

    bool nothingIsSelected = true;

    foreach ( QString name, hashParams.keys() ) {
        int pos = regExp.indexIn(name);
        if ( pos == -1 ) {
            e.problemList.append( QPair<QString, QString>(name, 
                                                  "Variable name is wrong!") );
            continue;
        }
        else if ( hashParams[name].type() != QVariant::Int )
            e.problemList.append( QPair<QString, QString>(name, 
                                                          "Is not integer") );

        if ( hashParams[name].toInt() != 0 )
            nothingIsSelected = false;
    }

    if ( nothingIsSelected )
        e.problemList.append( QPair<QString, QString>("any node", 
                                             "Select any node to optimize") );

    if ( ! e.problemList.isEmpty() )
        throw e;
}

Plugin::DependenceMode GAOptimizationFitnessFunctionNodePosition::dependenceMode () const
{ return MultiDependent; }

const QStringList& GAOptimizationFitnessFunctionNodePosition::requiredPluginTypes () const
{ 
    static QStringList types( QStringList() << "optimization.ga.criteria.*" );
    return types;
}

bool GAOptimizationFitnessFunctionNodePosition::isUserExecutable () const
{ return false; }

/*****************************************************************************/
