
#include "include/fermanext_trussunit_TrussLoadCase.h"

#include "TrussUnit.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                       "java.fermanext.trussunit.native_TrussLoadCaseArray") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    return (trussLoadCases == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    createLoadCase
 * Signature: ()Lfermanext/trussunit/TrussLoadCase;
 */
jobject JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_createLoadCase
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "createLoadCase()" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return 0;
    }

    TrussUnit::LoadCase& loadCase = trussLoadCases->createLoadCase();

    return JavaPluginArgumentRegistrator::registerArgument( 
                        &loadCase, env, "fermanext/trussunit/TrussLoadCase" );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    removeLoadCase
 * Signature: (Lfermanext/trussunit/TrussLoadCase;)Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_removeLoadCase__Lfermanext_trussunit_TrussLoadCase_2
  (JNIEnv* env, jobject self, jobject jLoadCase)
{
    LOG4CXX_DEBUG( logger, "removeLoadCase(TrussLoadCase)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return false;
    }

    TrussUnit::LoadCase* loadCase =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, jLoadCase );
    if ( loadCase == 0 ) {
        // TrussLoadCase is not valid
        LOG4CXX_WARN( logger, "TrussLoadCase is not valid" );
        return false;
    }
    return trussLoadCases->removeLoadCase( *loadCase );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    removeLoadCase
 * Signature: (I)Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_removeLoadCase__I
  (JNIEnv* env, jobject self, jint indx)
{
    LOG4CXX_DEBUG( logger, "removeLoadCase(int)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return false;
    }
    return trussLoadCases->removeLoadCase( indx );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    setCurrentLoadCase
 * Signature: (I)Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_setCurrentLoadCase__I
  (JNIEnv* env, jobject self, jint indx)
{
    LOG4CXX_DEBUG( logger, "setCurrentLoadCase(int)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return false;
    }
    return trussLoadCases->setCurrentLoadCase( indx );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    setCurrentLoadCase
 * Signature: (Lfermanext/trussunit/TrussLoadCase;)Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_setCurrentLoadCase__Lfermanext_trussunit_TrussLoadCase_2
  (JNIEnv* env, jobject self, jobject jLoadCase)
{
    LOG4CXX_DEBUG( logger, "setCurrentLoadCase(TrussLoadCase)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return false;
    }

    TrussUnit::LoadCase* loadCase =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, jLoadCase );
    if ( loadCase == 0 ) {
        // TrussLoadCase is not valid
        LOG4CXX_WARN( logger, "TrussLoadCase is not valid" );
        return false;
    }
    return trussLoadCases->setCurrentLoadCase( *loadCase );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    getCurrentLoadCase
 * Signature: ()Lfermanext/trussunit/TrussLoadCase;
 */
jobject JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_getCurrentLoadCase
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getCurrentLoadCase()" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return 0;
    }

    TrussUnit::LoadCase* curLoadCase = trussLoadCases->getCurrentLoadCase();
    if ( curLoadCase == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                      curLoadCase, env, "fermanext/system/TrussUnitLoadCase" );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    getLoadCaseIndex
 * Signature: (Lfermanext/trussunit/TrussLoadCase;)I
 */
jint JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_getLoadCaseIndex
  (JNIEnv* env, jobject self, jobject jLoadCase)
{
    LOG4CXX_DEBUG( logger, "getLoadCaseIndex(TrussLoadCase)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return -1;
    }

    TrussUnit::LoadCase* loadCase =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, jLoadCase );
    if ( loadCase == 0 ) {
        // TrussLoadCase is not valid
        LOG4CXX_WARN( logger, "TrussLoadCase is not valid" );
        return -1;
    }

    return trussLoadCases->getLoadCaseIndex( *loadCase );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    findLoadCase
 * Signature: (I)Lfermanext/trussunit/TrussLoadCase;
 */
jobject JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_findLoadCase
  (JNIEnv* env, jobject self, jint indx)
{
    LOG4CXX_DEBUG( logger, "findLoadCase(int)" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return 0;
    }

    TrussUnit::LoadCase* loadCase = trussLoadCases->findLoadCase( indx );
    if ( loadCase == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                         loadCase, env, "fermanext/system/TrussUnitLoadCase" );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCaseArray
 * Method:    countLoadCases
 * Signature: ()I
 */
jint JNICALL Java_fermanext_trussunit_TrussLoadCaseArray_countLoadCases
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countLoadCases()" );
    TrussUnit::LoadCases* trussLoadCases =
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCases>( env, self );
    if ( trussLoadCases == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCaseArray instance is 0, maybe it was "
                              "not properly registered?" );
        return -1;
    }
    return trussLoadCases->countLoadCases();
}

/*****************************************************************************/
