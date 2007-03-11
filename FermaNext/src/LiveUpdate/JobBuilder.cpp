
#include <QRegExp>
#include <QUuid>

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
    return QString("DownloadJob (url: %1, save: %2)").arg(m_urlToDownload).
        arg(m_pathToSave);
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
    return QString("RenameJob (from: %1, to: %2)").arg(m_fromPath).
        arg(m_toPath);
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
    return QString("DeleteJob (%1)").arg(m_pathToDelete);
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
    return QString("CreateDirJob (%1)").arg(m_dirToCreate);
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
        url = cfgUrl + "/" + cfgName + "_" + os + "/";

    } else {
        LOG4CXX_ERROR(logger, "Config for LiveUpdate is not available");
        return;
    }

    QRegExp conflictRegExp("^" + QString(CONFLICT) + "\\(.+\\)$");

    QList<QDomElement>::ConstIterator it = elements.begin();

    QList<Job*> firstSteps;
    QList<Job*> secondSteps;
    QList<Job*> thirdSteps;

    for ( ; it != elements.end(); ++it ) {
        QDomElement elem = *it;

        // Continue if attribute does not have any status or name
        if ( !elem.hasAttribute("status") ||
             !elem.hasAttribute("name") )
            continue;

        QString status = elem.attribute("status");
        QString name = elem.attribute("name");
        bool conflict = conflictRegExp.exactMatch(status);

        if ( elem.tagName() == "File" ) {
            if ( status.contains(DO_GET) ) {
                firstSteps.append(
                          new DownloadJob( url + name, name, conflict) );
            }
            else if ( status.contains(DO_REPLACE) ) {
                firstSteps.append( 
                          new DownloadJob( url + name, name + ".NEW", 
                                           conflict ) );
                secondSteps.append(
                          new RenameJob( name, name + ".DELETE", conflict ) );
                secondSteps.append(
                          new RenameJob( name + ".NEW", name, conflict ) );
                thirdSteps.append(
                          new DeleteJob( name + ".DELETE", conflict ) );
            }
            else if ( status.contains(DO_DELETE) ) {
                secondSteps.append( 
                          new RenameJob( name, name + ".DELETE", conflict ) );
                thirdSteps.append( 
                          new DeleteJob( name + ".DELETE", conflict ) );
            }
            else 
                // Ok, status is unknown
                continue;
        }
        else if ( elem.tagName() == "Directory" &&
                  elem.hasAttribute("status") ) {
            if ( status.contains(DO_GET) ) {
                firstSteps.append( new CreateDirJob( name, conflict ) );
            }
            else if ( status.contains(DO_REPLACE) ) {
                // Never should happen for directory
                LOG4CXX_WARN(logger, QString("Replace for directory: %1. "
                                             "Should never happen").arg(name).
                             toStdString());
                continue;
            }
            else if ( status.contains(DO_DELETE) ) {
                thirdSteps.append( new DeleteJob( name, conflict ) );
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
