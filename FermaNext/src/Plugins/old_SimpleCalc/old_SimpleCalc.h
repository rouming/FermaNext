
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

    virtual ExecutionResult specificExecute ( 
                              const PluginExecutionParams& params,
                              const QList<UUIDObject*>& args,
                              const QHash< QString, QList<Plugin*> >& deps  )
        /*throw (WrongExecutionArgsException)*/;

    virtual const PluginInfo& pluginInfo () const;
    virtual Status pluginStatusCode () const;
    virtual QString pluginStatusMsg () const;
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const;

private:
    void startCalculation ( const QString& fileNameIn,
							const QString& fileNameOut ) const;

    // Manages temp file
    void createTempFile ();
    void destroyTempFile ();

    const QString& tempFileName () const;

private:
    QString tempFile;
};

#endif //OLD_SIMPLECALC_H
