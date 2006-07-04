
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

jobject JavaPluginArgumentRegistrator::registerArgument ( 
    UUIDObject* obj, JNIEnv* env, const char* javaClsName )
{
    if ( ! registerArgument(obj) )
        return 0;

    jclass instClass = env->FindClass( javaClsName );
    Q_ASSERT( instClass );

    jmethodID ctor = env->GetMethodID( instClass, "<init>", "()V" );
    Q_ASSERT( ctor );

    jobject jObj = env->NewObject( instClass, ctor );
    Q_ASSERT( jObj );

    jmethodID setUUIDMethod = env->GetMethodID( instClass, "setUUID", 
                                                "(Ljava/lang/String;)V" );
    Q_ASSERT( setUUIDMethod );

    jstring jStrUUID = env->NewStringUTF( qPrintable(obj->getUUID()) );
    env->CallVoidMethod( jObj, setUUIDMethod, jStrUUID );
    return jObj;
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

UUIDObject* JavaPluginArgumentRegistrator::getRegisteredByJavaObj ( 
    JNIEnv* env, jobject self )
{
    jclass instCls = env->GetObjectClass( self );
    Q_ASSERT( instCls );

    jmethodID getUUIDMethod = env->GetMethodID( instCls, "getUUID",
                                                "()Ljava/lang/String;" );
    Q_ASSERT( getUUIDMethod );

    jstring jUuid = (jstring)env->CallObjectMethod( self, getUUIDMethod );
    Q_ASSERT( jUuid );

     // Get chars
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );

    QString uuid = QString::fromUtf8( uuidChars );

    // Free chars
    env->ReleaseStringUTFChars( jUuid, uuidChars );
    
    return getRegistered( uuid );
}

/*****************************************************************************/
