
#include <QRegExp>
#include <QDir>
#include <QFileInfo>
#include <QUuid>
#include <QCoreApplication>

#ifdef Q_OS_WIN
 #include <windows.h>
#endif

#include "JobBuilder.h"
#include "Config.h"
#include "Global.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("LiveUpdate.JobBuilder") );

/*****************************************************************************
 * Helper
 *****************************************************************************/

#define DO_GET "get"
#define DO_REPLACE "replace"
#define DO_DELETE "delete"
#define CONFLICT "conflict"

bool SortElements ( const QDomElement& fir,  const QDomElement& sec )
{
    /*
    // Sorted jobs:

    A(GET)_A(DIR)_/home/user/a.dir
    A(GET)_A(DIR)_/home/user/b.dir

    A(GET)_B(FILE)_/home/user/a.file
    A(GET)_B(FILE)_/home/user/b.file

    B(REPLACE)_/home/user/a
    B(REPLACE)_/home/user/b

    C(DELETE)_A(FILE)_/home/user/a.file
    C(DELETE)_A(FILE)_/home/user/b.file

    C(DELETE)_B(DIR)_/home/user/a.dir/b.dir/c.dir
    C(DELETE)_B(DIR)_/home/user/a.dir/b.dir
    C(DELETE)_B(DIR)_/home/user/a.dir
    C(DELETE)_B(DIR)_/home/user/b.dir
    */

    QString firSortString;
    QString secSortString;

    QString firStatus = fir.attribute("status");
    QString secStatus = sec.attribute("status");

    // First status
    if ( firStatus.contains(DO_GET) )
        firSortString += "A";
    else if ( firStatus.contains(DO_REPLACE) )
        firSortString += "B";
    else if ( firStatus.contains(DO_DELETE) )
        firSortString += "C";

    // Second status
    if ( secStatus.contains(DO_GET) )
        secSortString += "A";
    else if ( secStatus.contains(DO_REPLACE) )
        secSortString += "B";
    else if ( secStatus.contains(DO_DELETE) )
        secSortString += "C";


    QString firTag = fir.tagName();
    QString secTag = sec.tagName();

    // First tag
    if ( firTag == "Directory" ) {
        if ( firStatus.contains(DO_DELETE) )
            firSortString += "B";
        else
            firSortString += "A";
    }
    else if ( firTag.contains("File") ) {
        if ( firStatus.contains(DO_DELETE) )
            firSortString += "A";
        else
            firSortString += "B";
    }

    // Second tag
    if ( secTag == "Directory" ) {
        if ( secStatus.contains(DO_DELETE) )
            secSortString += "B";
        else
            secSortString += "A";
    }
    else if ( secTag.contains("File") ) {
        if ( secStatus.contains(DO_DELETE) )
            secSortString += "A";
        else
            secSortString += "B";
    }

    firSortString += fir.attribute("name");
    secSortString += sec.attribute("name");

    // If first and second element are directories
    // with same DELETE status: use reverse order!
    // Because:
    //    Delete /a.dir/b.dir/c.dir
    //    Delete /a.dir/b.dir
    //    Delete /a.dir
    // 
    // I.e. we should delete all subdirs firstly

    if ( firTag == "Directory" && 
         secTag == "Directory" &&
         secStatus.contains(DO_DELETE) &&
         firStatus.contains(DO_DELETE) ) {
        // Reverse order!
        return secSortString < firSortString;
    }

    // Direct order!
    return firSortString < secSortString;
}

/*****************************************************************************/

Job::Job ( Job::JobType jobType ) :
    m_jobUuid( QUuid::createUuid().toString() ),
    m_jobType( jobType )
{}

Job::~Job ()
{}

const QString& Job::jobUuid () const
{ return m_jobUuid; }

const QString& Job::lastError () const
{ return m_lastError; }

bool Job::isError () const
{ return ! m_lastError.isEmpty(); }

void Job::clearLastError ()
{ m_lastError.clear(); }

void Job::setErrorString ( const QString& err ) 
{ m_lastError = err; }

Job::JobType Job::jobType () const
{ return m_jobType; }

