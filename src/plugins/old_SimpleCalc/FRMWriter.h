
#ifndef FRMWRITER_H
#define FRMWRITER_H

class TrussTopology;
class QString;

class FRMWriter
{
public:
    FRMWriter ( const TrussTopology& );
    void write ( const QString& fileName );
private:
    const TrussTopology& truss;
};

#endif //FRMWRITER_H
