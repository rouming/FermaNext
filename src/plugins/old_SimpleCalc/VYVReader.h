
#ifndef VYVREADER_H
#define VYVREADER_H

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

class VYVReader
{
public:
    VYVReader ( const QString &filename );
    ~VYVReader ();
    bool readCalcData ();
    CalcData getData () const;
    void output ( const QString &outfilename ) const;

private:
    QString File;
    CalcData Data;
};

#endif //VYVREADER_H