/*****************************************************************************/

DownloadJob::DownloadJob ( const QString& url, const QString& fileToSave,
                           bool executable ) :
    Job( Job::DownloadJob ),
    m_urlToDownload( url ),
    m_fileToSave( fileToSave ),
    m_progressStatus( Job::Success ),
    m_progressDone(0.0),
    m_requestAborted(false),
    m_executable(executable)
{
    m_http.setHost( m_urlToDownload.host() );

    QObject::connect( &m_http, SIGNAL(dataReadProgress(int,int)), 
                               SLOT(httpReadProgress(int,int)) );
    QObject::connect( &m_http, SIGNAL(done(bool)),
                               SLOT(httpDone(bool)) );
    QObject::connect( &m_http, 
                   SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
                   SLOT(httpResponseHeader(const QHttpResponseHeader &)) );
}

DownloadJob::~DownloadJob ()
{
    m_http.disconnect();
    m_fileToSave.close();
    m_http.close();
}

QString DownloadJob::jobMessage () const
{
    QString msg( tr("Downloading '%1'") );
    return msg.arg( m_urlToDownload.path().section('/', -1) );
}

void DownloadJob::doJob ()
{
    if ( m_progressStatus == Job::Running )
        return;

    m_progressStatus = Job::Running;
    m_progressDone = 0.0;
    Job::clearLastError();

    if ( m_fileToSave.exists() ) {
        QString msg( tr("File name exists: '%1'") );
        Job::setErrorString( msg.arg(m_fileToSave.fileName().section('/',-1)));

        m_progressDone = 0.0;
        m_progressStatus = Job::Failed;

        return;
    }

    if ( ! m_fileToSave.open(QIODevice::WriteOnly) ) {
        QString msg( tr("Can't open file for writing: '%1'") );
        Job::setErrorString( msg.arg(m_fileToSave.fileName().section('/',-1)));

        m_progressDone = 0.0;
        m_progressStatus = Job::Failed;

        return;
    }

    m_http.get( m_urlToDownload.path(), &m_fileToSave );
}

void DownloadJob::undoJob ()
{
    m_fileToSave.close();
    m_fileToSave.remove();
}

void DownloadJob::stopJob ()
{
    if ( m_progressStatus != Job::Running )
        return;

    m_fileToSave.close();
    m_http.abort();
    m_requestAborted = true;
}

void DownloadJob::getCurrentProgress ( JobProgressStatus& status, 
                                       double& done )
{
    status = m_progressStatus;
    done = m_progressDone;
}

void DownloadJob::httpReadProgress ( int done, int total )
{
    if ( total == 0 ) {
        return;
    }
    m_progressDone = done * 100 / total;
}

void DownloadJob::httpResponseHeader ( const QHttpResponseHeader& header )
{
    if ( header.statusCode() != 200 ) {
        m_http.abort();
        return;
    }
}

void DownloadJob::httpDone ( bool error )
{
    m_fileToSave.close();

    if ( m_requestAborted ) {
        m_fileToSave.remove();
        return;
    }

    if ( error ) {
        m_fileToSave.remove();
        m_progressDone = 0.0;
        m_progressStatus = Job::Failed;
        Job::setErrorString( m_http.errorString() );
    }
    else {
        // Set file permissions
        if ( m_executable ) {
            QFile::Permissions p = m_fileToSave.permissions();
            m_fileToSave.setPermissions( p | QFile::QFile::ExeOwner );
        }

        m_progressDone = 100.0;
        m_progressStatus = Job::Success;
    }
}

/*****************************************************************************/

RenameJob::RenameJob ( const QString& from, const QString& to ) :
    Job( Job::RenameJob ),
    m_progressStatus( Job::Success ),
    m_progressDone(0.0),
    m_fromPath( from ),
    m_toPath( to )
{}

RenameJob::~RenameJob ()
{}

QString RenameJob::jobMessage () const
{
    QString msg( tr("Renaming to '%1'") );
    return msg.arg( m_toPath.section('/', -1) );
}

