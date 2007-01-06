
#ifndef JAVAPLUGINARGUMENTREGISTRATOR_H
#define JAVAPLUGINARGUMENTREGISTRATOR_H

#include <jni.h>

#include <QHash>
#include <QPair>
#include <QStack>

#include "JavaVM/JavaVirtualMachine.h"
#include "UUIDObject.h"

/** 
 * Class registers arguments for Java plugins.
 * JNI side uses this class to get objects by uuids.
 */
class JavaPluginArgumentRegistrator
{
public:
    /** Creates new argument context in stack */
    static void push ();
    /** Returns to previous argument context */
    static void pop ();

    /** Registers argument in current argument context */
    static bool registerArgument ( UUIDObject*, bool shouldBeDeleted = false );

    /** 
     * Registers argument in current argument context,
     * creates Java object of specified class name and
     * sets uuid.
     * Note: Java class should be assignable from fermanext.system.NativeObject
     * @param uuidObj native object to register
     * @param env Java environment
     * @param javaClsName name of Java class to create an object
     * @return 0 if smth goes wrong, valid Java object otherwise
     */
    static jobject registerArgument ( UUIDObject* uuidObj, JNIEnv* env, 
                                      const char* javaClsName,
                                      bool shouldBeDeleted = false );

    /** Same as above */
    static JObject registerArgument ( UUIDObject* uuidObj, 
                                      JavaVirtualMachine& javaVM,
                                      const char* javaClsName,
                                      bool shouldBeDeleted = false );


    /** Unregisters argument from current argument context */
    static bool unregisterArgument ( UUIDObject* );
    /** Unregisters all arguments from current argument context */
    static void unregisterAllArguments ();

    /** Checks if registered in current argument context */
    static bool isRegistered ( UUIDObject* );
    /** Checks if registered in current argument context */
    static bool isRegistered ( const QString& uuid );

    /** Returns registered object by uuid from current argument context */
    static UUIDObject* getRegistered ( const QString& uuid );

    /** 
     * Returns registered object by Java object from current 
     * argument context 
     */
    static UUIDObject* getRegisteredByJavaObj ( JNIEnv* env, jobject self );

private:
    typedef QHash<QString, QPair<UUIDObject*, bool> > UUIDObjectMap;
    typedef QStack<UUIDObjectMap*> UUIDObjectMapStack;

    /** 
     * Returns current (or top) context of the stack
     * or 0 if it is empty.
     */
    static UUIDObjectMap* top ();

private:
    static UUIDObjectMapStack* argsStack;
};

/** 
 * Same as #JavaPluginArgumentRegistrator::getRegisteredByJavaObj, 
 * but tries to cast to specified type
 * NOTE: we can't move this function to #JavaPluginArgumentRegistrator class
 *       and make it static method because of VC6.0 fatal internal compiler 
 *       error C1001.
 */
template <class T>
T* getRegisteredByJavaObjAndCast ( JNIEnv* env, jobject self )
{
    UUIDObject* uuidObj = 
        JavaPluginArgumentRegistrator::getRegisteredByJavaObj( env, self );
    return dynamic_cast<T*>( uuidObj );
}

#endif //JAVAPLUGINARGUMENTREGISTRATOR_H
