
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

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    findNodeByCoord
 * Signature: (Lfermanext/trussunit/DoublePoint;)Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_findNodeByCoord
  (JNIEnv* env, jobject self, jobject jDoublePoint)
{
    LOG4CXX_DEBUG( logger, "findNodeByCoord(DoublePoint)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }    
    jclass pointCls = env->GetObjectClass( jDoublePoint );
    Q_ASSERT(pointCls);

    jmethodID getXMethod = env->GetMethodID( pointCls, "getX", "()D" );
    Q_ASSERT(getXMethod);
    jmethodID getYMethod = env->GetMethodID( pointCls, "getY", "()D" );
    Q_ASSERT(getYMethod);

    double x = env->CallDoubleMethod( jDoublePoint, getXMethod );
    double y = env->CallDoubleMethod( jDoublePoint, getYMethod );

    TrussNode* node = truss->findNodeByCoord( DoublePoint(x, y) );
    if ( node == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                                  node, env, "fermanext/trussunit/TrussNode" );
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    findNodeByNumber
 * Signature: (I)Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_findNodeByNumber
  (JNIEnv* env, jobject self, jint num)
{
    LOG4CXX_DEBUG( logger, "findNodeByNumber(int)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }
    TrussNode* node = truss->findNodeByNumber( num );
    if ( node == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                                  node, env, "fermanext/trussunit/TrussNode" );
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    findPivotByNumber
 * Signature: (I)Lfermanext/trussunit/TrussPivot;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_findPivotByNumber
  (JNIEnv* env, jobject self, jint num)
{
    LOG4CXX_DEBUG( logger, "findPivotByNumber(int)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }
    TrussPivot* pivot = truss->findPivotByNumber( num );
    if ( pivot == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                                pivot, env, "fermanext/trussunit/TrussPivot" );
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    findPivotByNodes
 * Signature: (Lfermanext/trussunit/TrussNode;Lfermanext/trussunit/TrussNode;)Lfermanext/trussunit/TrussPivot;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_findPivotByNodes
  (JNIEnv* env, jobject self, jobject jFirstNode, jobject jLastNode)
{
    LOG4CXX_DEBUG( logger, "findPivotByNodes(TrussNode, TrussNode)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode* firstNode = 
        getRegisteredByJavaObjAndCast<TrussNode>( env, jFirstNode );
    if ( firstNode == 0 ) {
        LOG4CXX_WARN( logger, "first TrussNode instance is invalid" );
        return 0;
    }
    
    TrussNode* lastNode = 
        getRegisteredByJavaObjAndCast<TrussNode>( env, jLastNode );
    if ( lastNode == 0 ) {
        LOG4CXX_WARN( logger, "last TrussNode instance is invalid" );
        return 0;
    }

    TrussPivot* pivot = truss->findPivotByNodes( *firstNode, *lastNode );
    if ( pivot == 0 )
        return 0;

    return JavaPluginArgumentRegistrator::registerArgument( 
                                pivot, env, "fermanext/trussunit/TrussPivot" );
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    findAdjoiningPivots
 * Signature: (Lfermanext/trussunit/TrussNode;)[Lfermanext/trussunit/TrussPivot;
 */
jobjectArray JNICALL Java_fermanext_trussunit_TrussUnit_findAdjoiningPivots
  (JNIEnv* env, jobject self, jobject jNode)
{
    LOG4CXX_DEBUG( logger, "findAdjoiningPivots(TrussNode)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode* node = getRegisteredByJavaObjAndCast<TrussNode>( env, jNode );
    if ( node == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is invalid" );
        return 0;
    }
    
    TrussUnit::PivotList pivots = truss->findAdjoiningPivots( *node );

    jclass pivotCls = env->FindClass( "fermanext/trussunit/TrussPivot" );
    Q_ASSERT( pivotCls );

    jobjectArray jArray = env->NewObjectArray( pivots.size(), pivotCls, 0 );
    Q_ASSERT( jArray );
    
    for ( uint i = 0; i < pivots.size(); ++i ) {
        TrussPivot* pivot = pivots.at(i);
        jobject jPivot = JavaPluginArgumentRegistrator::registerArgument( 
                                pivot, env, "fermanext/trussunit/TrussPivot" );
        env->SetObjectArrayElement( jArray, i, jPivot );
    }
    return jArray;
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    createNode
 * Signature: (Lfermanext/trussunit/DoublePoint;)Lfermanext/trussunit/TrussNode;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_createNode__Lfermanext_trussunit_DoublePoint_2
  (JNIEnv* env, jobject self, jobject jDoublePoint)
{
    LOG4CXX_DEBUG( logger, "createNode(DoublePoint)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }    
    jclass pointCls = env->GetObjectClass( jDoublePoint );
    Q_ASSERT(pointCls);

    jmethodID getXMethod = env->GetMethodID( pointCls, "getX", "()D" );
    Q_ASSERT(getXMethod);
    jmethodID getYMethod = env->GetMethodID( pointCls, "getY", "()D" );
    Q_ASSERT(getYMethod);

    double x = env->CallDoubleMethod( jDoublePoint, getXMethod );
    double y = env->CallDoubleMethod( jDoublePoint, getYMethod );

    TrussNode& node = truss->createNode( DoublePoint(x, y) );

    return JavaPluginArgumentRegistrator::registerArgument( 
                                 &node, env, "fermanext/trussunit/TrussNode" );
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

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    createPivot
 * Signature: (Lfermanext/trussunit/TrussNode;Lfermanext/trussunit/TrussNode;)Lfermanext/trussunit/TrussPivot;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_createPivot
  (JNIEnv* env, jobject self, jobject jFirstNode, jobject jLastNode)
{
    LOG4CXX_DEBUG( logger, "createPivot(TrussNode, TrussNode)" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussNode* firstNode = 
        getRegisteredByJavaObjAndCast<TrussNode>( env, jFirstNode );
    if ( firstNode == 0 ) {
        LOG4CXX_WARN( logger, "first TrussNode instance is invalid" );
        return 0;
    }
    
    TrussNode* lastNode = 
        getRegisteredByJavaObjAndCast<TrussNode>( env, jLastNode );
    if ( lastNode == 0 ) {
        LOG4CXX_WARN( logger, "last TrussNode instance is invalid" );
        return 0;
    }

    TrussPivot& pivot = truss->createPivot( *firstNode, *lastNode );

    return JavaPluginArgumentRegistrator::registerArgument( 
                               &pivot, env, "fermanext/trussunit/TrussPivot" );
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    countNodes
 * Signature: ()I
 */
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

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    countPivots
 * Signature: ()I
 */
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
 * Method:    getNodeList
 * Signature: ()[Lfermanext/trussunit/TrussNode;
 */
jobjectArray JNICALL Java_fermanext_trussunit_TrussUnit_getNodeList
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getNodeList()" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussUnit::NodeList nodes = truss->getNodeList();

    jclass nodeCls = env->FindClass( "fermanext/trussunit/TrussNode" );
    Q_ASSERT( nodeCls );

    jobjectArray jArray = env->NewObjectArray( nodes.size(), nodeCls, 0 );
    Q_ASSERT( jArray );
    
    for ( uint i = 0; i < nodes.size(); ++i ) {
        TrussNode* node = nodes.at(i);
        jobject jNode = JavaPluginArgumentRegistrator::registerArgument( 
                                node, env, "fermanext/trussunit/TrussNode" );
        env->SetObjectArrayElement( jArray, i, jNode );
    }
    return jArray;
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    getPivotList
 * Signature: ()[Lfermanext/trussunit/TrussPivot;
 */
jobjectArray JNICALL Java_fermanext_trussunit_TrussUnit_getPivotList
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getPivotList()" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussUnit::PivotList pivots = truss->getPivotList();

    jclass pivotCls = env->FindClass( "fermanext/trussunit/TrussPivot" );
    Q_ASSERT( pivotCls );

    jobjectArray jArray = env->NewObjectArray( pivots.size(), pivotCls, 0 );
    Q_ASSERT( jArray );
    
    for ( uint i = 0; i < pivots.size(); ++i ) {
        TrussPivot* pivot = pivots.at(i);
        jobject jPivot = JavaPluginArgumentRegistrator::registerArgument( 
                                pivot, env, "fermanext/trussunit/TrussPivot" );
        env->SetObjectArrayElement( jArray, i, jPivot );
    }
    return jArray;

}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    getTrussAreaSize
 * Signature: ()Lfermanext/trussunit/DoubleSize;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_getTrussAreaSize
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getTrussAreaSize()" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    jclass sizeClass = env->FindClass( "fermanext/trussunit/DoubleSize" );
    Q_ASSERT( sizeClass );

    jmethodID ctor = env->GetMethodID( sizeClass, "<init>", "(DD)V" );
    Q_ASSERT( ctor );

    const DoubleSize& size = truss->getTrussSize();

    return env->NewObject( sizeClass, ctor, size.getWidth(), size.getHeight());
}

/*
 * Class:     fermanext_trussunit_TrussUnit
 * Method:    getLoadCases
 * Signature: ()Lfermanext/trussunit/TrussLoadCaseArray;
 */
jobject JNICALL Java_fermanext_trussunit_TrussUnit_getLoadCases
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getLoadCases()" );
    TrussUnit* truss =
        getRegisteredByJavaObjAndCast<TrussUnit>( env, self );
    if ( truss == 0 ) {
        LOG4CXX_WARN( logger, "TrussUnit instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    TrussUnit::LoadCases& loadCases = truss->getLoadCases();

    return JavaPluginArgumentRegistrator::registerArgument( 
                  &loadCases, env, "fermanext/trussunit/TrussLoadCaseArray" );
}

/*****************************************************************************/
