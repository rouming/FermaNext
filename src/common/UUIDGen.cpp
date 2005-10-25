/**
 * Author:      uk-dave (http://www.uk-dave.com)  
 * Description: Generates random-number based UUIDs
 * Useful Links:
 *         What is a UUID: http://www.dsps.net/uuid.html
 *         UUID Spec: http://www.opengroup.org/onlinepubs/9629399/apdxa.htm
 */

#include "UUIDGen.h"
#include <time.h>
#include <qmutex.h>

/*****************************************************************************
 * UUIDGen
 *****************************************************************************/

const QString UUIDGen::hexChars = "0123456789abcdef";
const int UUIDGen::INDEX_TYPE = 6;
const int UUIDGen::INDEX_VARIATION = 8;
const int UUIDGen::TYPE_RANDOM_BASED = 4;

/*****************************************************************************/

UUIDGen::UUIDGen () 
{ 
    setSeed(time(0));
}

QString UUIDGen::uuid ()
{
    static UUIDGen gen;
    static QMutex mutex;
    mutex.lock();
    QString uuid = gen.generateUUID();
    mutex.unlock();
    return uuid;
}    

QString UUIDGen::generateUUID () const
{
    // Generate 128-bit random number
    QByteArray uuid(16);
    nextRandomBytes(uuid);
        
    // Set various bits such as type
    uuid[INDEX_TYPE] &= (char) 0x0F;
    uuid[INDEX_TYPE] |= (char) (TYPE_RANDOM_BASED << 4);
    uuid[INDEX_VARIATION] &= (char) 0x3F;
    uuid[INDEX_VARIATION] |= (char) 0x80;
        
    // Convert byte array into a UUID formated string
    QString b;
    for ( int i = 0; i < 16; ++i ) {
        if ( i==4 || i==6 || i==8 || i==10 ) 
            b.append('-');
        int hex = uuid[i] & 0xFF;
        b.append( hexChars.at(hex >> 4) );
        b.append( hexChars.at(hex & 0x0F) );
    }        
    return b;
}

void UUIDGen::nextRandomBytes( QByteArray& bytes) const
{
    int numRequested = bytes.size();
    int numGot = 0, randNumb = 0;
    while ( true ) {
        for ( int i = 0; i < 4; ++i ) {                           
            if (numGot == numRequested)
                return;
            randNumb = (i==0 ? nextInt() : randNumb >> 8);
            bytes[numGot++] = (char)randNumb;
        }
    }
}

// POSIX  1003.1-2003 example
int UUIDGen::nextInt () const
{
  seed = seed * 1103515245 + 12345;
  return((unsigned)(seed/65536) % 32768) << 16;
}

void UUIDGen::setSeed ( int s )
{
    seed = s;
}

/*****************************************************************************/
