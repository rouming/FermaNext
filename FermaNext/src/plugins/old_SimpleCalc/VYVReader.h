
#ifndef VYVREADER_H
#define VYVREADER_H

#include <qmemarray.h>
#include <qstring.h>

struct CalcData
{
    int nodesNum;                     //nodes number
    int pivotsNum;                    //pivots number
    int loadsNum;                     //load cases number
    double v;                         //volume
    double stressLimit;               //stress limit    
    double forceWeight;               //truss force weight
    QMemArray<int> pivotsFirstNodes;  //first nodes list of pivots
    QMemArray<int> pivotsLastNodes;   //last nodes list of pivots
    QMemArray<double> x;              //list of x nodes coord
    QMemArray<double> y;              //list of y nodes coord
    QMemArray<double> xTrans;         //list of nodes transference by x
    QMemArray<double> yTrans;         //list of nodes transference by y
    QMemArray<double> stress;         //list of pivots stresses
    QMemArray<double> safetyFactor;   //list of pivots safety factor
    QMemArray<double> pivotSquare;    //list of pivots square
    QMemArray<double> pivotLength;    //list of pivots length
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
