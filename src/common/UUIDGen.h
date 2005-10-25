
#ifndef UUIDGEN_H
#define UUIDGEN_H

#include <stdlib.h>
#include <qstring.h>
#include <qmutex.h>

class UUIDGen
{
public:    
    inline UUIDGen () { srand(time(0)); }

    /** 
     * Thread-safe static UUID random generation
     */
    static QString uuid ()
    {
        static UUIDGen gen;
        static QMutex mutex;
        mutex.lock();
        QString uuid = gen.generateUUID();
        mutex.unlock();
        return uuid;
    }    

    /**
     * Generates a random UUID and returns the QString representation of it.
     */
    inline QString generateUUID()
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

private:
    /**
     * Generates random bytes and places them into a user-supplied
     * byte array. The number of random bytes produced is equal
     * to the length of the byte array.
     */
    inline void nextRandomBytes( QByteArray& bytes)
    {
        uint numRequested = bytes.size();
        uint numGot = 0, randNumb = 0;
        while ( true ) {
            for ( int i = 0; i < 4; ++i ) {                           
                if (numGot == numRequested)
                    return;
                randNumb = (i==0 ? rand() : randNumb >> 8);
                bytes[numGot++] = (char)randNumb;
            }
        }
    }

private:
    static const QString hexChars;
    static const int INDEX_TYPE = 6;
    static const int INDEX_VARIATION = 8;
    static const int TYPE_RANDOM_BASED = 4;
};
const QString UUIDGen::hexChars = "0123456789abcdef";

#endif //UUIDGEN_H
