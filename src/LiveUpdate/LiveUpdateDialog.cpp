
#include <QMessageBox>
#include <QFile>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QVBoxLayout>


#include "Global.h"
#include "MD5Generator.h"
#include "MD5Comparator.h"
#include "LiveUpdateDialog.h"

/*****************************************************************************/

LiveUpdateDialog::LiveUpdateDialog ( QWidget* parent ) :
    QDialog(parent, Qt::WindowTitleHint),
    m_checker(0),
    m_jobBuilder(0)
{
    setupUi(this);
    m_detailsButton->setVisible(false);
    m_okButton->setVisible(false);
    m_cancelButton->setVisible(true);
    m_cancelButton->setEnabled(false);

    QObject::connect( m_detailsButton, SIGNAL(pressed()),
                      SLOT(onDetailsPressed()) );
    QObject::connect( m_okButton, SIGNAL(pressed()),
                      SLOT(close()) );
    QObject::connect( m_cancelButton, SIGNAL(pressed()),
                      SLOT(onCancelPressed()) );
}

void LiveUpdateDialog::startUpdate ()
{
    setProgress( tr("Verifying version ..."), 1 );

    try { m_checker = new LiveUpdateChecker( LiveUpdateChecker::LiveUpdate ); }
    catch ( LiveUpdateChecker::ConfigIsWrongException& ) {
        QMessageBox::critical( this, tr("Error"), 
                               tr("Invalid LiveUpdate configuration.<br>"
                                  "Update will be terminated.") );
        QCoreApplication::quit();
        return;
    }

    show();

    m_checker->startCheck();
    m_checker->wait();

    if ( m_checker->isError() ) {
        error( m_checker->lastError() );
        return;
    }

    if ( m_checker->isUpToDate() ) {
        setProgress( tr("Curent version '%1' is up to date.").
                     arg( Global::applicationVersionNumber() ), 100 );

        m_cancelButton->setVisible(false);
        m_okButton->setVisible(true);
        //TODO: m_detailsButton->setVisible(true);
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

        bool ok = question( tr("MD5 file for local copy does not exist.<br>"
                               "Will generate new?") );

        if ( !ok ) {
            error( tr("MD5 file for local copy does not exist.") );
            return;
        }

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

        if ( resolveConflict(msgs) ) {
            m_jobBuilder->resolveConflict();
        }
        else {
            error( tr("Conflicts were found.") );
            return;
        }
    }
    
    const QList<Job*>& jobs = m_jobBuilder->getJobs();

    if ( jobs.isEmpty() ) {
        setProgress( tr("Curent version '%1' is up to date.").
                     arg( Global::applicationVersionNumber() ), 100 );
        m_cancelButton->setVisible(false);
        m_okButton->setVisible(true);
        //TODO: m_detailsButton->setVisible(true);
        return;
    }

    m_cancelButton->setEnabled(true);

    // Do jobs
    m_jobBuilder->doJobs();

    // Was an error? 
    if ( ! m_jobFailed.isEmpty() ) {
        m_cancelButton->setEnabled(false);
        m_jobBuilder->undoJobs();
    }
    // Was stopped? 
    else if ( ! m_jobStopped.isEmpty() ) {
        m_cancelButton->setEnabled(false);
        m_jobBuilder->undoJobs();
    }
    // Success
    else {
        // Rewrite MD5 root file
        QFile outMD5File( md5FileName );
        if ( ! outMD5File.open(QIODevice::WriteOnly) ) {
            warning( tr("Can't rewrite root MD5 file: '%1'").
                     arg(m_checker->rootMD5File()) );
        }
        outMD5File.write( downloadedMD5Doc.toString(4).toAscii() );
    }

    // Success
    if ( m_jobFailed.isEmpty() && m_jobStopped.isEmpty() ) {
        setProgress( tr("Done. Version '%1' has been updated to '%2'.").
                     arg( Global::applicationVersionNumber() ).
                     arg( m_checker->checkedVersion() ),  100 );
    }
    // Stopped
    else if ( ! m_jobStopped.isEmpty() ) {
        Job* job = m_jobBuilder->findJobByUuid( m_jobStopped );
        if ( job ) {         
            setProgress( tr("Canceled."),  0 );
        }
    }
    // Error
    else {
        Job* job = m_jobBuilder->findJobByUuid( m_jobFailed );
        if ( job ) {         
            setProgress( tr("Failed : %1.").arg(job->jobMessage()),  0 );
        }
    }

    // Success.

    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);
    //TODO: m_detailsButton->setVisible(true);

    delete m_jobBuilder;
    m_jobBuilder = 0;
    m_conflictList.clear();
    m_jobFailed.clear();
    m_jobStopped.clear();
}

