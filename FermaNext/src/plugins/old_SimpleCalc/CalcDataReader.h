
#ifndef CAlCDATAREADER_H
#define CAlCDATAREADER_H

#include <qmemarray.h>
#include <qstring.h>

struct CalcData
{
    int NumNods;                      //���������� �����
    int NumPivots;                    //���������� ��������
    int NumLoads;                     //���������� ������� ���������� 
    double StressLimit;               //����������� ����������
    QMemArray <int> PivotsFirstNod;   //������ ������ ������� ����� � ��������
    QMemArray <int> PivotsSecondNod;  // "__" ������ "__"
    QMemArray <double> X;             //������ X ��������� �����
    QMemArray <double> Y;             //"__" Y "__"
    QMemArray <double> TransX;        //������ ����������� �� X
    QMemArray <double> TransY;        //������ ����������� �� Y
    QMemArray <double> Stress;        //���������� � ��������
    QMemArray <double> StressReserve; //����� ���������
    QMemArray <double> S;             //��������� ������� ��������
    double V;                         //�����
    double Sg;                        //������� ��� �����������
    QMemArray <double> PivotLength;   //����� �������
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
