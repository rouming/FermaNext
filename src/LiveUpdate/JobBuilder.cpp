
#include <QRegExp>

#include "JobBuilder.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("LiveUpdate.JobBuilder") );

/*****************************************************************************/

Job::Job ( Job::JobType jobType, bool isConflict ) :
    m_jobType( jobType ),
    m_isConflict( isConflict )
{}

Job::~Job ()
{}

bool Job::isConflict () const
{ return m_isConflict; }

void Job::resolveConflict ()
{ m_isConflict = false; }

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

JobBuilder::JobBuilder ( const QDomDocument& md5Compared )
{
    QDomElement md5CmpElem = md5Compared.documentElement();
    QList<QDomElement> elems;
    parseDocument( md5CmpElem, elems );
    qSort( elems.begin(), elems.end(), SortElements );
    createJobsList( elems );
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
    for ( ; it != elements.end(); ++it ) {
        QDomElement elem = *it;

        if ( elem.tagName() == "File" &&
             elem.hasAttribute("status") ) {

            QString status = elem.attribute("status");

            bool isConflict = conflictRegExp.exactMatch(status);
        
            Job* job = 0;

            if ( status.contains(DO_REPLACE) ) {
            }
            else if ( status.contains(DO_DELETE) ) {
            }
            else if ( status.contains(DO_GET) ) {
            }
            else 
                // Ok, status is unknown
                return;

            m_jobs.append(job);
        }

    }
}

void JobBuilder::doJobs ()
{
    //TODO
}

void JobBuilder::undoJobs ()
{
    //TODO
}

const QList<Job*>& JobBuilder::getJobs () const
{ return m_jobs; }

/*****************************************************************************/
