
#include "include/fermanext_system_StringUUIDObject.h"

#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.system.native_StringUUIDObject") );

/*****************************************************************************/

/*
 * Class:     fermanext_system_StringUUIDObject
 * Method:    nativeRegistration
 * Signature: ()V
 */
void JNICALL Java_fermanext_system_StringUUIDObject_nativeRegistration
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG(logger, "nativeRegistration");

    // Find class
    jclass jStrUUIDObjCls = env->GetObjectClass( self );
    Q_ASSERT(jStrUUIDObjCls);

    // Find getData method
    jmethodID jGetDataId = env->GetMethodID( jStrUUIDObjCls, "getData",
                                             "()Ljava/lang/String;" );
    Q_ASSERT(jGetDataId);

    // Find getUUID method
    jmethodID jGetUUIDId = env->GetMethodID( jStrUUIDObjCls, "getUUID",
                                             "()Ljava/lang/String;" );
    Q_ASSERT(jGetUUIDId);
    
    // Call getData method
    jstring jData = (jstring)env->CallObjectMethod( self, jGetDataId );
    Q_ASSERT(jData);

    // Get chars
    const char* chars = env->GetStringUTFChars( jData, 0 );
    // Init native key and val
    QString data = QString::fromUtf8( chars );
    // Free chars
    env->ReleaseStringUTFChars( jData, chars );

    // Call getUUID method
    jstring jUuid = (jstring)env->CallObjectMethod( self, jGetUUIDId );
    Q_ASSERT(jUuid);

    // Get chars
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );
    // Init native key and val
    QString uuid = QString::fromUtf8( uuidChars );
    // Free chars
    env->ReleaseStringUTFChars( jUuid, uuidChars );

    // Create native string, register it
    // and delegate management to registrator

    StringUUIDObject* str = new StringUUIDObject( data );
    try { str->setUUID(uuid); }
    catch ( UUIDObject::WrongUUIDException& ) {
        delete str;
        LOG4CXX_ERROR(logger, QString("Wrong uuid '%1'").
                      arg(uuid).toStdString());
        return;
    }

    bool res = JavaPluginArgumentRegistrator::registerArgument( str, true );
    if ( ! res ) {
        delete str;
        LOG4CXX_ERROR(logger, "Can't register StringUUIDObject");
        return;
    }

    // Everything is ok!
}

/*****************************************************************************/
