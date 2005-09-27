
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include "TrussTopology.h"
#include "FRMWriter.h"
#include "VYVReader.h"
#include <qstring.h>
#include <qfile.h>

const QString fermaResExt = ".vyv";

class SimpleCalcPlugin //: public Plugin
{
public:
    SimpleCalcPlugin ()
    { createTempFile(); }
    virtual ~SimpleCalcPlugin ()
    { destroyTempFile(); }

    void calculate ( TrussTopology& truss, CalcData& data )
    {
        FRMWriter frm(truss);
        frm.write( tempFileName() );
        startCalculation( tempFileName() );
        VYVReader vyv(data);
        vyv.read( tempFileName() + fermaResExt );
    }

    virtual void startCalculation ( const QString& fileName ) = 0;

protected:
    // Manages temp file
    void createTempFile ()
    {  tempFile = tmpnam(0); }

    void destroyTempFile () 
    { 
        QFile::remove( tempFile + fermaResExt );
        QFile::remove( tempFile ); 
    }

    const QString& tempFileName () const
    { return tempFile; }

private:
    QString tempFile;
};

#endif //OLD_SIMPLECALC_H
