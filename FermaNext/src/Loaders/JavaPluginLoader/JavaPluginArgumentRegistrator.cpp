
#include "JavaPluginArgumentRegistrator.h"

/*****************************************************************************
 * Java Plugin Argument Registrator
 *****************************************************************************/

QHash<QString, UUIDObject*>* JavaPluginArgumentRegistrator::args = 0;

/*****************************************************************************/

void JavaPluginArgumentRegistrator::registerArgument ( UUIDObject* obj )
{
    if ( obj == 0 )
        return;

    if ( args == 0 )
        args = new QHash<QString, UUIDObject*>;

    args->insert( obj->getUUID(), obj );
}

void JavaPluginArgumentRegistrator::unregisterArgument ( UUIDObject* obj )
{
    if ( obj == 0 || args == 0 )
        return;

    const QString& uuid = obj->getUUID();
    if ( ! args->contains(uuid) )
        return;

    args->remove( uuid );

    if ( args->size() == 0 ) {
        delete args;
        args = 0;
    }
}

bool JavaPluginArgumentRegistrator::isRegistered ( UUIDObject* obj )
{
    if ( obj == 0 || args == 0 )
        return false;

    return args->contains( obj->getUUID() );
}
bool JavaPluginArgumentRegistrator::isRegistered ( const QString& uuid )
{
    if ( args == 0 )
        return false;
    return args->contains( uuid );
}

UUIDObject* JavaPluginArgumentRegistrator::getRegistered ( 
    const QString& uuid )
{
    if ( args == 0 )
        return 0;
    if ( !args->contains(uuid) )
        return 0;

    return args->value( uuid );
}

/*****************************************************************************/
