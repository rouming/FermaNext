
#include "include/fermanext_logging_log4cxx_Logger.h"

#include <QtGlobal>

#include "Log4CXX.h"

using log4cxx::Logger;
using log4cxx::LoggerPtr;

/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    getLog4CXXLogger
 * Signature: (Ljava/lang/String;)Lfermanext/logging/log4cxx/Logger;
 */
jobject JNICALL Java_fermanext_logging_log4cxx_Logger_getLog4CXXLogger
  (JNIEnv* env, jclass clazz, jstring jStr)
{
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );

    // Create logger by name
    Logger::getLogger( std::string(strChars) );

    env->ReleaseStringUTFChars( jStr, strChars );
    
    jmethodID ctorId = env->GetMethodID( clazz, "<init>", "(Ljava/lang/String;)V" );
    Q_ASSERT( ctorId );

    jobject jLogger = env->NewObject( clazz, ctorId, jStr );
    Q_ASSERT( jLogger );    

    return jLogger;
}


/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    debug
 * Signature: (Ljava/lang/Object;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_Logger_debug
  (JNIEnv* env , jobject jThis, jobject jMsg)
{
    jclass jClass = env->GetObjectClass( jThis );
    Q_ASSERT( jClass );
    jfieldID nameField = env->GetFieldID( jClass, "name",
                                          "Ljava/lang/String;" );
    Q_ASSERT( nameField );
    jstring jStr = (jstring)env->GetObjectField( jThis, nameField );
    Q_ASSERT( jStr );
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    // Get logger
    LoggerPtr logger = Logger::getLogger( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );

    strChars = env->GetStringUTFChars( (jstring)jMsg, 0 );
    Q_ASSERT( strChars );
    LOG4CXX_DEBUG( logger, strChars );
    env->ReleaseStringUTFChars( (jstring)jMsg, strChars );
}


/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    info
 * Signature: (Ljava/lang/Object;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_Logger_info
  (JNIEnv* env, jobject jThis, jobject jMsg)
{
    jclass jClass = env->GetObjectClass( jThis );
    Q_ASSERT( jClass );
    jfieldID nameField = env->GetFieldID( jClass, "name",
                                          "Ljava/lang/String;" );
    Q_ASSERT( nameField );
    jstring jStr = (jstring)env->GetObjectField( jThis, nameField );
    Q_ASSERT( jStr );
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    // Get logger
    LoggerPtr logger = Logger::getLogger( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );

    strChars = env->GetStringUTFChars( (jstring)jMsg, 0 );
    Q_ASSERT( strChars );
    LOG4CXX_INFO( logger, strChars );
    env->ReleaseStringUTFChars( (jstring)jMsg, strChars );
}


/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    warn
 * Signature: (Ljava/lang/Object;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_Logger_warn
  (JNIEnv* env, jobject jThis, jobject jMsg)
{
    jclass jClass = env->GetObjectClass( jThis );
    Q_ASSERT( jClass );
    jfieldID nameField = env->GetFieldID( jClass, "name",
                                          "Ljava/lang/String;" );
    Q_ASSERT( nameField );
    jstring jStr = (jstring)env->GetObjectField( jThis, nameField );
    Q_ASSERT( jStr );
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    // Get logger
    LoggerPtr logger = Logger::getLogger( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );

    strChars = env->GetStringUTFChars( (jstring)jMsg, 0 );
    Q_ASSERT( strChars );
    LOG4CXX_WARN( logger, strChars );
    env->ReleaseStringUTFChars( (jstring)jMsg, strChars );
}


/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    error
 * Signature: (Ljava/lang/Object;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_Logger_error
  (JNIEnv* env, jobject jThis, jobject jMsg)
{
    jclass jClass = env->GetObjectClass( jThis );
    Q_ASSERT( jClass );
    jfieldID nameField = env->GetFieldID( jClass, "name",
                                          "Ljava/lang/String;" );
    Q_ASSERT( nameField );
    jstring jStr = (jstring)env->GetObjectField( jThis, nameField );
    Q_ASSERT( jStr );
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    // Get logger
    LoggerPtr logger = Logger::getLogger( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );

    strChars = env->GetStringUTFChars( (jstring)jMsg, 0 );
    Q_ASSERT( strChars );
    LOG4CXX_ERROR( logger, strChars );
    env->ReleaseStringUTFChars( (jstring)jMsg, strChars );
}


/*
 * Class:     fermanext_logging_log4cxx_Logger
 * Method:    fatal
 * Signature: (Ljava/lang/Object;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_Logger_fatal
  (JNIEnv* env, jobject jThis, jobject jMsg)
{
    jclass jClass = env->GetObjectClass( jThis );
    Q_ASSERT( jClass );
    jfieldID nameField = env->GetFieldID( jClass, "name",
                                          "Ljava/lang/String;" );
    Q_ASSERT( nameField );
    jstring jStr = (jstring)env->GetObjectField( jThis, nameField );
    Q_ASSERT( jStr );
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    // Get logger
    LoggerPtr logger = Logger::getLogger( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );

    strChars = env->GetStringUTFChars( (jstring)jMsg, 0 );
    Q_ASSERT( strChars );
    LOG4CXX_FATAL( logger, strChars );
    env->ReleaseStringUTFChars( (jstring)jMsg, strChars );
}
