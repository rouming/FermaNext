
#include <QRegExp>
#include <QUuid>

#include "JobBuilder.h"
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

    firSortString = fir.attribute("name");
    secSortString = sec.attribute("name");

    return firSortString < secSortString;
}

/*****************************************************************************/

Job::Job ( Job::JobType jobType, bool isConflict ) :
    m_jobUuid( QUuid::createUuid().toString() ),
    m_jobType( jobType ),
    m_isConflict( isConflict )
{}

Job::~Job ()
{}

bool Job::isConflict () const
{ return m_isConflict; }

void Job::resolveConflict ()
{ m_isConflict = false; }

const QString& Job::jobUuid () const
{ return m_jobUuid; }

Job::JobType Job::jobType () const
{ return m_jobType; }

/*****************************************************************************/

DownloadJob::DownloadJob ( const QString& url, const QString& pathToSave, 
                           bool isConflict ) :
    Job( Job::DownloadJob, isConflict ),
    m_urlToDownload( url ),
    m_pathToSave( pathToSave )
{}

DownloadJob::~DownloadJob ()
{}

QString DownloadJob::conflictMessage () const
{
    //TODO
    return "";
}

QString DownloadJob::jobMessage () const
{
    //TODO
    return "";
}

bool DownloadJob::doJob ()
{
    //TODO
    return true;
}

bool DownloadJob::undoJob ()
{
    //TODO
    return true;
}

/*****************************************************************************/

RenameJob::RenameJob ( const QString& from, const QString& to, 
                           bool isConflict ) :
    Job( Job::RenameJob, isConflict ),
    m_fromPath( from ),
    m_toPath( to )
{}

RenameJob::~RenameJob ()
{}

QString RenameJob::conflictMessage () const
{
    //TODO
    return "";
}

QString RenameJob::jobMessage () const
{
    //TODO
    return "";
}

bool RenameJob::doJob ()
{
    //TODO
    return true;
}

bool RenameJob::undoJob ()
{
    //TODO
    return true;
}

/*****************************************************************************/

DeleteJob::DeleteJob ( const QString& path,  bool isConflict ) :
    Job( Job::DeleteJob, isConflict ),
    m_pathToDelete( path )
{}

DeleteJob::~DeleteJob ()
{}

QString DeleteJob::conflictMessage () const
{
    //TODO
    return "";
}

QString DeleteJob::jobMessage () const
{
    //TODO
    return "";
}

bool DeleteJob::doJob ()
{
    //TODO
    return true;
}

bool DeleteJob::undoJob ()
{
    //TODO
    return true;
}

/*****************************************************************************/

CreateDirJob::CreateDirJob ( const QString& path,  bool isConflict ) :
    Job( Job::CreateDirJob, isConflict ),
    m_dirToCreate( path )
{}

CreateDirJob::~CreateDirJob ()
{}

QString CreateDirJob::conflictMessage () const
{
    //TODO
    return "";
}

QString CreateDirJob::jobMessage () const
{
    //TODO
    return "";
}

bool CreateDirJob::doJob ()
{
    //TODO
    return true;
}

bool CreateDirJob::undoJob ()
{
    //TODO
    return true;
}

/*****************************************************************************/

JobBuilder::JobBuilder ( const QDomDocument& md5Compared ) :
    m_currentJob(0)
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

void JobBuilder::clearJobs ()
{
    m_currentJob = 0;

    QList<Job*>::Iterator it = m_jobs.begin();
    for ( ; it != m_jobs.end(); ++it )
        delete *it;
    m_jobs.clear();
}

void JobBuilder::parseDocument ( const QDomElement& md5CmpElem,
                                 QList<QDomElement>& elements )
{
    if ( ! md5CmpElem.isNull() &&
         md5CmpElem.tagName() != "Directory" && 
         md5CmpElem.tagName() != "File" && 
         md5CmpElem.tagName() != "MD5" ) {
        LOG4CXX_WARN(logger, "Element is not a file or directory");
        return;
    }

    if ( md5CmpElem.tagName() != "MD5" )
        elements.append( md5CmpElem );

    if ( md5CmpElem.tagName() == "Directory" &&
         md5CmpElem.hasChildNodes() ) {

        QDomNodeList kids = md5CmpElem.childNodes();
        for ( int i = 0; i < kids.size(); ++i ) {
            QDomNode node = kids.item(i);
            if ( node.isElement() )
                parseDocument( node.toElement(), elements );
        }
    }
}

void JobBuilder::createJobsList ( const QList<QDomElement>& elements )
{
    QRegExp conflictRegExp("^" + QString(CONFLICT) + "\\(.+\\)$");

    QList<QDomElement>::ConstIterator it = elements.begin();

    QList<Job*> firstSteps;
    QList<Job*> secondSteps;
    QList<Job*> thirdSteps;

    for ( ; it != elements.end(); ++it ) {
        QDomElement elem = *it;

        if ( elem.tagName() == "File" &&
             elem.hasAttribute("status") ) {

            QString status = elem.attribute("status");

            bool conflict = conflictRegExp.exactMatch(status);
        
            if ( status.contains(DO_GET) ) {
                firstSteps.append(
                          new DownloadJob( "url", "path_to_save", conflict ) );
            }
            else if ( status.contains(DO_REPLACE) ) {
                firstSteps.append(
                          new DownloadJob( "url", "path_to_save", conflict ) );
                secondSteps.append(
                          new RenameJob( "from", "to", conflict ) );
                thirdSteps.append(
                          new DeleteJob( "path_to_delete", conflict ) );
            }
            else if ( status.contains(DO_DELETE) ) {
                secondSteps.append( 
                          new RenameJob( "from", "to", conflict ) );
                thirdSteps.append( 
                          new DeleteJob("path_to_delete", conflict ) );
            }
            else 
                // Ok, status is unknown
                continue;
        }
        else if ( elem.tagName() == "Directory" &&
                  elem.hasAttribute("status") ) {

            QString status = elem.attribute("status");

            bool conflict = conflictRegExp.exactMatch(status);
        
            if ( status.contains(DO_GET) ) {
                firstSteps.append(
                          new CreateDirJob( "path_to_create", conflict ) );
            }
            else if ( status.contains(DO_REPLACE) ) {
                // Never should happen for directory
                continue;
            }
            else if ( status.contains(DO_DELETE) ) {
                thirdSteps.append(
                          new DeleteJob( "path_to_delete", conflict ) );
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
}

void JobBuilder::doJobs ()
{
    QList<Job*>::Iterator it;

    double percentsPerJob = 100 / m_jobs.size();
    
    emit beforeDoJobs( m_jobs.size() );

    for ( it = m_jobs.begin(); it != m_jobs.end(); ++it ) {
        Job* job = *it;
        bool jobRes = job->doJob();
        if ( ! jobRes ) {
            m_currentJob = job;
            emit jobFailed( job->jobUuid() );
            return;
        }
    }
}

void JobBuilder::undoJobs ()
{
    if ( m_currentJob == 0 )
        // Nothing to undo
        return;

    int index = m_jobs.indexOf( m_currentJob );

    // If nothing was found
    if ( index == -1 ) {
        m_currentJob = 0;
        return;
    }

    emit beforeUndoJobs( index + 1 );

    for ( int i = index; i >= 0; --i ) {
        Job* job = m_jobs[i];
        job->undoJob();
    }

    m_currentJob = 0;
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
