
#include "PluginExecutionParams.h"

/*****************************************************************************/

void PluginExecutionParams::addParam ( const QString& name, 
                                       const QVariant& value )
{ params.insert( name, value ); }

void PluginExecutionParams::removeParam ( const QString& name )
{ params.remove(name); }

const QHash<QString, QVariant>& PluginExecutionParams::getParams () const
{ return params; }

/*****************************************************************************/
