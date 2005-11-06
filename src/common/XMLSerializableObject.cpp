
#include "XMLSerializableObject.h"

/*****************************************************************************
 * XML Serializable Object
 *****************************************************************************/

XMLSerializableObject::XMLSerializableObject ()
{}

XMLSerializableObject::~XMLSerializableObject ()
{}

/**
 * Override this method in parent class to provide loading of the object 
 * from XML format.
 */
void XMLSerializableObject::loadFromXML ( const QDomElement& elem )
    throw (LoadException)
{
    if ( ! elem.hasAttribute( "uuid" ) )
        throw LoadException();

    try { 
        setUUID( elem.attribute( "uuid" ) ); 
    }
    catch ( UUIDObject::WrongUUIDException& ) {
        throw LoadException();
    }
}

/**
 * Override this method in parent class to provide saving of the object 
 * to XML format.
 */
QDomElement XMLSerializableObject::saveToXML ( QDomDocument& doc )
{
    QDomElement elem = doc.createElement( "XMLSerializableObject" );
    elem.setAttribute( "uuid", getUUID() );
    return elem;
}

/*****************************************************************************/
