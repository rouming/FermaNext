
#include "PluginManager.h"
#include "Plugin.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("common.Plugin") );

/*****************************************************************************
 * Plugin
 *****************************************************************************/

Plugin::Plugin ( PluginManager& mng, const QString& path ) : 
    status(OkStatus), plgMng(mng), plgPath(path)
{}

Plugin::~Plugin ()
{}

Plugin::ExecutionResult Plugin::execute ( const QList<UUIDObject*>& args ) 
    /*throw (ContextIsNotValidException,
             ParamsAreNotAcceptedException,
             DependenciesAreNotResolvedException,
             WrongExecutionArgsException)*/
{
    LOG4CXX_DEBUG(logger, "execute");
    emit beforeExecution( *this );    

    //// REMOVE THIS THREE LINES ASAP!
    PluginExecutionParams params;
    QHash< QString, QList<Plugin*> > dependencies;
    ExecutionResult result = specificExecute( params, args, dependencies );


    /* Turned off for now.

    PluginExecutionContext& context = plgMng.currentExecutionContext();
    // Step to next context
    context.nextContext();

    if ( !context.isValid() ) {
        LOG4CXX_WARN(logger, "plugin context is not valid");
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw ContextIsNotValidException();
    }

    QHash< QString, QList<Plugin*> > dependencies = 
        context.getCallerDependencies(this);
    PluginExecutionParams execParams = context.getCallerParams(this);

    try { tryToAcceptParams(execParams); }
    catch ( ParamsAreNotAcceptedException& ) {
        LOG4CXX_WARN(logger, "plugin params are not accepted");
        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;
    }

    ExecutionResult result;
    try { result = specificExecute( execParams, args, dependencies ); }
    catch ( WrongExecutionArgsException& ) {
        LOG4CXX_WARN(logger, "plugin execution args are wrong");
        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;        
    }
    catch ( DependenciesAreNotResolvedException& ) {
        LOG4CXX_WARN(logger, "plugin dependencies are not resolved");
        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;
    }
    catch ( ... ) {
        // If unsupported exception has been thrown, do nothing.

        LOG4CXX_ERROR(logger, "plugin has thrown unsupported exception");
    }

    // Step to previous context
    context.previousContext();

    */

    emit afterExecution( *this, result );
    return result; 
}

PluginManager& Plugin::pluginManager ()
{ return plgMng; }

const QString& Plugin::pluginPath () const
{ return plgPath; }

Plugin::DependenceMode Plugin::dependenceMode () const
{ return Independent; }

const QStringList& Plugin::requiredPluginTypes () const
{ static QStringList emptyList; return emptyList; }

void Plugin::setStatus ( Status s )
{ status = s; }

/*****************************************************************************/
