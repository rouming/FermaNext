
#include "include/fermanext_trussunit_TrussUnit.h"

#include "TrussUnit.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussUnit") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussUnit_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    return (truss == 0 ? false : true);
}

/*****************************************************************************/

jint JNICALL Java_fermanext_trussunit_TrussUnit_countNodes
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countNodes" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return truss->countNodes();
}

jint JNICALL Java_fermanext_trussunit_TrussUnit_countPivots
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "countPivots" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return truss->countPivots();
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    createNode
 * Signature: (DD)Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_createNode__DD
  (JNIEnv* env, jobject self, jdouble x, jdouble y)
{
    LOG4CXX_DEBUG( logger, "createNode(double, double)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode& node = truss->createNode( x, y );
    // Register argument
    return JavaPluginArgumentRegistrator::registerArgument( 
                                 &node, env, "fermanext/trussunit/TrussNode" );
}

/*****************************************************************************/
