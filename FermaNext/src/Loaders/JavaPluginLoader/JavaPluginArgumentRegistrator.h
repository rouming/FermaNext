
#ifndef JAVAPLUGINARGUMENTREGISTRATOR_H
#define JAVAPLUGINARGUMENTREGISTRATOR_H

#include <QHash>

#include "UUIDObject.h"

/** 
 * Class registers arguments for Java plugins.
 * JNI sideo uses this class to get objects by uuids.
 */
class JavaPluginArgumentRegistrator
{
public:    
    /** Registers argument */
    static void registerArgument ( UUIDObject* );
    /** Unregisters argument */
    static void unregisterArgument ( UUIDObject* );
    /** Unregisters all arguments */
    static void unregisterAllArguments ();

    /** Checks if registered */
    static bool isRegistered ( UUIDObject* );
    /** Checks if registered */
    static bool isRegistered ( const QString& uuid );

    /** Returns registered object by uuid */
    static UUIDObject* getRegistered ( const QString& uuid );

private:
    static QHash<QString, UUIDObject*>* args;
};

#endif //JAVAPLUGINARGUMENTREGISTRATOR_H