void RenameJob::doJob ()
{
    m_progressStatus = Job::Running;
    m_progressDone = 0.0;

    if  ( ! QFile::rename(m_fromPath, m_toPath) ) {
        QString msg( tr("Can't rename file from '%1' to '%2'") );
        Job::setErrorString( msg.arg(m_fromPath.section('/', -1)).
                                 arg(m_toPath.section('/', -1)) );

        m_progressDone = 0.0;
        m_progressStatus = Job::Failed;

        return;
    }

    m_progressDone = 100.0;
    m_progressStatus = Job::Success;
}

void RenameJob::undoJob ()
{
    QFile::rename( m_toPath, m_fromPath );
}

void RenameJob::stopJob ()
{
    // Nothing
}

void RenameJob::getCurrentProgress ( JobProgressStatus& status, 
                                     double& done )
{
    status = m_progressStatus;
    done = m_progressDone;
}

/*****************************************************************************/

DeleteJob::DeleteJob ( const QString& path ) :
    Job( Job::DeleteJob ),
    m_progressStatus( Job::Success ),
    m_progressDone(0.0),
    m_pathToDelete( path )
{}

DeleteJob::~DeleteJob ()
{}

QString DeleteJob::jobMessage () const
{
    QString msg( tr("Deleting '%1'") );
    return msg.arg( m_pathToDelete.section('/', -1) );
}

void DeleteJob::doJob ()
{
    m_progressStatus = Job::Running;
    m_progressDone = 0.0;

    QFileInfo fileInfo( m_pathToDelete );

    // If directory
    if ( fileInfo.isDir() ) {
        QDir dir;
        dir.rmdir( m_pathToDelete );
    }
    // If file
    else {
        QFile::remove( m_pathToDelete );
    }

    // Delete job (3-rd step) CAN NOT FAIL.
    // Axiom!

    m_progressDone = 100.0;
    m_progressStatus = Job::Success;
}

void DeleteJob::undoJob ()
{
    // Nothing. Can't redo
}

void DeleteJob::stopJob ()
{
    // Nothing
}

void DeleteJob::getCurrentProgress ( JobProgressStatus& status, 
                                     double& done )
{
    status = m_progressStatus;
    done = m_progressDone;
}

const QString& DeleteJob::pathToDelete () const
{ return m_pathToDelete; }

/*****************************************************************************/

DeleteLiveUpdateBinaryJob::DeleteLiveUpdateBinaryJob ( const QString& path ) :
    DeleteJob(path)
{}

DeleteLiveUpdateBinaryJob::~DeleteLiveUpdateBinaryJob ()
{}

void DeleteLiveUpdateBinaryJob::doJob ()
{
#ifdef Q_OS_WIN
    QString toRemove = QDir::toNativeSeparators(DeleteJob::pathToDelete());
    QString tempFileName( QDir::tempPath() + "/" + 
                          QUuid::createUuid().toString() + ".bat" );
    tempFileName = QDir::toNativeSeparators(tempFileName);
    QFile tempFile(tempFileName);
    if ( ! tempFile.open(QIODevice::WriteOnly) ) {
        // Last try
        QFile::remove(toRemove);
        return;
    }

    QString batContent =  QString(":Repeat\r\n"
                                  "del \"%1\"\r\n"
                                  "if exist \"%1\" goto Repeat\r\n"
                                  "del \"%2\"").
        arg(toRemove).arg(tempFileName);

    tempFile.write( batContent.toAscii().data() );
    tempFile.close();
    ShellExecuteA( 0, "open", tempFileName.toAscii().data(), 0, 0, SW_HIDE );
#else
    DeleteJob::doJob();
#endif
}

/*****************************************************************************/

CreateDirJob::CreateDirJob ( const QString& path ) :
    Job( Job::CreateDirJob ),
    m_progressStatus( Job::Success ),
    m_progressDone(0.0),
    m_dirToCreate( path )
{}

CreateDirJob::~CreateDirJob ()
{}

QString CreateDirJob::jobMessage () const
{
    QString msg( tr("Creating directory '%1'") );
    return msg.arg( m_dirToCreate.section('/', -1) );
}

