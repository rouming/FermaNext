
#ifndef XMLSERIALIZABLEOBJECT_H
#define XMLSERIALIZABLEOBJECT_H

#include "UUIDObject.h"
#include <QDomElement>

class XMLSerializableObject : public UUIDObject
{
protected:
    XMLSerializableObject ();
    virtual ~XMLSerializableObject ();

public:
    // Exceptions
    class LoadException {};

protected:
    // Override this methods in parent class to provide object serialization.
    virtual void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    virtual QDomElement saveToXML ( QDomDocument& );
};

#endif //XMLSERIALIZABLEOBJECT_H
