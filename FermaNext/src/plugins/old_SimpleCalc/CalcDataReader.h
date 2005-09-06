
#ifndef CAlCDATAREADER_H
#define CAlCDATAREADER_H

#include <qmemarray.h>
#include <qstring.h>

struct CalcData
{
    int NumNods;                      //ŒÚŒŞŒÛŒØŒçŒÕŒáŒâŒÒŒŞ ŒÃŒ×ŒÛŒŞŒÒ
    int NumPivots;                    //ŒÚŒŞŒÛŒØŒçŒÕŒáŒâŒÒŒŞ ŒÁŒâŒÕŒàŒÖŒİŒÕŒÙ
    int NumLoads;                     //ŒÚŒŞŒÛŒØŒçŒÕŒáŒâŒÒŒŞ ŒÁŒÛŒãŒçŒĞŒÕŒÒ ŒİŒĞŒÓŒàŒãŒÖŒÕŒİŒØŒï 
    double StressLimit;               //Œ´ŒŞŒßŒãŒáŒÚŒĞŒÕŒÜŒŞŒÕ ŒİŒĞŒßŒàŒïŒÖŒÕŒİŒØŒÕ
    QMemArray <int> PivotsFirstNod;   //ŒÜŒĞŒáŒáŒØŒÒ ŒßŒÕŒàŒÒŒëŒå ŒİŒŞŒÜŒÕŒàŒŞŒÒ ŒÃŒ×ŒÛŒŞŒÒ ŒÒ ŒÁŒâŒÕŒàŒÖŒİŒïŒå
    QMemArray <int> PivotsSecondNod;  // "__" ŒÒŒâŒŞŒàŒëŒå "__"
    QMemArray <double> X;             //ŒÜŒĞŒáŒáŒØŒÒ X ŒÚŒŞŒŞŒàŒÔŒØŒİŒĞŒâ ŒÃŒ×ŒÛŒŞŒÒ
    QMemArray <double> Y;             //"__" Y "__"
    QMemArray <double> TransX;        //ŒÜŒĞŒáŒáŒØŒÒ ŒßŒÕŒàŒÕŒÜŒÕŒéŒÕŒİŒØŒÙ ŒßŒŞ X
    QMemArray <double> TransY;        //ŒÜŒĞŒáŒáŒØŒÒ ŒßŒÕŒàŒÕŒÜŒÕŒéŒÕŒİŒØŒÙ ŒßŒŞ Y
    QMemArray <double> Stress;        //ŒİŒĞŒßŒàŒïŒÖŒÕŒİŒØŒï ŒÒ ŒÁŒâŒÕŒàŒÖŒİŒïŒå
    QMemArray <double> StressReserve; //Œ×ŒĞŒßŒĞŒá Œ¿ŒàŒŞŒçŒİŒŞŒáŒâŒØ
    QMemArray <double> S;             //ŒßŒŞŒâŒàŒÕŒÑŒİŒëŒÕ ŒßŒÛŒŞŒéŒĞŒÔŒØ ŒÁŒâŒÕŒàŒÖŒİŒÕŒÙ
    double V;                         //Œ¾ŒÑŒêŒñŒÜ
    double Sg;                        //ŒÁŒØŒÛŒŞŒÒŒŞŒÙ ŒÒŒÕŒá ŒÚŒŞŒİŒáŒâŒàŒãŒÚŒæŒØŒØ
    QMemArray <double> PivotLength;   //ŒÔŒÛŒØŒİŒĞ ŒÁŒâŒÕŒàŒÖŒİŒï
};

class CalcDataReader
{
public:
    CalcDataReader ( const QString &filename );
    ~CalcDataReader ();
    bool readCalcData ();
    CalcData getData () const;
    void output ( const QString &outfilename ) const;

private:
    QString File;
    CalcData Data;
};

#endif //CAlCDATAREADER_H
