
#ifndef MD5COMPARATOR_H
#define MD5COMPARATOR_H

#include <QDomDocument>

class MD5Comparator
{
public:
    /**
     * Compares two MD5 xmls. Result will be merge of two MD5 sums with
     * attributes: new, changed or deleted.
     */
    static QDomDocument md5Compare ( const QDomDocument& oldMD5,
                                     const QDomDocument& newMD5 );
};

#endif //MD5COMPARATOR_H
