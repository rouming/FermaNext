
#include "VYVReader.h"
#include "TrussCalcData.h"
#include <qfile.h>
#include <cmath>
#include <qstring.h>

/*****************************************************************************
 * VYV Reader
 *****************************************************************************/

VYVReader::VYVReader ( TrussCalcData& data_ ) :
    Data(data_)
{} 

bool VYVReader::write ( const QString& fileName ) const
{
    QFile OutFile(fileName);
    QString Message;
    int i;
    int j;

    if ( ! OutFile.open(IO_WriteOnly) )
        return false;

    Message = "������ �� ����� : " + fileName + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "��������� ����������� ������� �� : " + 
        QString::number(Data.nodesNum) + " ����� � " + 
        QString::number(Data.pivotsNum) + " ��������\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "���������� ������� ���������� : " + 
        QString::number(Data.loadsNum) + "\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "����������� ���������� \n" + 
        QString::number(Data.stressLimit,'e',3) + "\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "��������� �������� : \n";
    OutFile.writeBlock(Message, Message.length());
    for (i=0;i<Data.pivotsNum;i++) {
        Message = QString::number(i+1) + " �������� : " + 
            QString::number(Data.pivotsFirstNodes[i]) + " " +
            QString::number(Data.pivotsLastNodes[i]) + "\n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\n���������� ����� : \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.nodesNum; i++) {
        Message = QString::number(i+1) + " ���� : [" + 
            QString::number(Data.x[i],'e',3) + "," + 
            QString::number(Data.y[i],'e',3) + "] \n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\n����������� ����� :\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "������ ���������� " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.nodesNum; j++) {
            Message = 
                QString::number( j+1) + "���� : [" + 
                QString::number(Data.xTrans[Data.nodesNum*i+j],'e',3) +
                " , " +    
                QString::number(Data.yTrans[Data.nodesNum*i+j],'e',3) + 
                "] \n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\n���������� � �������� \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "������ ���������� " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " �������� : " + 
                QString::number(Data.stress[Data.pivotsNum*i+j],'e',3) + 
                "\n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\n����� ��������� \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "������ ���������� " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " �������� : " + 
                QString::number(Data.safetyFactor[Data.pivotsNum*i+j],'e',3) + "\n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\n��������� �������\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "�������� : " + 
            QString::number(Data.pivotSquare[i],'e',3) + "\n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\n����� = " + QString::number(Data.v) + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "\n������� ��� ����������� = " + 
        QString::number(Data.forceWeight) + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "\n����� �������� : \n\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "�������� : " + 
            QString::number(Data.pivotLength[i],'e',3) + "\n";
        OutFile.writeBlock(Message, Message.length());
    }
    return true;
}

