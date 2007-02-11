
#ifndef MD5GENERATOR_H
#define MD5GENERATOR_H

#include <QString>

class MD5Generator
{
public:
    /**
     * Generates MD5 sums for file or file in directory, if path is path
     * to directory.
     * @param path to file or directory with files for which MD5 sum should
     *        be generated.
     * @param recursion should we recursively find files for specified 
     *                  directory.
     * @return MD5 sums in xml format.
     */
    static QString md5ForFiles ( const QString& path, bool recursion = true );
};

#endif //MD5GENERATOR_H
