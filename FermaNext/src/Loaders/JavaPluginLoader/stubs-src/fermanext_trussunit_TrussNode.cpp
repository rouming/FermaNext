
#include "include/fermanext_trussunit_TrussNode.h"

#include "TrussUnit.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussNode") );

/*****************************************************************************/

TrussNode* getTrussNodeByUUID ( JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "getTrussNode" );

    jclass trussInstCls = env->GetObjectClass( self );
    Q_ASSERT( trussInstCls );

    jmethodID trussUUIDMethod = env->GetMethodID( trussInstCls, "getUUID",
                                                  "()Ljava/lang/String;" );
    Q_ASSERT( trussUUIDMethod );

    jstring jUuid = (jstring)env->CallObjectMethod( self, trussUUIDMethod );
    Q_ASSERT( jUuid );

     // Get chars
    const char* uuidChars = env->GetStringUTFChars( jUuid, 0 );

    QString uuid = QString::fromUtf8( uuidChars );

    // Free chars
    env->ReleaseStringUTFChars( jUuid, uuidChars );
    
    UUIDObject* truss = JavaPluginArgumentRegistrator::getRegistered( uuid );
    // Try to cast
    return dynamic_cast<TrussNode*>(truss);
}

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussNode_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    return (trussNode == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    setFixation
 * Signature: (Lfermanext/trussunit/TrussNode$Fixation;)V
 */
void JNICALL Java_fermanext_trussunit_TrussNode_setFixation
  (JNIEnv* env, jobject self, jobject jFix)
{
    LOG4CXX_DEBUG( logger, "setFixation" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    jclass fixClass = env->GetObjectClass( jFix );
    Q_ASSERT( fixClass );

    jmethodID ordinalMethod = env->GetMethodID( fixClass, "ordinal", "()I" );
    jint order = env->CallIntMethod( jFix, ordinalMethod );

    switch ( order ) {
    case TrussNode::Unfixed:
        trussNode->setFixation( TrussNode::Unfixed );
        break;
    case TrussNode::FixationByX:
        trussNode->setFixation( TrussNode::FixationByX );
        break;
    case TrussNode::FixationByY:
        trussNode->setFixation( TrussNode::FixationByY );
        break;
    case TrussNode::FixationByXY:
        trussNode->setFixation( TrussNode::FixationByXY );
        break;
    default:
        LOG4CXX_WARN( logger, QString("Unknown fixation type %1").
                                arg(order).toStdString() );
    }  
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    getFixation
 * Signature: ()Lfermanext/trussunit/TrussNode$Fixation;
 */
jobject JNICALL Java_fermanext_trussunit_TrussNode_getFixation
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getFixation" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    jclass fixClass = env->FindClass("fermanext/trussunit/TrussNode$Fixation");
    Q_ASSERT( fixClass );

    jmethodID getEnumConstMethod = 
        env->GetMethodID( fixClass, "getEnumConstants", 
                          "()[Ljava/lang/Object;" );
    Q_ASSERT( getEnumConstMethod );

    jobjectArray enumConstants = 
        (jobjectArray)env->CallObjectMethod( fixClass, getEnumConstMethod );
    Q_ASSERT( enumConstants );

    jsize enumConstSize = env->GetArrayLength( enumConstants );

    if ( enumConstSize == 0 ||
         enumConstSize - 1 > TrussNode::FixationByXY ) {
        LOG4CXX_WARN( logger, "Wrong Java enum TrussNode.Fixation" );
        return 0;
    }

    jobject jFix = env->GetObjectArrayElement( enumConstants,
                                               trussNode->getFixation() );
    Q_ASSERT( env->ExceptionCheck() == false );
    Q_ASSERT( jFix );

    return jFix;    
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    setPoint
 * Signature: (Lfermanext/trussunit/DoublePoint;)V
 */
void JNICALL Java_fermanext_trussunit_TrussNode_setPoint__Lfermanext_trussunit_DoublePoint_2
  (JNIEnv* env, jobject self, jobject jPoint)
{
    LOG4CXX_DEBUG( logger, "setPoint(DoublePoint)" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    jclass pointClass = env->GetObjectClass( jPoint );
    Q_ASSERT( pointClass );

    jmethodID getXMethod = env->GetMethodID( pointClass, "getX", "()D" );
    Q_ASSERT( getXMethod );

    jmethodID getYMethod = env->GetMethodID( pointClass, "getY", "()D" );
    Q_ASSERT( getYMethod );

    jdouble x = env->CallDoubleMethod( jPoint, getXMethod );
    jdouble y = env->CallDoubleMethod( jPoint, getYMethod );
    
    trussNode->setPoint( x, y );

}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    setPoint
 * Signature: (DD)V
 */
void JNICALL Java_fermanext_trussunit_TrussNode_setPoint__DD
  (JNIEnv* env, jobject self, jdouble x, jdouble y)
{
    LOG4CXX_DEBUG( logger, "setPoint(double, double)" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    trussNode->setPoint( x, y );
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    getPoint
 * Signature: ()Lfermanext/trussunit/DoublePoint;
 */
jobject JNICALL Java_fermanext_trussunit_TrussNode_getPoint
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getPoint" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }

    jclass pointClass = env->FindClass("fermanext/trussunit/DoublePoint");
    Q_ASSERT( pointClass );

    jmethodID pointCtor = env->GetMethodID( pointClass, "<init>", "(DD)V" );
    Q_ASSERT( pointCtor );

    double x = trussNode->getX();
    double y = trussNode->getY();

    jobject jPoint = env->NewObject( pointClass, pointCtor, x, y );
    Q_ASSERT( jPoint );

    return jPoint;
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    getX
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussNode_getX
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getX" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return trussNode->getX();
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    getY
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussNode_getY
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getY" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return trussNode->getY();
}

/*
 * Class:     fermanext_trussunit_TrussNode
 * Method:    getNumber
 * Signature: ()I
 */
jint JNICALL Java_fermanext_trussunit_TrussNode_getNumber
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getNumber" );
    TrussNode* trussNode = getTrussNodeByUUID( env, self );
    if ( trussNode == 0 ) {
        LOG4CXX_WARN( logger, "TrussNode instance is 0, maybe it was not "
                              "properly registered?" );
        return -1;
    }
    return trussNode->getNumber();
}

/*****************************************************************************/
