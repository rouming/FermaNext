
#ifndef UUIDOBJECT_H
#define UUIDOBJECT_H

#include <QUuid>

/**
 * UUID object provides basic functionality of getting and setting UUID.
 * This class should be the parent.
 */
class UUIDObject
{
protected:
    UUIDObject ();
    virtual ~UUIDObject ();

public:
    // Exceptions
    /** Occurs when UUID is not valid */
    class WrongUUIDException {};

    /** Returns UUID of this object */
    const QString& getUUID () const;

    /**
     * Sets UUID to this object
     * @throw UUIDObject::WrongUUIDException if UUID is not valid
     */
    void setUUID ( const QString& uuid ) /*throw (WrongUUIDException)*/;

private:
    QString uuid;
};

#endif //UUIDOBJECT_H
