
#ifndef WIN_SIMPLECALC_H
#define WIN_SIMPLECALC_H

#include "PluginLoader.h"

const QString calcDLLName = "plugins/win_Simple_f.dll";
const QString calcProcName = "SimpleFerma";

class WinSimpleCalcPlugin : public SimpleCalcPlugin
{
public:
    WinSimpleCalcPlugin () :
        simpleCalc(0)
    {
        loader.loadLibrary( calcDLLName );
        simpleCalc = (SimpleFerma)loader.getProcAddress( calcProcName );
    }

    ~WinSimpleCalcPlugin ()
    {
        loader.freeLibrary();
    }

    void startCalculation ( const QString& fileName )
    {
        simpleCalc( fileName.ascii() );
    }

private:
    typedef void  ( StandardCall *SimpleFerma ) ( const char* );
    PluginLoader loader;
    SimpleFerma simpleCalc;
};

WinSimpleCalcPlugin plugin;

#endif //WIN_SIMPLECALC_H
