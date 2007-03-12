
#include <QApplication>
#include <QStringList>
#include <QFile>
#include <QDomDocument>

#include "MD5Generator.h"
#include "MD5Comparator.h"
#include "JobBuilder.h"
#include "LiveUpdateDialog.h"
#include "Global.h"

#include <iostream>

int main ( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QStringList args = QCoreApplication::arguments();

    // Generate MD5 sum 
    if ( args.size() > 1 && args[1] == "--gen-md5" ) {

        QString pathToMd5 = Global::applicationDirPath();
        QString output;

        if ( args.size() > 2 )
            output = args[2];
        if ( args.size() > 3 )
            pathToMd5 = args[3];

        QDomDocument md5Xml = MD5Generator::md5ForFiles( pathToMd5 );
        
        if ( output.isEmpty() ) {
            std::cout << qPrintable(md5Xml.toString(4));
            return 0;
        }

        QFile outFile( output );
        if ( ! outFile.open(QIODevice::WriteOnly) ) {
            qWarning( "Can't open file: '%s' for writing", 
                      qPrintable(output) );
            return 1;
        }
        outFile.write( md5Xml.toString(4).toAscii() );
        return 0;
    }

    // Compare MD5
    else if ( args.size() > 1 && args[1] == "--cmp-md5" ) {
        if ( args.size() <= 3 ) {
            qWarning( "Usage: %s --cmp-md5 md5.xml md5.xml", 
                    qPrintable(Global::applicationName()) );
            return 1;
        }
        if ( args[2] == args[3] ) {
            qWarning( "Can't compare same files" );
            return 1;
        }

        QFile oldMD5File( args[2] );
        QFile newMD5File( args[3] );
        if ( ! oldMD5File.open(QIODevice::ReadOnly) ) {
            qWarning( "Can't open file: '%s' for reading", 
                      qPrintable(oldMD5File.fileName()) );
            return 1;
        }
        if ( ! newMD5File.open(QIODevice::ReadOnly) ) {
            qWarning( "Can't open file: '%s' for reading", 
                      qPrintable(newMD5File.fileName()) );
            return 1;
        }

        QDomDocument oldMD5Doc;
        QDomDocument newMD5Doc;
        if ( ! oldMD5Doc.setContent(&oldMD5File) ) {
            qWarning( "Can't parse XML data from file: '%s'", 
                      qPrintable(oldMD5File.fileName()) );
            return 1;            
        }
        if ( ! newMD5Doc.setContent(&newMD5File) ) {
            qWarning( "Can't parse XML data from file: '%s'", 
                      qPrintable(newMD5File.fileName()) );
            return 1;
        }

        QDomDocument mergedDoc = MD5Comparator::md5Compare( oldMD5Doc,
                                                            newMD5Doc );
        std::cout << qPrintable( mergedDoc.toString(4) );
        return 0;
    }

    // Print job list
    else if ( args.size() > 1 && args[1] == "--job-list" ) {
        if ( args.size() <= 2 ) {
            qWarning( "Usage: %s --job-list md5.xml", 
                    qPrintable(Global::applicationName()) );
            return 1;
        }

        QFile md5File( args[2] );

        if ( ! md5File.open(QIODevice::ReadOnly) ) {
            qWarning( "Can't open file: '%s' for reading", 
                      qPrintable(md5File.fileName()) );
            return 1;
        }

        QDomDocument md5Doc;
        if ( ! md5Doc.setContent(&md5File) ) {
            qWarning( "Can't parse XML data from file: '%s'", 
                      qPrintable(md5File.fileName()) );
            return 1;            
        }

        JobBuilder jobBuilder( md5Doc );

        if ( jobBuilder.isConflict() ) {
            const QStringList& msgs = jobBuilder.conflictMessages();
            foreach ( QString msg, msgs ) {
                qWarning("CONFLICT: %s", qPrintable(msg));
            }

            qWarning("");
            qWarning("");
        }

        const QList<Job*>& jobs = jobBuilder.getJobs();
        foreach ( Job* job, jobs ) {
            qWarning("%s", qPrintable(job->jobMessage()));
        }
        return 0;
    }
    else {
        LiveUpdateDialog dialog;
        dialog.show();
        return app.exec();


        /* TODO
        
        // Start live update
        LiveUpdateChecker checker;
        checker.startCheck();
        checker.wait();

        QDomDocument md5Doc = checker.getDownloadedMD5File();
        if ( md5Doc.isNull() ) {
            qWarning("Can't get root file!");
        }

        QString appPath = Global::applicationDirPath();
        QString md5FileName = appPath + "/md5.xml";

        QDomDocument downloadedMD5Doc = md5Doc;
        QDomDocument currentMD5Doc;
        QDomDocument generatedMD5Doc = MD5Generator::md5ForFiles( appPath );

        // Create current md5 if does not exist
        if ( ! QFile::exists(md5FileName) ) {
            currentMD5Doc = generatedMD5Doc;

            QFile outFile(md5FileName);
            if ( ! outFile.open(QIODevice::WriteOnly) ) {
                qWarning( "Can't open file: '%s' for writing", 
                          qPrintable(md5FileName) );
            }
            outFile.write( currentMD5Doc.toString(4).toAscii() );
        }
        else {
            QFile inFile(md5FileName);
            if ( ! inFile.open(QIODevice::ReadOnly) ) {
                qWarning( "Can't open file: '%s' for reading", 
                          qPrintable(md5FileName) );
                return 1;
            }
            if ( ! currentMD5Doc.setContent(&inFile) ) {
                qWarning( "Can't parse XML data from file: '%s'", 
                          qPrintable(md5FileName) );
                return 1;
            }
        }

        // Firstly compare current and downloaded
        QDomDocument cmpCurrentDownloadedMD5 = 
            MD5Comparator::md5Compare( currentMD5Doc, downloadedMD5Doc );
        // Then compare current and generated
        QDomDocument cmpCurrentGeneratedMD5 = 
            MD5Comparator::md5Compare( currentMD5Doc, generatedMD5Doc );
        // Then compare two comparisons
        QDomDocument cmpMD5 = 
            MD5Comparator::md5Compare( cmpCurrentGeneratedMD5,
                                       cmpCurrentDownloadedMD5 );

        JobBuilder jobBuilder( cmpMD5 );
        if ( jobBuilder.isConflict() ) {
            const QStringList& msgs = jobBuilder.conflictMessages();
            foreach ( QString msg, msgs ) {
                qWarning("CONFLICT: %s", qPrintable(msg));
            }

            qWarning("");
            qWarning("");
        }

        const QList<Job*>& jobs = jobBuilder.getJobs();
        foreach ( Job* job, jobs ) {
            qWarning("%s", qPrintable(job->jobMessage()));
        }
        return 0;

        */
    }

    return 0;
}
