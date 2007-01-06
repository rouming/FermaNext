
#include <QtGlobal>
#include <QHash>

#include "JavaVariableRegistrator.h"

/*****************************************************************************
 * Java Variable Registrator
 *****************************************************************************/

typedef QHash<QString, UUIDObject*> UUIDObjectMap;
Q_GLOBAL_STATIC(UUIDObjectMap, objectMap);

/*****************************************************************************/

void JavaVariableRegistrator::registerVariable ( UUIDObject* uuidObj )
{
    Q_ASSERT(uuidObj);
    objectMap()->insert( uuidObj->getUUID(), uuidObj );
}

jobject JavaVariableRegistrator::registerVariable ( UUIDObject* obj, 
                                                    JNIEnv* env, 
                                                    const char* javaClsName )
{
    // Firstly register object
    registerVariable( obj );

    jclass instClass = env->FindClass( javaClsName );
    Q_ASSERT( instClass );

    jmethodID ctor = env->GetMethodID( instClass, "<init>", "()V" );
    Q_ASSERT( ctor );

    jobject jObj = env->NewObject( instClass, ctor );
    Q_ASSERT( jObj );

    jmethodID setUUIDMethod = env->GetMethodID( instClass, "setUUID", 
                                                "(Ljava/lang/String;)V" );
    Q_ASSERT( setUUIDMethod );

    jstring jStrUUID = env->NewStringUTF( obj->getUUID().toUtf8().data() );
    env->CallVoidMethod( jObj, setUUIDMethod, jStrUUID );
    return jObj;
}

bool JavaVariableRegistrator::unregisterVariable ( UUIDObject* uuidObj )
{
    Q_ASSERT(uuidObj);
    int removed = objectMap()->remove( uuidObj->getUUID() );
    return removed > 0;
}

void JavaVariableRegistrator::unregisterAllVariables ()
{
    objectMap()->clear();
}

bool JavaVariableRegistrator::isRegistered ( UUIDObject* uuidObj )
{
    Q_ASSERT(uuidObj);
    return objectMap()->values().contains(uuidObj);
}

bool JavaVariableRegistrator::isRegistered ( const QString& uuid )
{
    return objectMap()->contains(uuid);
}

UUIDObject* JavaVariableRegistrator::getRegistered ( const QString& uuid )
{
    if ( isRegistered(uuid) )
        return objectMap()->value(uuid);
    return 0;
}

/*****************************************************************************/
