
#include "include/fermanext_logging_log4cxx_NDC.h"

#include <QtGlobal>

#include "Log4CXX.h"

using log4cxx::NDC;

/*
 * Class:     fermanext_logging_log4cxx_NDC
 * Method:    get
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL Java_fermanext_logging_log4cxx_NDC_get
  (JNIEnv* env, jclass)
{
    std::basic_string<log4cxx::logchar> ndcStr = NDC::pop();
#ifdef LOG4CXX_LOGCHAR_IS_UTF8
    return env->NewStringUTF( ndcStr.data() );
#else
    return env->NewString( ndcStr.data(), ndcStr.length() );
#endif
}

/*
 * Class:     fermanext_logging_log4cxx_NDC
 * Method:    pop
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL Java_fermanext_logging_log4cxx_NDC_pop
  (JNIEnv* env, jclass)
{
    std::basic_string<log4cxx::logchar> ndcStr = NDC::pop();
#ifdef LOG4CXX_LOGCHAR_IS_UTF8
    return env->NewStringUTF( ndcStr.data() );
#else
    return env->NewString( ndcStr.data(), ndcStr.length() );
#endif
}

/*
 * Class:     fermanext_logging_log4cxx_NDC
 * Method:    push
 * Signature: (Ljava/lang/String;)V
 */
void JNICALL Java_fermanext_logging_log4cxx_NDC_push
  (JNIEnv* env, jclass, jstring jStr)
{
    const char* strChars = env->GetStringUTFChars( jStr, 0 );
    Q_ASSERT( strChars );
    NDC::push( std::string(strChars) );
    env->ReleaseStringUTFChars( jStr, strChars );
}

/*
 * Class:     fermanext_logging_log4cxx_NDC
 * Method:    remove
 * Signature: ()V
 */
void JNICALL Java_fermanext_logging_log4cxx_NDC_remove
  (JNIEnv*, jclass)
{
    NDC::remove();
}
