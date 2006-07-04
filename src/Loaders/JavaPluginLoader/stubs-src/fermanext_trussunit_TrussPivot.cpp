
#include "include/fermanext_trussunit_TrussPivot.h"

#include "TrussUnit.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussPivot") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussPivot_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    return (pivot == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    getFirstNode
 * Signature: ()Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussPivot_getFirstNode
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getFirstNode()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode& node = pivot->getFirstNode();

    return JavaPluginArgumentRegistrator::registerArgument( 
                            &node, env, "fermanext/trussunit/TrussNode" );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    getLastNode
 * Signature: ()Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussPivot_getLastNode
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getLastNode()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode& node = pivot->getLastNode();

    return JavaPluginArgumentRegistrator::registerArgument( 
                            &node, env, "fermanext/trussunit/TrussNode" );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    setFirstNode
 * Signature: (Lfermanext/trussunit/TrussNode;)V
 */
void JNICALL Java_fermanext_trussunit_TrussPivot_setFirstNode
  (JNIEnv* env, jobject self, jobject jNode)
{
    LOG4CXX_DEBUG( logger, "setFirstNode()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
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
    pivot->setFirstNode( node );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    setLastNode
 * Signature: (Lfermanext/trussunit/TrussNode;)V
 */
void JNICALL Java_fermanext_trussunit_TrussPivot_setLastNode
  (JNIEnv* env, jobject self, jobject jNode)
{
    LOG4CXX_DEBUG( logger, "setLastNode()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
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
    pivot->setLastNode( node );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    getMaterial
 * Signature: ()Lfermanext/trussunit/TrussMaterial;
 */
jobject JNICALL Java_fermanext_trussunit_TrussPivot_getMaterial
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getMaterial()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    // Java doesn't have const declaration, so we use const_cast
    TrussMaterial* mat = const_cast<TrussMaterial*>( pivot->getMaterial() );
    if ( mat == 0 )
        return 0;
    return JavaPluginArgumentRegistrator::registerArgument( 
                              mat, env, "fermanext/trussunit/TrussMaterial" );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    setMaterial
 * Signature: (Lfermanext/trussunit/TrussMaterial;)V
 */
void JNICALL Java_fermanext_trussunit_TrussPivot_setMaterial
  (JNIEnv* env, jobject self, jobject jMat)
{
    LOG4CXX_DEBUG( logger, "setMaterial(TrussMaterial)" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    TrussMaterial* mat = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, jMat );
    if ( mat == 0 ) {
        // TrussMaterial is not valid
        LOG4CXX_WARN( logger, "TrussMaterial is not valid" );
        return;
    }
    pivot->setMaterial( mat );    
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    getThickness
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussPivot_getThickness
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getThickness()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return pivot->getThickness();
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    setThickness
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussPivot_setThickness
  (JNIEnv* env, jobject self, jdouble thickness)
{
    LOG4CXX_DEBUG( logger, "setThickness(double)" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }
    pivot->setThickness( thickness );
}

/*
 * Class:     fermanext_trussunit_TrussPivot
 * Method:    getNumber
 * Signature: ()I
 */
jint JNICALL Java_fermanext_trussunit_TrussPivot_getNumber
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getNumber()" );
    TrussPivot* pivot = JavaPluginArgumentRegistrator::
        getRegisteredByJavaObjAndCast<TrussPivot>( env, self );
    if ( pivot == 0 ) {
        LOG4CXX_WARN( logger, "TrussPivot instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return pivot->getNumber();
}

/*****************************************************************************/