bool VYVReader::read ( const QString& fileName )
{
    QFile ResFile(fileName);
    QString str;
    QString buff;
    int i;
    int j;
    int pos = 24;
    int len;
    double length;
    double MaxStress;

    if ( ! ResFile.open(IO_ReadOnly) )
        return false;

    ResFile.readLine( str, 256 );
    ResFile.readLine( str, 256 );
    Data.pivotsNum = str.toInt();
    Data.pivotsFirstNodes.resize(Data.pivotsNum);
    Data.pivotsLastNodes.resize(Data.pivotsNum);
    Data.pivotLength.resize(Data.pivotsNum);
    Data.pivotSquare.resize(Data.pivotsNum);
    ResFile.readLine( str, 256 );
    Data.nodesNum = str.toInt();
    Data.x.resize(Data.nodesNum);
    Data.y.resize(Data.nodesNum);
    for ( i = 0; i < 3; i++) {
        ResFile.readLine( str, 256 );
    }
    Data.loadsNum = str.toInt();
    Data.xTrans.resize(Data.nodesNum * Data.loadsNum);
    Data.yTrans.resize(Data.nodesNum * Data.loadsNum);
    Data.stress.resize(Data.pivotsNum * Data.loadsNum);
    Data.safetyFactor.resize(Data.pivotsNum * Data.loadsNum);
    ResFile.readLine( str, 256 );
    Data.stressLimit = str.toDouble();
//---------------------------------------------------------------------------//
    while (str.contains("������ ��������� ��������")==0) 
        ResFile.readLine( str, 256 );
    for (i = 0;i < Data.pivotsNum;i++) {
        ResFile.readLine(str,256);
        buff = str[0];
        Data.pivotsFirstNodes[i] = buff.toInt();
        buff = str[1];
        Data.pivotsLastNodes[i] = buff.toInt();
    }
//---------------------------------------------------------------------------//
    while ( str.contains("������ ��������� �����")==0) 
        ResFile.readLine(str,256);
    for (i = 0;i < Data.nodesNum;i++) {
        ResFile.readLine(str,256);
        buff = str;
        len = str.length();
        str.remove(0,pos);
        buff.remove(pos,len - pos + 1);
        Data.x[i] = buff.toDouble();
        Data.y[i] = str.toDouble();
    }
//---------------------------------------------------------------------------//
    while ( str.contains("������ ��������� ��������")==0) 
        ResFile.readLine(str,256);
    for (i = 0; i < Data.pivotsNum; i++) {
        ResFile.readLine(str,256);
        Data.pivotSquare[i] = str.toDouble();
    }
//---------------------------------------------------------------------------//
    Data.v = 0;
    for (i = 0; i < Data.pivotsNum; i++) {
        length = sqrt( (Data.x[ Data.pivotsFirstNodes[i] -1] - 
                        Data.x[ Data.pivotsLastNodes[i] -1])*
                       (Data.x[ Data.pivotsFirstNodes[i] -1] - 
                        Data.x[ Data.pivotsLastNodes[i] -1])+
                       (Data.y[ Data.pivotsFirstNodes[i] -1] - 
                        Data.y[ Data.pivotsLastNodes[i] -1])*
                       (Data.y[ Data.pivotsFirstNodes[i] -1] - 
                        Data.y[ Data.pivotsLastNodes[i] -1]) );
        Data.pivotLength[i] = length;
        Data.v = Data.v + length * Data.pivotSquare[i];
    }
//---------------------------------------------------------------------------//
    while ( str.contains("����������� �����")==0) 
        ResFile.readLine( str, 256 );
    ResFile.readLine( str, 256 );
    for (i = 0; i < Data.loadsNum; i++) {
        for (j = 0; j < Data.nodesNum; j++) {
            ResFile.readLine(str,256 );
            buff = str;
            len = str.length();
            str.remove( 0, pos );
            buff.remove( pos, len - pos + 1 );
            Data.xTrans[Data.nodesNum*i+j] = buff.toDouble();
            Data.yTrans[Data.nodesNum*i+j] = str.toDouble();
        }
        ResFile.readLine( str, 256 );
    }
//---------------------------------------------------------------------------//
    while ( str.contains("���������� � ��������")==0) 
        ResFile.readLine( str, 256 );
    ResFile.readLine( str, 256 );
    for (i = 0; i < Data.loadsNum; i++) {
        for (j = 0; j < Data.pivotsNum; j++) {
            ResFile.readLine(str,256);
            Data.stress[Data.pivotsNum*i+j] = str.toDouble();;
            Data.safetyFactor[Data.pivotsNum*i+j] = 
                Data.stressLimit/Data.stress[Data.pivotsNum*i+j];
        }
        ResFile.readLine( str, 256 );
    }
//---------------------------------------------------------------------------//
    Data.forceWeight = 0;
    for (i = 0; i < Data.pivotsNum; i++) {
        MaxStress = 0;
        for (j = 0; j < Data.loadsNum; j++) {
            if (fabs(Data.stress[Data.pivotsNum*j+i]) > MaxStress)
                MaxStress = fabs(Data.stress[Data.pivotsNum*j+i]);
        }
        Data.forceWeight = Data.forceWeight + MaxStress*Data.pivotLength[i];
    }
//---------------------------------------------------------------------------//
        /*while ( str.contains("��������� ������� ��������")==0) 
          ResFile.readLine(str,256);
          for (i = 0; i < Data.pivotsNum; i++) {
          ResFile.readLine(str,256);
          Data.pivotSquare[i] = str.toDouble();
          }*/
//---------------------------------------------------------------------------//
    return true;
}

/*****************************************************************************/
