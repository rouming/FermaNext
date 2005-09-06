
#include "CalcDataReader.h"
#include <qfile.h>
#include <cmath>

CalcDataReader::CalcDataReader( const QString &filename )
{
    File = filename;
    while ( readCalcData() == FALSE );
} 

CalcDataReader::~CalcDataReader()
{}

void CalcDataReader::output( const QString &outfilename ) const
{
    QFile OutFile(outfilename);
    QString Message;
    int i;
    int j;

    if (OutFile.open(IO_WriteOnly)) {
        Message = "������ �� ����� : " + File + "\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "��������� ����������� ������� �� : " + 
                  QString::number(Data.NumNods) + " ����� � " + 
                  QString::number(Data.NumPivots) + " ��������\n\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "���������� ������� ���������� : " + 
                  QString::number(Data.NumLoads) + "\n\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "����������� ���������� \n" + 
                  QString::number(Data.StressLimit,'e',3) + "\n\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "��������� �������� : \n";
        OutFile.writeBlock(Message, Message.length());
        for (i=0;i<Data.NumPivots;i++) {
            Message = QString::number(i+1) + " �������� : " + 
                      QString::number(Data.PivotsFirstNod[i]) + " " +
                      QString::number(Data.PivotsSecondNod[i]) + "\n";
            OutFile.writeBlock(Message, Message.length());
        }
        Message = "\n���������� ����� : \n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumNods; i++) {
            Message = QString::number(i+1) + " ���� : [" + 
                      QString::number(Data.X[i],'e',3) + "," + 
                      QString::number(Data.Y[i],'e',3) + "] \n";
            OutFile.writeBlock(Message, Message.length());
        }
        Message = "\n����������� ����� :\n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumLoads; i++) {
            Message = "������ ���������� " + QString::number(i+1) + "\n";
            OutFile.writeBlock(Message, Message.length());
            for (j = 0; j < Data.NumNods; j++) {
                Message = 
                    QString::number( j+1) + "���� : [" + 
                    QString::number(Data.TransX[Data.NumNods*i+j],'e',3) +
                    " , " +    
                    QString::number(Data.TransY[Data.NumNods*i+j],'e',3) + 
                    "] \n";
                OutFile.writeBlock(Message, Message.length());
            }
        }
        Message = "\n���������� � �������� \n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumLoads; i++) {
            Message = "������ ���������� " + QString::number(i+1) + "\n";
            OutFile.writeBlock(Message, Message.length());
            for (j = 0; j < Data.NumPivots; j++) {
                Message = 
                    QString::number( j+1) + " �������� : " + 
                    QString::number(Data.Stress[Data.NumPivots*i+j],'e',3) + 
                    "\n";
                OutFile.writeBlock(Message, Message.length());
            }
        }
        Message = "\n����� ��������� \n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumLoads; i++) {
            Message = "������ ���������� " + QString::number(i+1) + "\n";
            OutFile.writeBlock(Message, Message.length());
            for (j = 0; j < Data.NumPivots; j++) {
                Message = 
                    QString::number( j+1) + " �������� : " + 
                    QString::number(Data.StressReserve[Data.NumPivots*i+j],'e',3) + "\n";
                OutFile.writeBlock(Message, Message.length());
            }
        }
        Message = "\n��������� �������\n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumPivots; i++) {
            Message = QString::number(i+1) + "�������� : " + 
                      QString::number(Data.S[i],'e',3) + "\n";
            OutFile.writeBlock(Message, Message.length());
        }
        Message = "\n����� = " + QString::number(Data.V) + "\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "\n������� ��� ����������� = " + 
                  QString::number(Data.Sg) + "\n";
        OutFile.writeBlock(Message, Message.length());
        Message = "\n����� �������� : \n\n";
        OutFile.writeBlock(Message, Message.length());
        for (i = 0; i < Data.NumPivots; i++) {
            Message = QString::number(i+1) + "�������� : " + 
                      QString::number(Data.PivotLength[i],'e',3) + "\n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
}

