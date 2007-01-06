
#include "NativePluginFrontEnd.h"
#include "Optimization.GA.criteria.force.h"
#include "TrussUnit.h"
#include "Log4CXX.h"

#include <float.h>

/*****************************************************************************
 * GA criteria force plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(GAOptimizationCriteriaForce)

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("plugin.GAOptimizationCriteria_cppForce") );

/*****************************************************************************/

GAOptimizationCriteriaForce::GAOptimizationCriteriaForce ( 
    PluginManager& mng, const QString& path ) :
    NativePlugin(mng, path)
{}

GAOptimizationCriteriaForce::~GAOptimizationCriteriaForce ()
{}

bool GAOptimizationCriteriaForce::getForceWeight ( 
    float& forceWeight, 
    const Plugin::ExecutionResult& calcRes )
{
    LOG4CXX_DEBUG(logger, "getForceWeight" );

    if ( calcRes.status != OkStatus ) {
        LOG4CXX_WARN(logger, "Calculation plugin results are not valid." );
        return false;        
    }

    QDomDocument doc;
    if ( ! doc.setContent(calcRes.data) ) {
        LOG4CXX_WARN(logger, "Calculation plugin results XML is not valid." );
        return false;
    }

    QDomElement resultsElem = doc.firstChild().toElement();
    if ( ! resultsElem.hasAttribute("forceWeight") ) {
        LOG4CXX_WARN(logger, "Calculation plugin results don't contain force "
                             "weight." );
        return false;

    }
    QString forceWeightStr = resultsElem.attribute( "forceWeight" );

    bool ok = false;
    float fw = forceWeightStr.toFloat(&ok);
    if ( ! ok ) {
        LOG4CXX_WARN(logger, "Can't convert force weight to double." );
        return false;
    }

    forceWeight = fw;
    return true;
}

Plugin::ExecutionResult GAOptimizationCriteriaForce::specificExecute ( 
    const PluginExecutionParams&,
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
        LOG4CXX_WARN(logger, "First execution argument should be an instance "
                     "of TrussUnit." );
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

    // Execute calculation plugin
    ExecutionResult calcRes = deps.values()[0][0]->execute( args );

    float forceWeight = 0.0f;
    bool ok = getForceWeight( forceWeight, calcRes );
    if ( ! ok )
        return Plugin::ExecutionResult( InternalErrStatus );

    // By default get MAX float number
    QString fitnessValue = QString("%1").arg(FLT_MAX);

    if ( forceWeight > 0.001f )
        fitnessValue = QString("%1").arg(forceWeight);

    return Plugin::ExecutionResult( OkStatus, fitnessValue );
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

bool GAOptimizationCriteriaForce::isUserExecutable () const
{ return false; }

/*****************************************************************************/
