
#ifndef JAVAVARIABLEREGISTRATOR_H
#define JAVAVARIABLEREGISTRATOR_H

#include "UUIDObject.h"

#include <jni.h>

/** Class is used for getting registered #UUIDObject from jni side */
class JavaVariableRegistrator
{
public:
    /** Registers variable */
    static void registerVariable ( UUIDObject* );

    /** 
     * Registers varaible, creates Java object of specified class 
     * name and sets uuid.
     * Note: Java class should be assignable from fermanext.system.NativeObject
     * @param uuidObj native object to register
     * @param env Java environment
     * @param javaClsName name of Java class to create an object
     * @return 0 if smth goes wrong, valid Java object otherwise
     */
    static jobject registerVariable ( UUIDObject* uuidObj, JNIEnv* env, 
                                      const char* javaClsName );

    /** Unregisters variable */
    static bool unregisterVariable ( UUIDObject* );

    /** Unregisters all variables */
    static void unregisterAllVariables ();

    /** Checks if variables is registered */
    static bool isRegistered ( UUIDObject* )
    /** Checks if variables is registered */;
    static bool isRegistered ( const QString& uuid );

    /** Returns registered variable by uuid */
    static UUIDObject* getRegistered ( const QString& uuid );
};

/** 
 * Same as #JavaVariableRegistrator::getRegistered, 
 * but tries to cast to specified type
 * NOTE: we can't move this function to #JavaVariableRegistrator class
 *       and make it static method because of VC6.0 fatal internal compiler 
 *       error C1001.
 */
template <class T>
T* getRegisteredAndCast ( const QString& uuid )
{ return dynamic_cast<T*>( JavaVariableRegistrator::getRegistered(uuid) ); }

#endif //JAVAVARIABLEREGISTRATOR_H
