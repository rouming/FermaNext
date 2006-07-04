
#include "UUIDObject.h"

/*****************************************************************************
 * UUIDObject
 *****************************************************************************/

UUIDObject::UUIDObject () :
    uuid( QUuid::createUuid().toString() )
{}

UUIDObject::~UUIDObject ()
{}

const QString& UUIDObject::getUUID () const
{ return uuid; }

void UUIDObject::setUUID ( const QString& uuid_ ) 
    /*throw (UUIDObject::WrongUUIDException)*/
{
    if ( QUuid(uuid_).isNull() )
        throw WrongUUIDException();
    uuid = uuid_;
}

/*****************************************************************************/
