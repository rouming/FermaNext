
#ifndef FRMWRITER_H
#define FRMWRITER_H

#include "TrussUnit.h"

class FRMWriter
{
public:
    FRMWriter ( const TrussUnit& );
    void write ( const QString& fileName );
private:
    const TrussUnit& truss;
};

#endif //FRMWRITER_H
