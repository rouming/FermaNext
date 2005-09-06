
#ifndef CAlCDATAREADER_H
#define CAlCDATAREADER_H

#include <qmemarray.h>
#include <qstring.h>

struct CalcData
{
    int NumNods;                      //�ڌތی،�Ռ��Ҍ� �Ì׌یތ�
    int NumPivots;                    //�ڌތی،�Ռ��Ҍ� ����Ռ��֌݌Ռ�
    int NumLoads;                     //�ڌތی،�Ռ��Ҍ� ���ی��ЌՌ� �݌Ќӌ���֌Ռ݌،� 
    double StressLimit;               //���ތߌ��ڌЌՌ܌ތ� �݌Ќߌ���֌Ռ݌،�
    QMemArray <int> PivotsFirstNod;   //�܌Ќ��،� �ߌՌ��Ҍ�� �݌ތ܌Ռ��ތ� �Ì׌یތ� �� ����Ռ��֌݌��
    QMemArray <int> PivotsSecondNod;  // "__" �Ҍ�ތ���� "__"
    QMemArray <double> X;             //�܌Ќ��،� X �ڌތތ��Ԍ،݌Ќ� �Ì׌یތ�
    QMemArray <double> Y;             //"__" Y "__"
    QMemArray <double> TransX;        //�܌Ќ��،� �ߌՌ��Ռ܌Ռ�Ռ݌،� �ߌ� X
    QMemArray <double> TransY;        //�܌Ќ��،� �ߌՌ��Ռ܌Ռ�Ռ݌،� �ߌ� Y
    QMemArray <double> Stress;        //�݌Ќߌ���֌Ռ݌،� �� ����Ռ��֌݌��
    QMemArray <double> StressReserve; //�׌ЌߌЌ� �����ތ�݌ތ���
    QMemArray <double> S;             //�ߌތ���Ռь݌�� �ߌیތ�ЌԌ� ����Ռ��֌݌Ռ�
    double V;                         //���ь���
    double Sg;                        //���،یތҌތ� �ҌՌ� �ڌތ݌�����ڌ�،�
    QMemArray <double> PivotLength;   //�Ԍی،݌� ����Ռ��֌݌�
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
