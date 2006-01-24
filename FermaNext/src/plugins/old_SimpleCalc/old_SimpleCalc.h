
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include "NativePlugin.h"
#include "CalculationInterface.h"
#include <qstring.h>

const QString fermaResExt = ".vyv";

class TrussCalcData;

class SimpleCalcPlugin : public CalculationInterface, 
                         public NativePlugin
{
public:
    SimpleCalcPlugin ( PluginManager& mng, const QString& path );
    virtual ~SimpleCalcPlugin ();

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;

    virtual void calculate ( TrussTopology&, TrussCalcData& ) const;
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