void CreateDirJob::doJob ()
{
    m_progressStatus = Job::Running;
    m_progressDone = 0.0;

    QDir dir;

    if ( ! dir.mkdir(m_dirToCreate) ) {
        QString msg( tr("Can't create directory '%1'") );
        Job::setErrorString( msg.arg(m_dirToCreate.section('/', -1)) );

        m_progressDone = 0.0;
        m_progressStatus = Job::Failed;

        return;
    }

    m_progressDone = 100.0;
    m_progressStatus = Job::Success;
}

void CreateDirJob::undoJob ()
{
    QDir dir;
    dir.rmdir(m_dirToCreate);
}

void CreateDirJob::stopJob ()
{
    // Nothing
}

void CreateDirJob::getCurrentProgress ( JobProgressStatus& status, 
                                        double& done )
{
    status = m_progressStatus;
    done = m_progressDone;
}

/*****************************************************************************/

JobBuilder::JobBuilder ( const QDomDocument& md5Compared ) :
    m_currentJob(0),
    m_isConflict(false),
    m_jobsTerminated(false),
    m_pointOfNoReturnInd(-1)
{
    QDomElement md5CmpElem = md5Compared.documentElement();
    QList<QDomElement> elems;
    parseDocument( md5CmpElem, elems );
    qSort( elems.begin(), elems.end(), SortElements );
    createJobsList( elems );
}

JobBuilder::~JobBuilder ()
{
    clearJobs();
}

bool JobBuilder::isConflict () const
{ return m_isConflict; }

void JobBuilder::resolveConflict ()
{ 
    m_conflictMsgs.clear();
    m_isConflict = false; 
}

const QStringList& JobBuilder::conflictMessages () const
{ return m_conflictMsgs; }

void JobBuilder::clearJobs ()
{
    resolveConflict();

    m_currentJob = 0;
    m_jobsTerminated = false;
    m_pointOfNoReturnInd = -1;

    QList<Job*>::Iterator it = m_jobs.begin();
    for ( ; it != m_jobs.end(); ++it )
        delete *it;
    m_jobs.clear();
}

void JobBuilder::parseDocument ( const QDomElement& md5CmpElem,
                                 QList<QDomElement>& elements )
{
    if ( md5CmpElem.isNull() ||
         md5CmpElem.tagName() != "Directory" && 
         md5CmpElem.tagName() != "File" && 
         md5CmpElem.tagName() != "MD5" ) {
        LOG4CXX_WARN(logger, 
                     QString("Element is not a file or directory: %1").
                     arg(md5CmpElem.tagName()).toStdString() );
        return;
    }  

    if ( md5CmpElem.tagName() != "MD5" ) {
        // Add if File or Directory
        elements.append( md5CmpElem );
 
        LOG4CXX_INFO(logger, QString("Add element to list: %1").
                     arg(md5CmpElem.tagName()).toStdString() );
    }

    if ( (md5CmpElem.tagName() == "Directory" || 
          md5CmpElem.tagName() == "MD5" ) &&
         md5CmpElem.hasChildNodes() ) {

        QDomNodeList kids = md5CmpElem.childNodes();
        for ( int i = 0; i < kids.size(); ++i ) {
            QDomNode node = kids.item(i);
            if ( node.isElement() ) {

                QDomElement elem = node.toElement();

                // Set full relative path
                if ( md5CmpElem.tagName() == "Directory" &&
                     md5CmpElem.hasAttribute("name") ) {
                    QString newName = md5CmpElem.attribute("name") + "/" +
                                      elem.attribute("name");
                    elem.setAttribute( "name", newName );
                }

                parseDocument( elem, elements );
            }
        }
    }
}