LiveUpdateDialog::~LiveUpdateDialog ()
{
    delete m_jobBuilder;
    delete m_checker;
}

void LiveUpdateDialog::error ( const QString& e )
{
    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);
    //TODO: m_detailsButton->setVisible(true);
    setProgress( tr("Failed : %1.").arg(e), 0 );
    QCoreApplication::processEvents();
    return;
}

void LiveUpdateDialog::warning ( const QString& w )
{
    QMessageBox::warning( this, tr("Warning"), w );
    QCoreApplication::processEvents();
    return;
}

bool LiveUpdateDialog::question ( const QString& q )
{
    bool res = false;
    if ( QMessageBox::Ok == QMessageBox::question(this, tr("Question"), q,
                                                  QMessageBox::Yes,
                                                  QMessageBox::No) )
        res = true;
    QCoreApplication::processEvents();
    return res;
}

bool LiveUpdateDialog::resolveConflict ( const QStringList& conflicts )
{
    if ( conflicts.size() == 0 )
        return true;

    QString msg( (conflicts.size() == 1 ? "Conflict was found." : 
                                          "Conflicts were found.") );
    QMessageBox msgBox( QMessageBox::Question, "Conflict", 
                        msg, QMessageBox::NoButton, this);
    QPushButton* resolveButton = 
        msgBox.addButton(tr("Resolve all"), QMessageBox::ActionRole);
    QPushButton* cancelButton = 
        msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
    msgBox.addButton(tr("Details ..."), QMessageBox::ActionRole);

    while ( 1 ) {
        msgBox.exec();
        if ( msgBox.clickedButton() == resolveButton )
            return true;
        else if ( msgBox.clickedButton() == cancelButton )
            return false;
        // Details is pressed
        else { 
            // Create conflicts detail dialog
            
            QDialog d( this );            
            d.setWindowTitle( tr("Conflict details") );
            QVBoxLayout* vboxLayout = new QVBoxLayout(&d);
            vboxLayout->setSpacing(6);
            vboxLayout->setMargin(9);
            QListWidget* listWidget = new QListWidget(&d);
            listWidget->addItems( conflicts );
            vboxLayout->addWidget(listWidget);

            QDialogButtonBox* buttonBox = new QDialogButtonBox(&d);
            QObject::connect( buttonBox, SIGNAL(accepted()),
                              &d, SLOT(hide()) );
            buttonBox->setOrientation(Qt::Horizontal);
            buttonBox->setStandardButtons(QDialogButtonBox::Ok);
            vboxLayout->addWidget(buttonBox);

            QSize size(350, 250);
            size = size.expandedTo(d.minimumSizeHint());
            d.resize(size);
            d.exec();

            continue;
        }
    } 
}

void LiveUpdateDialog::setProgress ( const QString& text, int done )
{
    jobLabel->setText( text );
    progressBar->setValue( done );
    QCoreApplication::processEvents();
}

void LiveUpdateDialog::onCancelPressed ()
{
    if ( ! m_jobBuilder )
        return;
    m_jobBuilder->stopJobs();
}

void LiveUpdateDialog::onDetailsPressed ()
{
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
    Q_UNUSED(jobsToDo);
}

void LiveUpdateDialog::onBeforeUndoJobs ( uint jobsToUndo )
{
    Q_UNUSED(jobsToUndo);
}

void LiveUpdateDialog::onJobFailed ( const QString& jobUuid )
{
    m_jobFailed = jobUuid;
}

void LiveUpdateDialog::onJobStopped ( const QString& jobUuid )
{
    m_jobStopped = jobUuid;
}

/*****************************************************************************/
