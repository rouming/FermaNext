
#ifndef VYVREADER_H
#define VYVREADER_H

#include <QString>
#include <QDomElement>

#include "TrussCalcData.h"

class VYVReader
{
public:
    VYVReader ();

    /** Reads from *.vyv file to inner data buffer */
    bool read ( const QString& fileName );

    /**
     * Writes inner data buffer to file in vyv format,
     * or in xml format.
     */
    bool write ( const QString& fileName, bool inXML = true ) const;

    /** Returns inner data in xml string */
    QString toXMLString () const;

private:
    /** Returns inner data in xml */
    QDomElement toXML ( QDomDocument& ) const;

private:
    TrussCalcData Data;
};

#endif //VYVREADER_H
