
#include "include/fermanext_trussunit_TrussMaterial.h"

#include "TrussMaterial.h"
#include "JavaPluginArgumentRegistrator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger(
                         "java.fermanext.trussunit.native_TrussMaterial") );

/*****************************************************************************/

jboolean JNICALL Java_fermanext_trussunit_TrussMaterial_isValid
  (JNIEnv* env, jobject self )
{
    LOG4CXX_DEBUG( logger, "isValid" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    return (trussMat == 0 ? false : true);
}

/*****************************************************************************/

/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    setMaterialName
 * Signature: (Ljava/lang/String;)V
 */
void JNICALL Java_fermanext_trussunit_TrussMaterial_setMaterialName
  (JNIEnv* env, jobject self, jstring jMatName)
{
    LOG4CXX_DEBUG( logger, "setMaterialName(String)" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }

    // Get chars
    const char* matNameChars = env->GetStringUTFChars( jMatName, 0 );

    trussMat->setMaterialName( QString::fromUtf8(matNameChars) );    

    // Free chars
    env->ReleaseStringUTFChars( jMatName, matNameChars );
}

/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    setWorkingStress
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussMaterial_setWorkingStress
  (JNIEnv* env, jobject self, jdouble stress)
{
    LOG4CXX_DEBUG( logger, "setWorkingStress(double)" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }
    trussMat->setWorkingStress( stress );
}

/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    setElasticityModule
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussMaterial_setElasticityModule
  (JNIEnv* env, jobject self, jdouble elMod)
{
    LOG4CXX_DEBUG( logger, "setElasticityModule(double)" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }
    trussMat->setElasticityModule( elMod );    
}

/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    setDensity
 * Signature: (D)V
 */
void JNICALL Java_fermanext_trussunit_TrussMaterial_setDensity
  (JNIEnv* env, jobject self, jdouble dens)
{
    LOG4CXX_DEBUG( logger, "setDensity(double)" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return;
    }
    trussMat->setDensity( dens );    
}


/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    getMaterialName
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL Java_fermanext_trussunit_TrussMaterial_getMaterialName
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getMaterialName()" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return 0;
    }
    return env->NewStringUTF( qPrintable(trussMat->getMaterialName()) );
}

/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    getWorkingStress
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussMaterial_getWorkingStress
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getWorkingStress()" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return trussMat->getWorkingStress();
}


/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    getElasticityModule
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussMaterial_getElasticityModule
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getElasticityModule()" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return trussMat->getElasticityModule();
}


/*
 * Class:     fermanext_trussunit_TrussMaterial
 * Method:    getDensity
 * Signature: ()D
 */
jdouble JNICALL Java_fermanext_trussunit_TrussMaterial_getDensity
  (JNIEnv* env, jobject self)
{
    LOG4CXX_DEBUG( logger, "getDensity()" );
    TrussMaterial* trussMat =
        getRegisteredByJavaObjAndCast<TrussMaterial>( env, self );
    if ( trussMat == 0 ) {
        LOG4CXX_WARN( logger, "TrussMaterial instance is 0, maybe it was not "
                              "properly registered?" );
        return 0.0;
    }
    return trussMat->getDensity();
}

/*****************************************************************************/