bool CalcDataReader::readCalcData()
{
    QFile ResFile(File);
    QString str;
    QString buff;
    int i;
    int j;
    int pos = 24;
    int len;
    double length;
    double MaxStress;
    if (ResFile.open(IO_ReadOnly)) {
        ResFile.readLine( str, 256 );
        ResFile.readLine( str, 256 );
        Data.NumPivots = str.toInt();
        Data.PivotsFirstNod.resize(Data.NumPivots);
        Data.PivotsSecondNod.resize(Data.NumPivots);
        Data.PivotLength.resize(Data.NumPivots);
        Data.S.resize(Data.NumPivots);
        ResFile.readLine( str, 256 );
        Data.NumNods = str.toInt();
        Data.X.resize(Data.NumNods);
        Data.Y.resize(Data.NumNods);
        for ( i = 0; i < 3; i++) {
            ResFile.readLine( str, 256 );
        }
        Data.NumLoads = str.toInt();
        Data.TransX.resize(Data.NumNods * Data.NumLoads);
        Data.TransY.resize(Data.NumNods * Data.NumLoads);
        Data.Stress.resize(Data.NumPivots * Data.NumLoads);
        Data.StressReserve.resize(Data.NumPivots * Data.NumLoads);
        ResFile.readLine( str, 256 );
        Data.StressLimit = str.toDouble();
//---------------------------------------------------------------------------//
        while (str.contains("������ ��������� ��������")==0) 
            ResFile.readLine( str, 256 );
        for (i = 0;i < Data.NumPivots;i++) {
            ResFile.readLine(str,256);
            buff = str[0];
            Data.PivotsFirstNod[i] = buff.toInt();
            buff = str[1];
            Data.PivotsSecondNod[i] = buff.toInt();
        }
//---------------------------------------------------------------------------//
        while ( str.contains("������ ��������� �����")==0) 
            ResFile.readLine(str,256);
        for (i = 0;i < Data.NumNods;i++) {
            ResFile.readLine(str,256);
            buff = str;
            len = str.length();
            str.remove(0,pos);
            buff.remove(pos,len - pos + 1);
            Data.X[i] = buff.toDouble();
            Data.Y[i] = str.toDouble();
        }
//---------------------------------------------------------------------------//
        while ( str.contains("������ ��������� ��������")==0) 
            ResFile.readLine(str,256);
        for (i = 0; i < Data.NumPivots; i++) {
            ResFile.readLine(str,256);
            Data.S[i] = str.toDouble();
        }
//---------------------------------------------------------------------------//
        Data.V = 0;
        for (i = 0; i < Data.NumPivots; i++) {
            length = sqrt( (Data.X[ Data.PivotsFirstNod[i] -1] - 
                            Data.X[ Data.PivotsSecondNod[i] -1])*
                           (Data.X[ Data.PivotsFirstNod[i] -1] - 
                            Data.X[ Data.PivotsSecondNod[i] -1])+
                           (Data.Y[ Data.PivotsFirstNod[i] -1] - 
                            Data.Y[ Data.PivotsSecondNod[i] -1])*
                           (Data.Y[ Data.PivotsFirstNod[i] -1] - 
                            Data.Y[ Data.PivotsSecondNod[i] -1]) );
            Data.PivotLength[i] = length;
            Data.V = Data.V + length * Data.S[i];
        }
//---------------------------------------------------------------------------//
        while ( str.contains("����������� �����")==0) 
            ResFile.readLine( str, 256 );
        ResFile.readLine( str, 256 );
        for (i = 0; i < Data.NumLoads; i++) {
            for (j = 0; j < Data.NumNods; j++) {
                ResFile.readLine(str,256 );
                buff = str;
                len = str.length();
                str.remove( 0, pos );
                buff.remove( pos, len - pos + 1 );
                Data.TransX[Data.NumNods*i+j] = buff.toDouble();
                Data.TransY[Data.NumNods*i+j] = str.toDouble();
            }
            ResFile.readLine( str, 256 );
        }
//---------------------------------------------------------------------------//
        while ( str.contains("���������� � ��������")==0) 
            ResFile.readLine( str, 256 );
        ResFile.readLine( str, 256 );
        for (i = 0; i < Data.NumLoads; i++) {
            for (j = 0; j < Data.NumPivots; j++) {
                ResFile.readLine(str,256);
                Data.Stress[Data.NumPivots*i+j] = str.toDouble();;
                Data.StressReserve[Data.NumPivots*i+j] = 
                    Data.StressLimit/Data.Stress[Data.NumPivots*i+j];
            }
            ResFile.readLine( str, 256 );
        }
//---------------------------------------------------------------------------//
        Data.Sg = 0;
        for (i = 0; i < Data.NumPivots; i++) {
            MaxStress = 0;
            for (j = 0; j < Data.NumLoads; j++) {
                if (fabs(Data.Stress[Data.NumPivots*j+i]) > MaxStress)
                    MaxStress = fabs(Data.Stress[Data.NumPivots*j+i]);
            }
            Data.Sg = Data.Sg + MaxStress*Data.PivotLength[i];
        }
//---------------------------------------------------------------------------//
        /*while ( str.contains("��������� ������� ��������")==0) 
          ResFile.readLine(str,256);
          for (i = 0; i < Data.NumPivots; i++) {
          ResFile.readLine(str,256);
          Data.S[i] = str.toDouble();
          }*/
//---------------------------------------------------------------------------//
    }
    else 
        return FALSE;
    return TRUE;
}

CalcData CalcDataReader::getData() const
{
    return Data;
}
