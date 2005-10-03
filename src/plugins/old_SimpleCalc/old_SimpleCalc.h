
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include "Plugin.h"
#include <qstring.h>

const QString fermaResExt = ".vyv";

class TrussTopology;
class TrussCalcData;

class SimpleCalcPlugin : public Plugin
{
public:
    SimpleCalcPlugin ();
    virtual ~SimpleCalcPlugin ();
    

    const PluginInfo& pluginInfo () const;
    PluginType pluginType () const;
    bool dependsOn ( PluginType ) const;
    void calculate ( TrussTopology&, TrussCalcData& ) const;

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
