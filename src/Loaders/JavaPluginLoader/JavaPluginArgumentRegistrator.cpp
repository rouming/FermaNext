
#include "JavaPluginArgumentRegistrator.h"

/*****************************************************************************
 * Java Plugin Argument Registrator
 *****************************************************************************/

JavaPluginArgumentRegistrator::UUIDObjectMapStack* 
    JavaPluginArgumentRegistrator::argsStack = 0;

/*****************************************************************************/

void JavaPluginArgumentRegistrator::push ()
{
    if ( argsStack == 0 )
        argsStack = new UUIDObjectMapStack;
    argsStack->push( new UUIDObjectMap );
}

void JavaPluginArgumentRegistrator::pop ()
{
    if ( top() == 0 )
        return;
    delete argsStack->pop();
    if ( argsStack->isEmpty() ) {
        delete argsStack;
        argsStack = 0;
    }
}

JavaPluginArgumentRegistrator::UUIDObjectMap* 
    JavaPluginArgumentRegistrator::top ()
{
    if ( argsStack == 0 || argsStack->isEmpty() )
        return 0;
    return argsStack->top();
}

bool JavaPluginArgumentRegistrator::registerArgument ( UUIDObject* obj )
{
    if ( obj == 0 || top() == 0 )
        return false;

    top()->insert( obj->getUUID(), obj );
    return true;
}

bool JavaPluginArgumentRegistrator::unregisterArgument ( UUIDObject* obj )
{
    if ( obj == 0 || top() == 0 )
        return false;

    const QString& uuid = obj->getUUID();
    if ( ! top()->contains(uuid) )
        return false;

    top()->remove( uuid );
    return true;
}

void JavaPluginArgumentRegistrator::unregisterAllArguments ()
{
    if ( top() == 0 )
        return;
    
    top()->clear();
}

bool JavaPluginArgumentRegistrator::isRegistered ( UUIDObject* obj )
{
    if ( obj == 0 || top() == 0 )
        return false;

    return top()->contains( obj->getUUID() );
}
bool JavaPluginArgumentRegistrator::isRegistered ( const QString& uuid )
{
    if ( top() == 0 )
        return false;
    return top()->contains( uuid );
}

UUIDObject* JavaPluginArgumentRegistrator::getRegistered ( 
    const QString& uuid )
{
    if ( top() == 0 )
        return 0;
    if ( ! top()->contains(uuid) )
        return 0;

    return top()->value( uuid );
}

/*****************************************************************************/
