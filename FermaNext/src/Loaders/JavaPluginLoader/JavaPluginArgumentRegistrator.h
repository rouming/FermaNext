
#ifndef JAVAPLUGINARGUMENTREGISTRATOR_H
#define JAVAPLUGINARGUMENTREGISTRATOR_H

#include <jni.h>

#include <QHash>
#include <QStack>

#include "UUIDObject.h"

/** 
 * Class registers arguments for Java plugins.
 * JNI sideo uses this class to get objects by uuids.
 */
class JavaPluginArgumentRegistrator
{
public:
    /** Creates new argument context in stack */
    static void push ();
    /** Returns to previous argument context */
    static void pop ();

    /** Registers argument in current argument context */
    static bool registerArgument ( UUIDObject* );

    /** 
     * Registers argument in current argument context and returns 
     * tries to create Java object by UUID
     * @param uuidObj native object to register
     * @param env Java environment
     * @param javaClsName name of Java class to create an object
     * @return 0 if smth goes wrong, valid Java object otherwise
     */
    static jobject registerArgument ( UUIDObject* uuidObj, JNIEnv* env, 
                                      const char* javaClsName );
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

// template static

    /** Same as above, but tries to cast */
    template <class T>
    static T* getRegisteredByJavaObjAndCast ( JNIEnv* env, jobject self )
    {
        UUIDObject* uuidObj = getRegisteredByJavaObj( env, self );
        return dynamic_cast<T*>( uuidObj );
    }

private:
    typedef QHash<QString, UUIDObject*> UUIDObjectMap;
    typedef QStack<UUIDObjectMap*> UUIDObjectMapStack;

    /** 
     * Returns current (or top) context of the stack
     * or 0 if it is empty.
     */
    static UUIDObjectMap* top ();

private:
    static UUIDObjectMapStack* argsStack;
};

#endif //JAVAPLUGINARGUMENTREGISTRATOR_H
