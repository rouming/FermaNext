
#ifndef VYVREADER_H
#define VYVREADER_H

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
