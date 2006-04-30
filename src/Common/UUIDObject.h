
#ifndef UUIDOBJECT_H
#define UUIDOBJECT_H

#include "UUIDGen.h"

/**
 * UUID object provides basic functionality of getting and setting UUID.
 * This class should be the parent.
 */
class UUIDObject
{
protected:
    inline UUIDObject ()  
    { uuid = UUIDGen::uuid(); }

public:
    // Exceptions
    class WrongUUIDException {};

    inline const QString& getUUID () const
    { return uuid; }

    inline void setUUID ( const QString& id ) /*throw (WrongUUIDException)*/
    { 
        if ( ! UUIDGen::isValidUUID( id ) )
            throw WrongUUIDException();
        uuid = id;
    }

private:
    QString uuid;
};

#endif //UUIDOBJECT_H
