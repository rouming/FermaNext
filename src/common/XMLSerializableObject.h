
#ifndef XMLSERIALIZABLEOBJECT_H
#define XMLSERIALIZABLEOBJECT_H

#include "UUIDObject.h"
#include <qdom.h>

class XMLSerializableObject : public UUIDObject
{
protected:
    XMLSerializableObject ();
    virtual ~XMLSerializableObject ();

public:
    // Excetpions
    class LoadException {};

    // Override this methods in parent class to provide object serialization.
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual void saveToXML ( QDomElement& );
};

#endif //XMLSERIALIZABLEOBJECT_H
