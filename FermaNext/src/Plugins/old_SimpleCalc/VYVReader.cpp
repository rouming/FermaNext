
#include <cmath>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>

#include "VYVReader.h"
#include "TrussCalcData.h"

/*****************************************************************************
 * VYV Reader
 *****************************************************************************/

VYVReader::VYVReader ()
{} 

bool VYVReader::write ( const QString& fileName,
                        bool inXML ) const
{
    QFile OutFile(fileName);
    QString Message;
    int i;
    int j;

    if ( ! OutFile.open(QIODevice::WriteOnly) )
        return false;

    if ( inXML ) {
        QTextStream stream( &OutFile );

        QDomDocument doc;
        QDomNode xmlInstr = 
            doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF-8\"") );
        doc.insertBefore( xmlInstr, doc.firstChild() );

        doc.appendChild( toXML(doc) );
        doc.save( stream, 4 );
        return true;
    }

    Message = "Чтение из файла : " + fileName + "\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "Ферменная конструкция состоит из : " + 
        QString::number(Data.nodesNum) + " Узлов и " + 
        QString::number(Data.pivotsNum) + " Стержней\n\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "Количество случаев нагружения : " + 
        QString::number(Data.loadsNum) + "\n\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "Допускаемое Напряжение \n" + 
        QString::number(Data.stressLimit,'e',3) + "\n\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "Топология стержней : \n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i=0;i<Data.pivotsNum;i++) {
        Message = QString::number(i+1) + " Стержень : " + 
            QString::number(Data.pivotsFirstNodes[i]) + " " +
            QString::number(Data.pivotsLastNodes[i]) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
    }
    Message = "\nКоординаты Узлов : \n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.nodesNum; i++) {
        Message = QString::number(i+1) + " Узел : [" + 
            QString::number(Data.x[i],'e',3) + "," + 
            QString::number(Data.y[i],'e',3) + "] \n";
        OutFile.write(Message.toAscii().data(), Message.length());
    }
    Message = "\nПеремещения узлов :\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
        for (j = 0; j < Data.nodesNum; j++) {
            Message = 
                QString::number( j+1) + "Узел : [" + 
                QString::number(Data.xTrans[Data.nodesNum*i+j],'e',3) +
                " , " +    
                QString::number(Data.yTrans[Data.nodesNum*i+j],'e',3) + 
                "] \n";
            OutFile.write(Message.toAscii().data(), Message.length());
        }
    }
    Message = "\nНапряжения в стержнях \n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " Стержень : " + 
                QString::number(Data.stress[Data.pivotsNum*i+j],'e',3) + 
                "\n";
            OutFile.write(Message.toAscii().data(), Message.length());
        }
    }
    Message = "\nЗапас прочности \n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.loadsNum; i++) {
        Message = "Случай нагружения " + QString::number(i+1) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
        for (j = 0; j < Data.pivotsNum; j++) {
            Message = 
                QString::number( j+1) + " Стержень : " + 
                QString::number(
                          Data.safetyFactor[Data.pivotsNum*i+j],'e',3) + "\n";
            OutFile.write(Message.toAscii().data(), Message.length());
        }
    }
    Message = "\nНачальные площади\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "Стержень : " + 
            QString::number(Data.pivotSquare[i],'e',3) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
    }
    Message = "\nОбъём = " + QString::number(Data.v) + "\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "\nСиловой вес конструкции = " + 
        QString::number(Data.forceWeight) + "\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    Message = "\nДлины Стержней : \n\n";
    OutFile.write(Message.toAscii().data(), Message.length());
    for (i = 0; i < Data.pivotsNum; i++) {
        Message = QString::number(i+1) + "Стержень : " + 
            QString::number(Data.pivotLength[i],'e',3) + "\n";
        OutFile.write(Message.toAscii().data(), Message.length());
    }
    return true;
}

bool VYVReader::read ( const QString& fileName )
{
    QFile resFile(fileName);
    QString str;
    QString buff;
    int i;
    int j;
    int pos = 24;
    int len;
    double length;
    double MaxStress;

    if ( ! resFile.open(QIODevice::ReadOnly) )
        return false;

    QTextStream stream( &resFile );
    stream.setCodec( QTextCodec::codecForName("ISO 8859-1") );

    stream.readLine();
    str = stream.readLine();
    Data.pivotsNum = str.toInt();
    Data.pivotsFirstNodes.resize(Data.pivotsNum);
    Data.pivotsLastNodes.resize(Data.pivotsNum);
    Data.pivotLength.resize(Data.pivotsNum);
    Data.pivotSquare.resize(Data.pivotsNum);
    str = stream.readLine();
    Data.nodesNum = str.toInt();
    Data.x.resize(Data.nodesNum);
    Data.y.resize(Data.nodesNum);
    for ( i = 0; i < 3; i++) {
        str = stream.readLine(  );
    }
    Data.loadsNum = str.toInt();
    Data.xTrans.resize(Data.nodesNum * Data.loadsNum);
    Data.yTrans.resize(Data.nodesNum * Data.loadsNum);
    Data.stress.resize(Data.pivotsNum * Data.loadsNum);
    Data.safetyFactor.resize(Data.pivotsNum * Data.loadsNum);
    str = stream.readLine(  );
    Data.stressLimit = str.toDouble();

    QStringList lines;
    while ( !stream.atEnd() )
        lines += stream.readLine();
    resFile.close();    

    QStringList::Iterator it;
//---------------------------------------------------------------------------//
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Massiv topologiy stergney") == 0 )
            continue;
        for (i = 0;i < Data.pivotsNum;i++) {
            ++it; if ( it == lines.end() ) break; str = *it;

            buff = str[0];
            Data.pivotsFirstNodes[i] = buff.toInt();
            buff = str[1];
            Data.pivotsLastNodes[i] = buff.toInt();
        }
        break;
    }
