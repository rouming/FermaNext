
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

    Message = "Чтение из файла : " + fileName + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "Ферменная конструкция состоит из : " + 
        QString::number(Data.nodesNum) + " Узлов и " + 
        QString::number(Data.pivotsNum) + " Стержней\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "Количество случаев нагружения : " + 
        QString::number(Data.loadsNum) + "\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "Допускаемое Напряжение \n" + 
        QString::number(Data.stressLimit,'e',3) + "\n\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "Топология стержней : \n";
    OutFile.writeBlock(Message, Message.length());
    for (i=0;i<Data.pivotsNum;i++) {
        Message = QString::number(i+1) + " Стержень : " + 
            QString::number(Data.pivotsFirstNodes[i]) + " " +
            QString::number(Data.pivotsLastNodes[i]) + "\n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\nКоординаты Узлов : \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.nodesNum; i++) {
        Message = QString::number(i+1) + " Узел : [" + 
            QString::number(Data.x[i],'e',3) + "," + 
            QString::number(Data.y[i],'e',3) + "] \n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\nПеремещения узлов :\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.nodesNum; j++) {
            Message = 
                QString::number( j+1) + "Узел : [" + 
                QString::number(Data.xTrans[Data.nodesNum*i+j],'e',3) +
                " , " +    
                QString::number(Data.yTrans[Data.nodesNum*i+j],'e',3) + 
                "] \n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\nНапряжения в стержнях \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " Стержень : " + 
                QString::number(Data.stress[Data.pivotsNum*i+j],'e',3) + 
                "\n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\nЗапас прочности \n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.writeBlock(Message, Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " Стержень : " + 
                QString::number(Data.safetyFactor[Data.pivotsNum*i+j],'e',3) + "\n";
            OutFile.writeBlock(Message, Message.length());
        }
    }
    Message = "\nНачальные площади\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "Стержень : " + 
            QString::number(Data.pivotSquare[i],'e',3) + "\n";
        OutFile.writeBlock(Message, Message.length());
    }
    Message = "\nОбъём = " + QString::number(Data.v) + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "\nСиловой вес конструкции = " + 
        QString::number(Data.forceWeight) + "\n";
    OutFile.writeBlock(Message, Message.length());
    Message = "\nДлины Стержней : \n\n";
    OutFile.writeBlock(Message, Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "Стержень : " + 
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
    while (str.contains("Массив топологий стержней")==0) 
        ResFile.readLine( str, 256 );
    for (i = 0;i < Data.pivotsNum;i++) {
        ResFile.readLine(str,256);
        buff = str[0];
        Data.pivotsFirstNodes[i] = buff.toInt();
        buff = str[1];
        Data.pivotsLastNodes[i] = buff.toInt();
    }
//---------------------------------------------------------------------------//
    while ( str.contains("Массив координат узлов")==0) 
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
    while ( str.contains("Массив начальных площадей")==0) 
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
    while ( str.contains("Перемещения узлов")==0) 
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
    while ( str.contains("Напряжения в стержнях")==0) 
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
        /*while ( str.contains("Потребные площади стержней")==0) 
          ResFile.readLine(str,256);
          for (i = 0; i < Data.pivotsNum; i++) {
          ResFile.readLine(str,256);
          Data.pivotSquare[i] = str.toDouble();
          }*/
//---------------------------------------------------------------------------//
    return true;
}

/*****************************************************************************/
