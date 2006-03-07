
#ifndef FERMANEXTCONFIG_H
#define FERMANEXTCONFIG_H

#include <QString>
#include <QDom>
#include <QMap>
#include <QVariant>

typedef QMap<QString, QVariant> ConfigItems;

class FermaNextConfig
{
public:
    // Some config exceptions
    class ConfigReadException  {};
    class ConfigWriteException {};
    class ConfigParseException {};        

    FermaNextConfig ( const QString& file ) 
        throw (ConfigReadException, ConfigWriteException);

    QDomNode findConfigNode ( const QString& name ) const;
    QDomNode findConfigNode ( const QDomNode& node, const QString& name ) const;
    ConfigItems getConfigItems ( const QDomNode& node ) const;
    ConfigItems getConfigItems ( const QString& name ) const;

protected:
    void parseConfig () throw (ConfigReadException, ConfigParseException);

private:
    static const QDomNode nullNode;

    QString fileName;
    QDomDocument configDoc;
};


#endif //FERMANEXTCONFIG_H