//---------------------------------------------------------------------------//
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Massiv coordinat uzlov") == 0 ) 
            continue;
        for (i = 0;i < Data.nodesNum;i++) {
            ++it; if ( it == lines.end() ) break; str = *it;

            buff = str;
            len = str.length();
            str.remove(0,pos);
            buff.remove(pos,len - pos + 1);
            Data.x[i] = buff.toDouble();
            Data.y[i] = str.toDouble();
        }
        break;
    }
//---------------------------------------------------------------------------//
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Massiv nachalnih ploshadey") == 0 ) 
            continue;
        for (i = 0; i < Data.pivotsNum; i++) {
            ++it; if ( it == lines.end() ) break; str = *it;
            Data.pivotSquare[i] = str.toDouble();
        }
        break;
    }

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
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Peremeshenie uzlov") == 0 ) 
            continue;
        ++it; if ( it == lines.end() ) break; str = *it;
        for (i = 0; i < Data.loadsNum; i++) {
            for (j = 0; j < Data.nodesNum; j++) {
                ++it; if ( it == lines.end() ) break; str = *it;

                buff = str;
                len = str.length();
                str.remove( 0, pos );
                buff.remove( pos, len - pos + 1 );
                Data.xTrans[Data.nodesNum*i+j] = buff.toDouble();
                Data.yTrans[Data.nodesNum*i+j] = str.toDouble();
            }
            ++it; if ( it == lines.end() ) break; str = *it;
        }
        break;
    }
//---------------------------------------------------------------------------//
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Napryajenie v sterjnyah") == 0 ) 
            continue;
        ++it; if ( it == lines.end() ) break; str = *it;
        for (i = 0; i < Data.loadsNum; i++) {
            for (j = 0; j < Data.pivotsNum; j++) {
                ++it; if ( it == lines.end() ) break; str = *it;

                Data.stress[Data.pivotsNum*i+j] = str.toDouble();;
                if ( Data.stress[Data.pivotsNum*i+j] == 0 )
                    Data.safetyFactor[Data.pivotsNum*i+j] = 0;
                else
                    Data.safetyFactor[Data.pivotsNum*i+j] = 
                        Data.stressLimit/Data.stress[Data.pivotsNum*i+j];
            }
            ++it; if ( it == lines.end() ) break; str = *it;
        }
        break;
    }

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
    for ( it = lines.begin(); it != lines.end(); ++it ) {
        QString str = *it;
        if ( str.contains("Potrebnie ploshadi stergney") ==0 ) 
            continue;
        for (i = 0; i < Data.pivotsNum; i++) {
            ++it; if ( it == lines.end() ) break; str = *it;
            Data.pivotSquare[i] = str.toDouble();
        }
        break;
    }
//---------------------------------------------------------------------------//
    return true;
}

QDomElement VYVReader::toXML ( QDomDocument& doc ) const
{
    QDomElement plugRes = doc.createElement("PluginResults");
    
    plugRes.setAttribute( "forceWeight", Data.forceWeight );
    plugRes.setAttribute( "materialVolume", Data.v );
    plugRes.setAttribute( "maxTensionStress", Data.stressLimit );
    
    // Fill load case results data
    int i;
    for ( i = 0; i < Data.loadsNum; ++i ) {
        QDomElement loadCase = doc.createElement("LoadCaseResults");
        loadCase.setAttribute( "loadCaseNumber", i + 1 );
        plugRes.appendChild( loadCase );

        // Fill node results data
        for ( uint j = 0; j < Data.x.size(); ++j ) {
            QDomElement nodeRes = doc.createElement("NodeResults");
            nodeRes.setAttribute( "dispX", Data.xTrans[Data.nodesNum * i + j] );
            nodeRes.setAttribute( "dispY", Data.yTrans[Data.nodesNum * i + j] );
            nodeRes.setAttribute( "nodeNumber", j + 1 );
            loadCase.appendChild( nodeRes );
        }

        for ( uint k = 0; k < Data.pivotsFirstNodes.size(); ++k ) {
            QDomElement pivotRes = doc.createElement("PivotResults");
            pivotRes.setAttribute( "stress", Data.stress[Data.pivotsNum * i + k] );
            pivotRes.setAttribute( "requiredThickness", Data.pivotSquare[k] );
            pivotRes.setAttribute( "safetyMargin", 
                          fabs( Data.safetyFactor[Data.pivotsNum * i + k] ) );
            pivotRes.setAttribute( "pivotNumber", k + 1 );
            loadCase.appendChild( pivotRes );
        }
    }

    //plugRes.setAttribute( "nodesNum", Data.nodesNum );
    //plugRes.setAttribute( "pivotNum", Data.pivotsNum );
    //plugRes.setAttribute( "loadsNum", Data.loadsNum );

    return plugRes;
}

QString VYVReader::toXMLString () const
{ 
    QDomDocument doc;
    doc.appendChild( toXML(doc) );
    return doc.toString(4);
} 

/*****************************************************************************/
