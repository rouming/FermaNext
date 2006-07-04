
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
                         "java.fermanext.trussunit.native_TrussLoadCase") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussLoadCase_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussUnit::LoadCase* trussLoadCase = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, self );
    return (trussLoadCase == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussLoadCase
 * Method:    addLoad
 * Signature: (Lfermanext/trussunit/TrussNode;DD)V
 */
void JNICALL Java_fermanext_trussunit_TrussLoadCase_addLoad
  (JNIEnv* env, jobject self, jobject jNode, jdouble xForce, jdouble yForce)
{
    LOG4CXX_DEBUG( logger, "addLoad(Node, double, double)" );
    TrussUnit::LoadCase* trussLoadCase = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, self );
    if ( trussLoadCase == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCase instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    TrussNode* node = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussNode>( env, jNode );

    if ( node == 0 ) {
        // Node is not valid
        LOG4CXX_WARN( logger, "TrussNode is not valid" );
        return;
    }
    trussLoadCase->addLoad( *node, xForce, yForce );    
}

/*
 * Class:     fermanext_trussunit_TrussLoadCase
 * Method:    removeLoad
 * Signature: (Lfermanext/trussunit/TrussNode;)Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoadCase_removeLoad
  (JNIEnv* env, jobject self, jobject jNode)
{
    LOG4CXX_DEBUG( logger, "removeLoad(Node)" );
    TrussUnit::LoadCase* trussLoadCase = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, self );
    if ( trussLoadCase == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCase instance is 0, maybe it was not "
                              "properly registered?" );
        return false;
    }

    TrussNode* node = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussNode>( env, jNode );
    if ( node == 0 ) {
        // Node is not valid
        LOG4CXX_WARN( logger, "TrussNode is not valid" );
        return false;
    }
    return trussLoadCase->removeLoad( *node );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCase
 * Method:    findLoad
 * Signature: (Lfermanext/trussunit/TrussNode;)Lfermanext/trussunit/TrussLoad;
 */
jobject JNICALL Java_fermanext_trussunit_TrussLoadCase_findLoad
  (JNIEnv* env, jobject self, jobject jNode)
{
    LOG4CXX_DEBUG( logger, "findLoad(Node)" );
    TrussUnit::LoadCase* trussLoadCase = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, self );

    if ( trussLoadCase == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCase instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode* node = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussNode>( env, jNode );
    if ( node == 0 ) {
        // Node is not valid
        LOG4CXX_WARN( logger, "TrussNode is not valid" );
        return 0;
    }

    TrussLoad* trussLoad = trussLoadCase->findLoad( *node );
    if ( trussLoad == 0 )
        // Nothing was found
        return 0;

    // Register argument
    return JavaPluginArgumentRegistrator::registerArgument( 
                            trussLoad, env, "fermanext/trussunit/TrussLoad" );
}

/*
 * Class:     fermanext_trussunit_TrussLoadCase
 * Method:    countLoads
 * Signature: ()I
 */
jint JNICALL Java_fermanext_trussunit_TrussLoadCase_countLoads
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countLoads()" );
    TrussUnit::LoadCase* trussLoadCase = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussUnit::LoadCase>( env, self );
    if ( trussLoadCase == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoadCase instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return trussLoadCase->countLoads();
}

/*****************************************************************************/
