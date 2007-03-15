
#include <QMessageBox>
#include <QFile>

#include "Global.h"
#include "MD5Generator.h"
#include "MD5Comparator.h"
#include "LiveUpdateDialog.h"

/*****************************************************************************/

LiveUpdateDialog::LiveUpdateDialog ( QWidget* parent ) :
    QDialog(parent),
    m_checker(0),
    m_jobBuilder(0)
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
    QString md5FileName = appPath + "/" + m_checker->rootMD5File();
        
    QDomDocument downloadedMD5Doc = md5Doc;
    QDomDocument currentMD5Doc;

    setProgress( tr("Generating MD5 ..."), 2 );

    QDomDocument generatedMD5Doc = MD5Generator::md5ForFiles( appPath );

    // Create current md5 if does not exist
    if ( ! QFile::exists(md5FileName) ) {

        setProgress( tr("Creating local MD5 file ..."), 3 );

        warning( tr("MD5 file for local copy does not exist.<br>"
                    "Will generate new.") );

        currentMD5Doc = generatedMD5Doc;
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

    m_jobBuilder = new JobBuilder( cmpMD5 );


    QObject::connect( m_jobBuilder, 
                      SIGNAL(progress(const QString&,double)),
                      SLOT(onJobProgress(const QString&,double)));

    QObject::connect( m_jobBuilder, 
                      SIGNAL(beforeDoJobs(uint)),
                      SLOT(onBeforeDoJobs(uint)));
    
    QObject::connect( m_jobBuilder, 
                      SIGNAL(beforeUndoJobs(uint)),
                      SLOT(onBeforeUndoJobs(uint)));
                                        
    QObject::connect( m_jobBuilder, 
                      SIGNAL(jobFailed(const QString&)),
                      SLOT(onJobFailed(const QString&)));

    QObject::connect( m_jobBuilder, 
                      SIGNAL(jobStopped(const QString&)),
                      SLOT(onJobStopped(const QString&)));


    if ( m_jobBuilder->isConflict() ) {
        const QStringList& msgs = m_jobBuilder->conflictMessages();
        QString msg = msgs.join("<br>");
        warning( msg );
        foreach ( QString msg, msgs ) {
            qWarning("CONFLICT: %s", qPrintable(msg));
        }
        
        qWarning(" ");
        qWarning(" ");
    }
    
    const QList<Job*>& jobs = m_jobBuilder->getJobs();

    if ( jobs.isEmpty() ) {
        setProgress( tr("Curent version '%1' is up to date.").
                     arg( Global::applicationVersionNumber() ), 100 );
        m_checker->disconnect();
        return;
    }

    foreach ( Job* job, jobs ) {
        qWarning("%s, UUID: %s", qPrintable(job->jobMessage()),
                 qPrintable(job->jobUuid()));
    }

    // Do jobs
    m_jobBuilder->doJobs();


    // Rewrite MD5 root file
    QFile outMD5File( md5FileName );
    if ( ! outMD5File.open(QIODevice::WriteOnly) ) {
        warning( tr("Can't rewrite root MD5 file: '%1'").
                 arg(m_checker->rootMD5File()) );
    }
    outMD5File.write( downloadedMD5Doc.toString(4).toAscii() );


    // Success. Quit
    delete m_jobBuilder;
    m_jobBuilder = 0;

    setProgress( tr("Done. Version '%1' has been updated to '%2'.").
                 arg( Global::applicationVersionNumber() ).
                 arg( m_checker->checkedVersion() ),  100 );
}

LiveUpdateDialog::~LiveUpdateDialog ()
{
    delete m_jobBuilder;
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
    QMessageBox::warning( this, tr("Warning"), w );
    QCoreApplication::processEvents();
    return;
}

void LiveUpdateDialog::setProgress ( const QString& text, int done )
{
    jobLabel->setText( text );
    progressBar->setValue( done );
    QCoreApplication::processEvents();
}

void LiveUpdateDialog::onJobProgress ( const QString& jobUuid, double done )
{
    if ( ! m_jobBuilder )
        return;

    Job* job = m_jobBuilder->findJobByUuid( jobUuid );
    if ( ! job )
        return;

    setProgress( job->jobMessage(), int(done * 90 / 100) + 10 );
}

void LiveUpdateDialog::onBeforeDoJobs ( uint jobsToDo )
{
    qWarning("onBeforeDoJobs %d", jobsToDo );
}

void LiveUpdateDialog::onBeforeUndoJobs ( uint jobsToUndo )
{
    qWarning("onBeforeUndoJobs %d", jobsToUndo );
}

void LiveUpdateDialog::onJobFailed ( const QString& jobUuid )
{
    qWarning("onJobFailed %s", qPrintable(jobUuid) );
}

void LiveUpdateDialog::onJobStopped ( const QString& jobUuid )
{
    qWarning("onJobStopped %s", qPrintable(jobUuid) );
}

/*****************************************************************************/
