/**
 * Author:      uk-dave (http://www.uk-dave.com)  
 * Description: Generates random-number based UUIDs
 * Useful Links:
 *         What is a UUID: http://www.dsps.net/uuid.html
 *         UUID Spec: http://www.opengroup.org/onlinepubs/9629399/apdxa.htm
 */

#ifndef UUIDGEN_H
#define UUIDGEN_H

#include <qstring.h>

class UUIDGen
{
public:    
    UUIDGen ();

    /** 
     * Thread-safe static UUID random generation
     */
    static QString uuid ();

    /**
     * Generates a random UUID and returns the QString representation of it.
     */
    QString generateUUID () const;

    /** 
     * Returns the next pseudorandom, uniformly distributed int 
     * value from this random number generator's sequence. 
     */
    int nextInt () const;

    /** 
     * Sets the seed of this random number generator
     */
    void setSeed ( int );

private:
    /**
     * Generates random bytes and places them into a user-supplied
     * byte array. The number of random bytes produced is equal
     * to the length of the byte array.
     */
    void nextRandomBytes( QByteArray& bytes) const;

private:
    static const QString hexChars;
    static const int INDEX_TYPE;
    static const int INDEX_VARIATION;
    static const int TYPE_RANDOM_BASED;

    mutable int seed;
};

#endif //UUIDGEN_H
