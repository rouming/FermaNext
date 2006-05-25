
#include "include/fermanext_logging_log4cxx_BasicConfigurator.h"

#include "Log4CXX.h"

using log4cxx::BasicConfigurator;

/*
 * Class:     fermanext_logging_log4cxx_BasicConfigurator
 * Method:    configure
 * Signature: ()V
 */
void JNICALL Java_fermanext_logging_log4cxx_BasicConfigurator_configure
  (JNIEnv*, jclass)
{
    BasicConfigurator::configure();
}

/*
 * Class:     fermanext_logging_log4cxx_BasicConfigurator
 * Method:    resetConfiguration
 * Signature: ()V
 */
void JNICALL Java_fermanext_logging_log4cxx_BasicConfigurator_resetConfiguration
  (JNIEnv *, jclass)
{
    BasicConfigurator::resetConfiguration();
}
