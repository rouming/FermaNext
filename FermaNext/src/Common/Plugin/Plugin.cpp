
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
    /*throw (WrongExecutionArgsException)*/
{
    LOG4CXX_DEBUG(logger, "execute");
    emit beforeExecution( *this );
    ExecutionResult result = specificExecute( args );
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
