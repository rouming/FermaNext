
#include "include/fermanext_trussunit_TrussLoad.h"

#include "TrussLoad.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussLoad") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussLoad_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    return (trussLoad == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    getXForce
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussLoad_getXForce
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getXForce()" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return trussLoad->getXForce();
}

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    getYForce
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussLoad_getYForce
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getYForce()" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return trussLoad->getYForce();
}


/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    setXForce
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussLoad_setXForce
  (JNIEnv* env, jobject self, jdouble xForce)
{
    LOG4CXX_DEBUG( logger, "setXForce(double)" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }
    trussLoad->setXForce( xForce );
}

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    setYForce
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussLoad_setYForce
  (JNIEnv* env, jobject self, jdouble yForce)
{
    LOG4CXX_DEBUG( logger, "setYForce(double)" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
     if ( trussLoad == 0 ) {
         LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                               "properly registered?" );
         return;
     }
     trussLoad->setYForce( yForce );
}

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    setForces
 * Signature: (DD)V
 */
void JNICALL Java_fermanext_trussunit_TrussLoad_setForces
  (JNIEnv* env, jobject self, jdouble xForce, jdouble yForce)
{
    LOG4CXX_DEBUG( logger, "setForces(double, double)" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                      "properly registered?" );
        return;
    }
    trussLoad->setForces( xForce, yForce );
}

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    isEnabled
 * Signature: ()Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoad_isEnabled
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "isEnabled()" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                              "properly registered?" );
        return false;
    }
    return trussLoad->isEnabled();
}

/*
 * Class:     fermanext_trussunit_TrussLoad
 * Method:    isRemoved
 * Signature: ()Z
 */
jboolean JNICALL Java_fermanext_trussunit_TrussLoad_isRemoved
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "isRemoved()" );
    TrussLoad* trussLoad = 
        getRegisteredByJavaObjAndCast<TrussLoad>( env, self );
    if ( trussLoad == 0 ) {
        LOG4CXX_WARN( logger, "TrussLoad instance is 0, maybe it was not "
                              "properly registered?" );
        return false;
    }
    return trussLoad->isRemoved();
}

/*****************************************************************************/