void JobBuilder::createJobsList ( const QList<QDomElement>& elements )
{
    QString url = "http://";

    Config& config = Global::config();
    ConfigNode rootNode = config.rootNode();
    ConfigNodeList cfgNodes = rootNode.findChildNodes( "LiveUpdate" );
    if ( cfgNodes.size() != 0 ) {
        NodeAttributeList attrs = cfgNodes.at(0).getAttributes();
        QString cfgUrl;
        QString cfgName;
        foreach ( NodeAttribute attr, attrs ) {
            if ( attr.first == "url" )
                cfgUrl = attr.second;
            else if ( attr.first == "name" )
                cfgName = attr.second;
        }

        if ( cfgUrl.isEmpty() || cfgName.isEmpty() ) {
            LOG4CXX_ERROR(logger, "Config for LiveUpdate is not valid");
            return;
        }

#if defined Q_OS_WIN
        QString os = "win";
#elif defined Q_OS_LINUX
        QString os = "lin";
#elif defined Q_OS_MAX
        QString os = "mac";
#else
#error Unsupported os
#endif
        url = cfgUrl + "/" + cfgName + "-" + os + "/";

    } else {
        LOG4CXX_ERROR(logger, "Config for LiveUpdate is not available");
        return;
    }

    QRegExp conflictRegExp("^" + QString(CONFLICT) + "\\(.+\\)$");

    QList<QDomElement>::ConstIterator it = elements.begin();

    QList<Job*> firstSteps;
    QList<Job*> secondSteps;
    QList<Job*> thirdSteps;
    QStringList conflictMsgs;
    bool conflict = false;

    for ( ; it != elements.end(); ++it ) {
        QDomElement elem = *it;

        // Continue if attribute does not have any status or name
        if ( !elem.hasAttribute("status") ||
             !elem.hasAttribute("name") )
            continue;

        QString status = elem.attribute("status");
        QString name = elem.attribute("name");

        bool elemConflict = conflictRegExp.exactMatch(status);

        // Mark that there was a conflict
        if ( ! conflict && elemConflict ) {
            conflict = true;
        }

        if ( elem.tagName() == "File" ) {
            
            bool exec = elem.hasAttribute( "permissions" );

            if ( status.contains(DO_GET) ) {
                firstSteps.append( new DownloadJob( url + name, name, exec) );
            }
            else if ( status.contains(DO_REPLACE) ) {

                // Add conflict message
                if ( elemConflict ) {
                    QString msg( tr("Can't replace. Locally modified: '%1'") );
                    conflictMsgs.append( msg.arg(name) );
                }

                firstSteps.append( 
                          new DownloadJob( url + name, name + ".NEW", exec ) );

                secondSteps.append(
                          new RenameJob( name, name + ".DELETE" ) );
                secondSteps.append(
                          new RenameJob( name + ".NEW", name ) );

                // We can't just delete ourselfs. 
                if ( name == Global::applicationName() ) {
                    thirdSteps.append(
                          new DeleteLiveUpdateBinaryJob( name + ".DELETE" ) );
                }
                else
                    thirdSteps.append(
                          new DeleteJob( name + ".DELETE" ) );
            }
            else if ( status.contains(DO_DELETE) ) {

                // Add conflict message
                if ( elemConflict ) {
                    QString msg( tr("Can't delete. Locally modified: '%1'") );
                    conflictMsgs.append( msg.arg(name) );
                }

                secondSteps.append( 
                          new RenameJob( name, name + ".DELETE" ) );

                // We can't just delete ourselfs. 
                if ( name == Global::applicationName() ) {
                    thirdSteps.append(
                          new DeleteLiveUpdateBinaryJob( name + ".DELETE" ) );
                }
                else
                    thirdSteps.append(
                          new DeleteJob( name + ".DELETE" ) );
            }
            else 
                // Ok, status is unknown
                continue;
        }
        else if ( elem.tagName() == "Directory" &&
                  elem.hasAttribute("status") ) {
            if ( status.contains(DO_GET) ) {
                firstSteps.append( new CreateDirJob( name ) );
            }
            else if ( status.contains(DO_REPLACE) ) {
                // Never should happen for directory
                LOG4CXX_WARN(logger, QString("Replace for directory: %1. "
                                             "Should never happen").arg(name).
                             toStdString());
                continue;
            }
            else if ( status.contains(DO_DELETE) ) {
                thirdSteps.append( new DeleteJob( name ) );
            }
            else 
                // Ok, status is unknown
                continue;
        }
        else 
            // Unknown element
            continue;
    }

    // Firstly do clean
    clearJobs();

    // Append in order
    m_jobs = firstSteps + secondSteps + thirdSteps;

    // Set point of not return index
    m_pointOfNoReturnInd = firstSteps.size() + secondSteps.size();

    // Sort and copy conflict msgs
    qSort( conflictMsgs.begin(), conflictMsgs.end() );
    m_conflictMsgs = conflictMsgs;
    m_isConflict = conflict;
}

