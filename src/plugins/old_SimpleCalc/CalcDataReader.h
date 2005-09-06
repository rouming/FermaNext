
#ifndef CAlCDATAREADER_H
#define CAlCDATAREADER_H

#include <qmemarray.h>
#include <qstring.h>

struct CalcData
{
    int NumNods;                      //количество Узлов
    int NumPivots;                    //количество Стержней
    int NumLoads;                     //количество Случаев нагружения 
    double StressLimit;               //Допускаемое напряжение
    QMemArray <int> PivotsFirstNod;   //массив первых номеров Узлов в Стержнях
    QMemArray <int> PivotsSecondNod;  // "__" вторых "__"
    QMemArray <double> X;             //массив X координат Узлов
    QMemArray <double> Y;             //"__" Y "__"
    QMemArray <double> TransX;        //массив перемещений по X
    QMemArray <double> TransY;        //массив перемещений по Y
    QMemArray <double> Stress;        //напряжения в Стержнях
    QMemArray <double> StressReserve; //запас Прочности
    QMemArray <double> S;             //потребные площади Стержней
    double V;                         //Объём
    double Sg;                        //Силовой вес конструкции
    QMemArray <double> PivotLength;   //длина Стержня
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
