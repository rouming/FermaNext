
#include "Log4CXX.h"
#include "JavaPluginArgumentRegistrator.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( 
  Logger::getLogger("loader.JavaPluginArgumentRegistrator") );

/*****************************************************************************
 * Java Plugin Argument Registrator
 *****************************************************************************/

JavaPluginArgumentRegistrator::UUIDObjectMapStack* 
    JavaPluginArgumentRegistrator::argsStack = 0;

/*****************************************************************************/

void JavaPluginArgumentRegistrator::push ()
{
    LOG4CXX_DEBUG(logger, "push");
    if ( argsStack == 0 )
        argsStack = new UUIDObjectMapStack;
    argsStack->push( new UUIDObjectMap );
}

void JavaPluginArgumentRegistrator::pop ()
{
    LOG4CXX_DEBUG(logger, "pop");
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
    LOG4CXX_DEBUG(logger, "top");
    if ( argsStack == 0 || argsStack->isEmpty() )
        return 0;
    return argsStack->top();
}

bool JavaPluginArgumentRegistrator::registerArgument ( UUIDObject* obj,
                                                       bool shouldBeDeleted )
{
    LOG4CXX_DEBUG(logger, "registerArgument");
    if ( obj == 0 || top() == 0 ) {
        LOG4CXX_WARN(logger, "obj is null or stack is empty");
        return false;
    }

    LOG4CXX_DEBUG(logger, QString("register object with uuid '%1'").
                    arg(obj->getUUID()).toStdString());

    top()->insert( obj->getUUID(), 
                   QPair<UUIDObject*, bool>(obj, shouldBeDeleted) );
    return true;
}

jobject JavaPluginArgumentRegistrator::registerArgument ( 
    UUIDObject* obj, JNIEnv* env, 
    const char* javaClsName,
    bool shouldBeDeleted )
{
    LOG4CXX_DEBUG(logger, "registerArgument (java obj)");
    if ( ! registerArgument(obj, shouldBeDeleted) )
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

    jstring jStrUUID = env->NewStringUTF( obj->getUUID().toUtf8().data() );
    env->CallVoidMethod( jObj, setUUIDMethod, jStrUUID );
    return jObj;
}

JObject JavaPluginArgumentRegistrator::registerArgument ( 
    UUIDObject* obj, JavaVirtualMachine& javaVM, 
    const char* javaClsName,
    bool shouldBeDeleted )
{
    LOG4CXX_DEBUG(logger, "registerArgument (java obj)");
    if ( ! registerArgument(obj, shouldBeDeleted) )
        return 0;

    JClass instClass = javaVM.findClass( javaClsName );
    Q_ASSERT( instClass );

    JMethodID ctor = javaVM.getMethodID( instClass, "<init>", "()V" );
    Q_ASSERT( ctor );

    JObject jObj = javaVM.newObject( instClass, ctor );
    Q_ASSERT( jObj );

    JMethodID setUUIDMethod = javaVM.getMethodID( instClass, "setUUID", 
                                                  "(Ljava/lang/String;)V" );
    Q_ASSERT( setUUIDMethod );

    JString jStrUUID = javaVM.newStringUTF( obj->getUUID().toUtf8().data() );
    javaVM.callVoidMethod( jObj, setUUIDMethod, jStrUUID );
    return jObj;
}

bool JavaPluginArgumentRegistrator::unregisterArgument ( UUIDObject* obj )
{
    LOG4CXX_DEBUG(logger, "unregisterArgument");
    if ( obj == 0 || top() == 0 )
        return false;

    const QString& uuid = obj->getUUID();
    if ( ! top()->contains(uuid) )
        return false;

    // Only operator[] gives us a reference. 
    // We should use a reference to zero a deleted pointer
    QPair<UUIDObject*, bool>& val = (*top())[uuid];
    if ( val.second ) {
        // If true, delete the object
        delete val.first;
        val.first = 0;
    }

    top()->remove( uuid );
    return true;
}

void JavaPluginArgumentRegistrator::unregisterAllArguments ()
{
    LOG4CXX_DEBUG(logger, "unregisterAllArgument");
    if ( top() == 0 )
        return;
    
    foreach ( QString uuid, top()->keys() ) {
        // Only operator[] gives us a reference. 
        // We should use a reference to zero a deleted pointer
        QPair<UUIDObject*, bool>& val = (*top())[uuid];
        if ( val.second ) {
            // If true, delete the object
            delete val.first;
            val.first = 0;
        }
    }
    top()->clear();
}

bool JavaPluginArgumentRegistrator::isRegistered ( UUIDObject* obj )
{
    LOG4CXX_DEBUG(logger, "isRegistered");
    if ( obj == 0 || top() == 0 ) {
        LOG4CXX_WARN(logger, "obj is null or stack is empty");
        return false;
    }

    bool contains = top()->contains( obj->getUUID() );

    if ( contains ) {
        LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' exists").
                      arg(obj->getUUID()).toStdString());
    }
    else {
        LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' does not exist").
                      arg(obj->getUUID()).toStdString());
    }

    return contains;
}
bool JavaPluginArgumentRegistrator::isRegistered ( const QString& uuid )
{
    LOG4CXX_DEBUG(logger, QString("isRegistered (uuid): '%1'").
                    arg(uuid).toStdString());
    if ( top() == 0 ) {
        LOG4CXX_WARN(logger, "stack is empty");
        return false;
    }
    bool contains = top()->contains( uuid );

    if ( contains ) {
        LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' exists").
                      arg(uuid).toStdString());
    }
    else {
        LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' does not exist").
                      arg(uuid).toStdString());
    }

    return contains;
}

UUIDObject* JavaPluginArgumentRegistrator::getRegistered ( 
    const QString& uuid )
{
    LOG4CXX_DEBUG(logger, QString("getRegistered (uuid): '%1'").
                    arg(uuid).toStdString());

    if ( top() == 0 ) {
        LOG4CXX_WARN(logger, "stack is empty");
        return 0;
    }
    if ( ! top()->contains(uuid) ) {
        LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' does not exist").
                      arg(uuid).toStdString());
        return 0;
    }

    LOG4CXX_DEBUG(logger, QString("obj with uuid '%1' exists").
                  arg(uuid).toStdString());

    return top()->value( uuid ).first;
}

UUIDObject* JavaPluginArgumentRegistrator::getRegisteredByJavaObj ( 
    JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG(logger, "getRegisteredByJavaObj" );

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