bool JobBuilder::doJobs ()
{
    // Do nothing if conflict. Should be resolved first.
    if ( m_isConflict )
        return false;

    if ( m_jobs.size() == 0 )
        return true;

    double percentsPerJob = 100.0 / m_jobs.size();
  
    emit beforeDoJobs( m_jobs.size() );

    m_currentJob = 0;

    for ( int i = 0; i < m_jobs.size(); ++i ) {

        Job* job = m_jobs[i];
        m_currentJob = job;

        // Point of not return is reached
        if ( i == m_pointOfNoReturnInd )
            emit pointOfNoReturn( job->jobUuid() );

        Job::JobProgressStatus status = Job::Running;
        double percentsDone = percentsPerJob * i;
        double percentageJob = 0;

        // Progress before do
        emit progress( job->jobUuid(), percentsDone );

        // Job can be terminated at this point
        if ( m_jobsTerminated ) {
            emit jobStopped( job->jobUuid() );
            return false;
        }

        job->doJob();

        do {
            job->getCurrentProgress( status, percentageJob );
            double done = percentsDone + percentsPerJob * percentageJob /100.0;

            emit progress( job->jobUuid(), done );

            if ( status == Job::Running && ! m_jobsTerminated ) {
                QCoreApplication::processEvents();
                Global::sleepMsecs( 20 );
            }
        }
        while ( status == Job::Running && ! m_jobsTerminated );
        
        if ( status == Job::Failed ) {
            if ( m_jobsTerminated )
                emit jobStopped( job->jobUuid() );
            else
                emit jobFailed( job->jobUuid() );
            return false;
        }

        // If jobs were terminated
        if ( m_jobsTerminated ) {
            emit jobStopped( job->jobUuid() );
            return false;
        }

        // Success        
        // Last element
        if ( i + 1 == m_jobs.size() )
            percentsDone = 100.0;
        else 
            percentsDone = percentsPerJob * (i + 1);

        emit progress( job->jobUuid(), percentsDone );
    }
    m_currentJob = 0;
    return true;
}

bool JobBuilder::undoJobs ()
{
    // Do nothing if conflict. Should be resolved first.
    if ( m_isConflict )
        return false;

    if ( m_currentJob == 0 )
        // Nothing to undo
        return true;

    int index = m_jobs.indexOf( m_currentJob );

    // If nothing was found or first job failed
    if ( index == -1 ) {
        m_currentJob = 0;
        return true;
    }

    // Can't undo if point of no return is reached
    if ( m_pointOfNoReturnInd == index )
        return false;

    double percentsPerJob = 100 / m_jobs.size();
    double percentsDone = (index + 1) * percentsPerJob;

    emit beforeUndoJobs( index );

    for ( int i = index; i >= 0; --i ) {
        Job* job = m_jobs[i];

        // Progress before undo
        emit progress( job->jobUuid(), percentsDone );

        job->undoJob();

        // Last element
        if ( i == 0 )
            percentsDone = 0.0;
        else
            percentsDone -= percentsPerJob;

        emit progress( job->jobUuid(), percentsDone );
    }

    m_currentJob = 0;

    return true;
}

void JobBuilder::stopJobs ()
{
    m_jobsTerminated = true;
}

const QList<Job*>& JobBuilder::getJobs () const
{ return m_jobs; }

Job* JobBuilder::findJobByUuid ( const QString& uuid ) const
{
    QList<Job*>::ConstIterator it;
    for ( it = m_jobs.begin(); it != m_jobs.end(); ++it )
        if ( (*it)->jobUuid() == uuid )
            return *it;
    return 0;
}

/*****************************************************************************/
