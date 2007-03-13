
#include <QMessageBox>
#include <QFile>

#include "Global.h"
#include "MD5Generator.h"
#include "MD5Comparator.h"
#include "JobBuilder.h"
#include "LiveUpdateDialog.h"

/*****************************************************************************/

LiveUpdateDialog::LiveUpdateDialog ( QWidget* parent ) :
    QDialog(parent),
    m_checker(0)
{
    setupUi(this);
}

void LiveUpdateDialog::startUpdate ()
{
    setProgress( tr("Verifying version ..."), 1 );

    try { m_checker = new LiveUpdateChecker; }
    catch ( LiveUpdateChecker::ConfigIsWrongException& ) {
        QMessageBox::critical( this, tr("Error"), 
                               tr("Invalid LiveUpdate configuration.<br>"
                                  "Updation will be terminated.") );
        QCoreApplication::quit();
        return;
    }

    QObject::connect( m_checker, SIGNAL(error(const QString&)),
                                 SLOT(error(const QString&)) );

    show();

    m_checker->startCheck();
    m_checker->wait();

    if ( m_checker->isUpToDate() ) {
        setProgress( tr("Curent version '%1' is up to date.").
                     arg( Global::applicationVersionNumber() ), 100 );
        m_checker->disconnect();
        return;
    }

    // TODO: versionLabel->setText( m_checker->checkedVersion() );

    QDomDocument md5Doc = m_checker->getDownloadedMD5File();
    if ( md5Doc.isNull() )
        return;

    QString appPath = Global::applicationDirPath();
    QString md5FileName = appPath + "/md5.xml";
        
    QDomDocument downloadedMD5Doc = md5Doc;
    QDomDocument currentMD5Doc;

    setProgress( tr("Generating MD5 ..."), 2 );

    QDomDocument generatedMD5Doc = MD5Generator::md5ForFiles( appPath );

    // Create current md5 if does not exist
    if ( ! QFile::exists(md5FileName) ) {

        setProgress( tr("Creating local MD5 file ..."), 3 );

        warning( tr("Current MD5 file does not exist.<br>Will create new.") );

        currentMD5Doc = generatedMD5Doc;

        QFile outFile(md5FileName);
        if ( ! outFile.open(QIODevice::WriteOnly) ) {
            error( tr("Can't open file: '%1' for writing").arg(md5FileName) );
            return;
        }
        outFile.write( currentMD5Doc.toString(4).toAscii() );
    }
    else {
        setProgress( tr("Reading local MD5 file ..."), 3 );

        QFile inFile(md5FileName);
        if ( ! inFile.open(QIODevice::ReadOnly) ) {
            error( tr("Can't open file: '%1' for reading").arg(md5FileName) );
            return;
        }
        if ( ! currentMD5Doc.setContent(&inFile) ) {
            error( tr("Can't parse XML data from file: '%1'").
                   arg(md5FileName) );
            return;
        }
    }
    
    setProgress( tr("Compare local and downloaded MD5..."), 5 );

    // Firstly compare current and downloaded
    QDomDocument cmpCurrentDownloadedMD5 = 
        MD5Comparator::md5Compare( currentMD5Doc, downloadedMD5Doc );

    setProgress( tr("Compare local and generated MD5..."), 7 );

    // Then compare current and generated
    QDomDocument cmpCurrentGeneratedMD5 = 
        MD5Comparator::md5Compare( currentMD5Doc, generatedMD5Doc );

    setProgress( tr("Verifying conflicts ..."), 9 );

    // Then compare two comparisons
    QDomDocument cmpMD5 = 
        MD5Comparator::md5Compare( cmpCurrentGeneratedMD5,
                                       cmpCurrentDownloadedMD5 );

    setProgress( tr("Build job list ..."), 10 );

    JobBuilder jobBuilder( cmpMD5 );
    if ( jobBuilder.isConflict() ) {
        const QStringList& msgs = jobBuilder.conflictMessages();
        QString msg = msgs.join("<br>");
        warning( msg );
        foreach ( QString msg, msgs ) {
            qWarning("CONFLICT: %s", qPrintable(msg));
        }
        
        qWarning(" ");
        qWarning(" ");
    }
    
    const QList<Job*>& jobs = jobBuilder.getJobs();
    foreach ( Job* job, jobs ) {
        qWarning("%s", qPrintable(job->jobMessage()));
    }

    // Success. Quit
    m_checker->disconnect();
    QCoreApplication::quit();
}

LiveUpdateDialog::~LiveUpdateDialog ()
{
    delete m_checker;
}

void LiveUpdateDialog::error ( const QString& e )
{
    QMessageBox::critical( this, tr("Error"), e );

    // Do disconnection to stop other possible errors
    m_checker->disconnect();

    QCoreApplication::processEvents();
    QCoreApplication::quit();
    return;
}

void LiveUpdateDialog::warning ( const QString& w )
{
    QMessageBox::critical( this, tr("Warning"), w );
    QCoreApplication::processEvents();
    return;
}

void LiveUpdateDialog::setProgress ( const QString& text, int done )
{
    jobLabel->setText( text );
    progressBar->setValue( done );
    QCoreApplication::processEvents();
}

/*****************************************************************************/
