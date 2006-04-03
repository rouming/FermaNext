
#ifndef WIN_SIMPLECALC_H
#define WIN_SIMPLECALC_H

#include "DynaLoader.h"

const QString calcDLLName = "plugins/win_Simple_f.dll";
const QString calcProcName = "SimpleFerma";

class os_dependent_SimpleCalcPlugin : public SimpleCalcPlugin
{
public:
    os_dependent_SimpleCalcPlugin ( PluginManager& mng, const QString& path ) :
        SimpleCalcPlugin( mng, path ),
        simpleCalc(0)
    {
        loader.loadLibrary( calcDLLName );
        simpleCalc = (SimpleFerma)loader.getProcAddress( calcProcName );
    }

    ~os_dependent_SimpleCalcPlugin ()
    {
        loader.freeLibrary();
    }

    void startCalculation ( const QString& fileName ) const
    {
        simpleCalc( fileName.toAscii() );
    }

private:
    typedef void  ( StandardCall *SimpleFerma ) ( const char* );
    DynaLoader loader;
    SimpleFerma simpleCalc;
};

#endif //WIN_SIMPLECALC_H
