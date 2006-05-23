
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include <QString>

#include "NativePlugin.h"

const QString fermaResExt = ".vyv";

class TrussCalcData;

class SimpleCalcPlugin : public NativePlugin
{
public:
    SimpleCalcPlugin ( PluginManager& mng, const QString& path );
    virtual ~SimpleCalcPlugin ();

    virtual void execute ( const QList<UUIDObject*>& )
        /*throw (WrongExecutionArgsException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;

    virtual void startCalculation ( const QString& fileName ) const = 0;

protected:
    // Manages temp file
    void createTempFile ();
    void destroyTempFile ();

    const QString& tempFileName () const;

private:
    QString tempFile;
};

#endif //OLD_SIMPLECALC_H
