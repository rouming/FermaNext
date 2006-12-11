
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

/*****************************************************************************
 * StringUUIDObject
 *****************************************************************************/

StringUUIDObject::StringUUIDObject ( const QString& str ) :
    dataStr(str)
{}

const QString& StringUUIDObject::getData () const
{ return dataStr; }

void StringUUIDObject::setData ( const QString& str )
{ dataStr = str; }

/*****************************************************************************/

