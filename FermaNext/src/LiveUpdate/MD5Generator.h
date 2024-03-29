
#ifndef MD5GENERATOR_H
#define MD5GENERATOR_H

#include <QDomDocument>

class MD5Generator
{
public:
    /**
     * Generates MD5 sums for file or file in directory, if path is path
     * to directory.
     * @param path to file or directory with files for which MD5 sum should
     *        be generated.
     * @param exc except list. Files or dir names from this list are skipped 
     *        from result xml doc.
     * @param recursion should we recursively find files for specified 
     *                  directory.
     * @return MD5 sums in xml format.
     */
    static QDomDocument md5ForFiles ( const QString& path, 
                                      const QStringList& exc = QStringList(),
                                      bool recursion = true );
};

#endif //MD5GENERATOR_H
