
#ifndef VYVREADER_H
#define VYVREADER_H

class QString;
class TrussCalcData;

class VYVReader
{
public:
    VYVReader ( TrussCalcData& );
    bool read ( const QString& fileName );
    bool write ( const QString& fileName ) const;

private:
    TrussCalcData& Data;
};

#endif //VYVREADER_H
