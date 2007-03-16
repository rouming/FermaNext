
#include <QCoreApplication>

#include "LiveUpdateChecker.h"
#include "Config.h"
#include "Global.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("LiveUpdate.LiveUpdateChecker") );

/*****************************************************************************/

LiveUpdateChecker::LiveUpdateChecker () :
    m_httpGetId(0),
    m_md5Buffer( &m_md5ByteArray ),
    m_isUpToDate(true),
    m_version( Global::applicationVersionNumber() )

    /*throw (LiveUpdateChecker::ConfigIsWrongException)*/
{
    Config& config = Global::config();
    ConfigNode rootNode = config.rootNode();
    ConfigNodeList cfgNodes = rootNode.findChildNodes( "LiveUpdate" );
    if ( cfgNodes.size() != 0 ) {
        NodeAttributeList attrs = cfgNodes.at(0).getAttributes();
        QString cfgUrl;
        QString cfgName;
        QString cfgRootFile;
        foreach ( NodeAttribute attr, attrs ) {
            if ( attr.first == "url" )
                cfgUrl = attr.second;
            else if ( attr.first == "name" )
                cfgName = attr.second;
            else if ( attr.first == "rootFile" )
                cfgRootFile = attr.second;
        }

        if ( cfgUrl.isEmpty() || cfgName.isEmpty() || cfgRootFile.isEmpty() ) {
            LOG4CXX_ERROR(logger, "Config for LiveUpdate is not valid");
            throw ConfigIsWrongException();
        }

#if defined Q_OS_WIN
        QString os = "win";
#elif defined Q_OS_LINUX
        QString os = "lin";
#elif defined Q_OS_MAC
        QString os = "mac";
#else
#error Unsupported os
#endif
        m_rootMD5File = cfgRootFile;
        m_url = cfgUrl + "/" + cfgName + "-" + os + "/" + cfgRootFile;
        LOG4CXX_INFO(logger, QString("Url : %1").arg(m_url.toString()).
                     toStdString());

    } else {
        LOG4CXX_ERROR(logger, "Config for LiveUpdate is not available");
        throw ConfigIsWrongException();
    }

    QObject::connect(&m_http, SIGNAL(done(bool)), SLOT(httpDone(bool)));
    QObject::connect( &m_http, 
                   SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
                   SLOT(httpResponseHeader(const QHttpResponseHeader &)) );
}

LiveUpdateChecker::~LiveUpdateChecker ()
{
    m_http.disconnect();
    m_http.close();
}

void LiveUpdateChecker::startCheck ()
{
    // Return if in progress
    if ( m_httpGetId != 0 )
        return;

    m_md5ByteArray.clear();
    m_http.setHost( m_url.host() );
    m_httpGetId = m_http.get(m_url.path(), &m_md5Buffer);
}
    
void LiveUpdateChecker::stopCheck ()
{
    m_http.abort();
}

void LiveUpdateChecker::wait ()
{
    while ( m_httpGetId != 0 ) {
        QCoreApplication::processEvents();
        Global::sleepMsecs( 200 );
    }
}

const QString& LiveUpdateChecker::rootMD5File () const
{ return m_rootMD5File; }

bool LiveUpdateChecker::isUpToDate ()
{ return m_isUpToDate; }

const QString& LiveUpdateChecker::checkedVersion () const
{ return m_version; }

QDomDocument LiveUpdateChecker::getDownloadedMD5File () const
{
    if ( m_httpGetId != 0 )
        return QDomDocument();

    QDomDocument xmlDoc;

    if ( ! xmlDoc.setContent(m_md5ByteArray) ) {
        return QDomDocument();
    }
    return xmlDoc;
}

void LiveUpdateChecker::httpResponseHeader ( 
    const QHttpResponseHeader& header )
{
    if ( header.statusCode() != 200 ) {
        m_http.abort();
        return;
    }
}

void LiveUpdateChecker::httpDone ( bool err )
{
    if ( err ) {
        emit error( m_http.errorString() );
    }
    else {
        QDomDocument md5Doc;
        md5Doc.setContent( m_md5ByteArray );
        QDomElement md5Elem = md5Doc.documentElement();
        if ( md5Elem.tagName() != "MD5" ||
             ! md5Elem.hasAttribute("appVersion") ) {
            emit error( "Wrong MD5 file" );
        }
        else {
            QString newVer = md5Elem.attribute("appVersion");
            QString curVer = Global::applicationVersionNumber();
            m_isUpToDate = newVer <= curVer;
            m_version = newVer;
            emit newVersionIsAvailable( newVer > curVer, newVer );
        }        
    }

    m_httpGetId = 0;
}

/*****************************************************************************/
