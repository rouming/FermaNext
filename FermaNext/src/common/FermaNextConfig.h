
#ifndef FERMANEXTCONFIG_H
#define FERMANEXTCONFIG_H

#include <qstring.h>
#include <qdom.h>
#include <qmap.h>
#include <qvariant.h>

extern const QString CONFIG_FILENAME;

typedef QMap<QString, QVariant> ConfigItems;

class FermaNextConfig
{
public:
    // Some config exceptions
    class ConfigReadException  {};
    class ConfigWriteException {};
    class ConfigParseException {};        

    FermaNextConfig ( const QString& file = CONFIG_FILENAME ) throw (ConfigReadException, 
                                                                     ConfigWriteException);

    QDomNode findConfigNode ( const QString& name ) const;
    QDomNode findConfigNode ( const QDomNode& node, const QString& name ) const;
    ConfigItems getConfigItems ( const QDomNode& node ) const;

protected:
    void parseConfig () throw (ConfigReadException, ConfigParseException);

private:
    QString fileName;
    QDomDocument configDoc;

    static const QDomNode nullNode;
};


#endif //FERMANEXTCONFIG_H
