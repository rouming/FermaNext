
#include "include/fermanext_logging_log4cxx_PropertyConfigurator.h"

#include <QtGlobal>

#include "Log4CXX.h"

using log4cxx::PropertyConfigurator;

/*
 * Class:     fermanext_logging_log4cxx_PropertyConfigurator
 * Method:    configureAndWatch
 * Signature: (Ljava/lang/String;J)V
 */
void JNICALL Java_fermanext_logging_log4cxx_PropertyConfigurator_configureAndWatch
  (JNIEnv* env, jclass, jstring jFileName, jlong jDelay)
{
    const char* strChars = env->GetStringUTFChars( jFileName, 0 );
    Q_ASSERT( strChars );
    PropertyConfigurator::configureAndWatch( std::string(strChars), jDelay );
    env->ReleaseStringUTFChars( jFileName, strChars );
}
