
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
    std::string ndcStr = NDC::get();
    return env->NewStringUTF( ndcStr.data() );
}

/*
 * Class:     fermanext_logging_log4cxx_NDC
 * Method:    pop
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL Java_fermanext_logging_log4cxx_NDC_pop
  (JNIEnv* env, jclass)
{
    std::string ndcStr = NDC::pop();
    return env->NewStringUTF( ndcStr.data() );
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
