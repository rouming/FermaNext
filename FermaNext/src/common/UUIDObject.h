
#ifndef UUIDOBJECT_H
#define UUIDOBJECT_H

#include "UUIDGen.h"
#include <qregexp.h>

const QRegExp uuidValidator(
                            "([0-9A-Fa-f]{8})-"
                            "([0-9A-Fa-f]{4})-"
                            "([0-9A-Fa-f]{4})-"
                            "([0-9A-Fa-f]{4})-"
                            "([0-9A-Fa-f]{12})"
                            );


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

    inline void setUUID ( const QString& id ) throw (WrongUUIDException)
    { 
        if ( -1 == uuidValidator.search( id ) )
            throw WrongUUIDException();
        uuid = id;
    }

private:
    QString uuid;
};

#endif //UUIDOBJECT_H
