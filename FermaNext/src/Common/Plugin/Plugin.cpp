
#include <QCoreApplication>

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

    // Try to workaround problem of gui hanging while execution
    // In ultimate future all execution should be done in other thread.
    QCoreApplication::processEvents();

    PluginExecutionContext& context = plgMng.currentExecutionContext();
    // Step to next context
    context.nextContext();

    if ( !context.isValid() ) {
        LOG4CXX_WARN(logger, "plugin context is not valid");
        throw ContextIsNotValidException(getUUID());
    }

    QHash< QString, QList<Plugin*> > dependencies = 
        context.getCallerDependencies(this);
    PluginExecutionParams execParams = context.getCallerParams(this);

    try { tryToAcceptParams(execParams); }
    catch ( ParamsAreNotAcceptedException& ) {
        LOG4CXX_WARN(logger, "plugin params are not accepted");
        // Step to previous context
        context.previousContext();
        throw;
    }

    emit beforeExecution( *this );

    ExecutionResult result;
    try { result = specificExecute( execParams, args, dependencies ); }
    catch ( WrongExecutionArgsException& e ) {
        LOG4CXX_WARN(logger, QString("execution args of plugin with uuid '%1'"
                             " are wrong").arg(e.pluginUuid).toStdString());
        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;        
    }
    catch ( DependenciesAreNotResolvedException& e ) {
        LOG4CXX_WARN(logger, QString("dependencies of plugin with uuid '%1'"
                             " are not resolved").arg(e.pluginUuid).
                             toStdString());

        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;
    }
    catch ( ContextIsNotValidException& e ) {
        LOG4CXX_WARN(logger, QString("context of plugin with uuid '%1'"
                             " is not valid").arg(e.pluginUuid).toStdString());

        // Step to previous context
        context.previousContext();
        emit afterExecution( *this, ExecutionResult(UnknownStatus) );
        throw;
    }
    catch ( ParamsAreNotAcceptedException& e ) {
        LOG4CXX_WARN(logger, QString("params of plugin with uuid '%1'"
                             " are not accepted").arg(e.pluginUuid).
                             toStdString());

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

    emit afterExecution( *this, result );
    return result; 
}

void Plugin::stopExecution ()
{ /* do nothing */ }

PluginManager& Plugin::pluginManager ()
{ return plgMng; }

const QString& Plugin::pluginPath () const
{ return plgPath; }

Plugin::DependenceMode Plugin::dependenceMode () const
{ return Independent; }

const QStringList& Plugin::requiredPluginTypes () const
{ static QStringList emptyList; return emptyList; }

bool Plugin::isUserExecutable () const
{ return true; }

void Plugin::setStatus ( Status s )
{ status = s; }

/*****************************************************************************/
