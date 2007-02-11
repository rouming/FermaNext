
#include <QHash>
#include <QList>
#include <QSet>
#include <QRegExp>

#include "MD5Comparator.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("LiveUpdate") );

/*****************************************************************************
 * Helpers
 *****************************************************************************/

#define NO_ELEM  "no_elem"
#define NO_STAT  "no_stat"
#define EQUAL    "equal"
#define CREATED  "created"
#define DELETED  "deleted"
#define MODIFIED "modified"

#define NOTHING   "nothing"
#define UNDEFINED "?"
#define CONFLICT  "conflict"

#define DO_NOTHING "(do nothing)"
#define DO_REPLACE "(replace)"
#define DO_DELETE  "(delete)"
#define DO_GET     "(get)"

#define PRE_COMPARE "compare"

enum ElementType { FileElement = 0, DirectoryElement };

static QString md5CompareStatus ( ElementType elemType,
                                  bool oldElemExists, bool newElemExists,
                                  const QString& oldSt, const QString& newSt,
                                  const QString& oldMD5, const QString& newMD5)
{
    LOG4CXX_DEBUG(logger, QString("md5CompareStatus (oldSt: %1, newSt %2, "
                                  "oldMD5: %3, newMD5 %4)").arg(oldSt).
                            arg(newSt).arg(oldMD5).arg(newMD5).toStdString());

    typedef QHash<QString, QHash<QString, QString> > StatusHash;
    static StatusHash commonHash;
    static QHash<ElementType, StatusHash > elemStatusHash;

    if ( commonHash.isEmpty() ) {
        // NO_ELEM
        commonHash[ NO_ELEM  ][ NO_ELEM  ] = NOTHING DO_NOTHING;
        commonHash[ NO_ELEM  ][ NO_STAT  ] = CREATED DO_GET;
        commonHash[ NO_ELEM  ][ EQUAL    ] = UNDEFINED;
        commonHash[ NO_ELEM  ][ CREATED  ] = CREATED DO_GET;
        commonHash[ NO_ELEM  ][ DELETED  ] = UNDEFINED;
        commonHash[ NO_ELEM  ][ MODIFIED ] = UNDEFINED;

        // NO_STAT
        commonHash[ NO_STAT  ][ NO_ELEM  ] = DELETED DO_DELETE;
        commonHash[ NO_STAT  ][ NO_STAT  ] = PRE_COMPARE "?" EQUAL DO_NOTHING 
                                                       ":" MODIFIED DO_REPLACE;
        commonHash[ NO_STAT  ][ EQUAL    ] = UNDEFINED;
        commonHash[ NO_STAT  ][ CREATED  ] = UNDEFINED;
        commonHash[ NO_STAT  ][ DELETED  ] = UNDEFINED;
        commonHash[ NO_STAT  ][ MODIFIED ] = UNDEFINED;

        // EQUAL
        commonHash[ EQUAL    ][ NO_ELEM  ] = UNDEFINED;
        commonHash[ EQUAL    ][ NO_STAT  ] = UNDEFINED;
        commonHash[ EQUAL    ][ EQUAL    ] = EQUAL DO_NOTHING;
        commonHash[ EQUAL    ][ CREATED  ] = UNDEFINED;
        commonHash[ EQUAL    ][ DELETED  ] = DELETED DO_DELETE;
        commonHash[ EQUAL    ][ MODIFIED ] = MODIFIED DO_REPLACE;

        // CREATED
        commonHash[ CREATED  ][ NO_ELEM  ] = CREATED DO_NOTHING;
        commonHash[ CREATED  ][ NO_STAT  ] = UNDEFINED;
        commonHash[ CREATED  ][ EQUAL    ] = UNDEFINED;
        commonHash[ CREATED  ][ CREATED  ] = PRE_COMPARE "?" EQUAL DO_NOTHING 
                                                       ":" CONFLICT DO_REPLACE;
        commonHash[ CREATED  ][ DELETED  ] = UNDEFINED;
        commonHash[ CREATED  ][ MODIFIED ] = UNDEFINED;

        // DELETED
        commonHash[ DELETED  ][ NO_ELEM  ] = UNDEFINED;
        commonHash[ DELETED  ][ NO_STAT  ] = UNDEFINED;
        commonHash[ DELETED  ][ EQUAL    ] = CREATED DO_GET;
        commonHash[ DELETED  ][ CREATED  ] = UNDEFINED;
        commonHash[ DELETED  ][ DELETED  ] = EQUAL DO_NOTHING;
        commonHash[ DELETED  ][ MODIFIED ] = CREATED DO_GET;

        // MODIFIED
        commonHash[ MODIFIED ][ NO_ELEM  ] = UNDEFINED;
        commonHash[ MODIFIED ][ NO_STAT  ] = UNDEFINED;
        commonHash[ MODIFIED ][ EQUAL    ] = MODIFIED DO_NOTHING;
        commonHash[ MODIFIED ][ CREATED  ] = UNDEFINED;
        commonHash[ MODIFIED ][ DELETED  ] = CONFLICT DO_DELETE;
        commonHash[ MODIFIED ][ MODIFIED ] = PRE_COMPARE "?" EQUAL DO_NOTHING 
                                                       ":" CONFLICT DO_REPLACE;

        
        elemStatusHash[FileElement] = commonHash;
        elemStatusHash[DirectoryElement] = commonHash;

        StatusHash& dirStatusHash = elemStatusHash[DirectoryElement];

        // Minor changes for directories
        dirStatusHash[ NO_STAT  ][ NO_STAT  ] = EQUAL DO_NOTHING; 
        dirStatusHash[ EQUAL    ][ MODIFIED ] = UNDEFINED;
        dirStatusHash[ CREATED  ][ CREATED  ] = EQUAL DO_NOTHING;

        dirStatusHash[ MODIFIED ][ NO_ELEM  ] = UNDEFINED;
        dirStatusHash[ MODIFIED ][ NO_STAT  ] = UNDEFINED;
        dirStatusHash[ MODIFIED ][ EQUAL    ] = UNDEFINED;
        dirStatusHash[ MODIFIED ][ CREATED  ] = UNDEFINED;
        dirStatusHash[ MODIFIED ][ DELETED  ] = UNDEFINED;
        dirStatusHash[ MODIFIED ][ MODIFIED ] = UNDEFINED;
    }
    if ( ! oldElemExists && ! newElemExists ) {
        LOG4CXX_WARN(logger, "Two elements do not exist. Comparison result is"
                             "undefined");
        return UNDEFINED;
    }

    Q_ASSERT( elemType == FileElement || elemType == DirectoryElement );

    StatusHash& statusHash = elemStatusHash[elemType];

    QString oldStatus;
    QString newStatus;
    
    // Element does not exist
    if ( ! oldElemExists )
        oldStatus = NO_ELEM;
    // Element exists but status does not exist
    else if ( oldElemExists && oldSt.isEmpty() )
        oldStatus = NO_STAT;
    // Ok. Save status
    else
        oldStatus = oldSt;

    // Element does not exist
    if ( ! newElemExists )
        newStatus = NO_ELEM;
    // Element exists but status does not exist
    else if ( newElemExists && newSt.isEmpty() )
        newStatus = NO_STAT;
    // Ok. Save status
    else
        newStatus = newSt;

    
    // Get status from hash
    if ( statusHash.contains(oldStatus) &&
         statusHash[oldStatus].contains(newStatus) ) {
        QString comparedStatus = statusHash[oldStatus][newStatus];

        LOG4CXX_INFO(logger, QString("Found status: %1").arg(comparedStatus).
                               toStdString());

        QRegExp preCompareRegExp( "^" PRE_COMPARE 
                                  "\\?(.+\\(.+\\)):(.+\\(.+\\))$" );

        if ( preCompareRegExp.exactMatch(comparedStatus) ) {
            LOG4CXX_INFO(logger, "Found pre_compared");

            if ( elemType == FileElement && oldMD5 == newMD5 ) {
                LOG4CXX_INFO(logger, "Comparison returns true");
                return preCompareRegExp.cap(1);
            }
            else {
                LOG4CXX_INFO(logger, "Comparison returns false");
                return preCompareRegExp.cap(2);
            }
        }
        else
            return comparedStatus;
    }

    else {
        LOG4CXX_WARN(logger, "Can't compare statuses. Comparison result is "
                             "undefined");
        return UNDEFINED;
    }
}

static void md5Compare ( QDomDocument& doc, QDomElement& parent,
                         const QDomElement& oldMD5Elem, 
                         const QDomElement& newMD5Elem )
{
    Q_ASSERT( ! oldMD5Elem.isNull() || ! newMD5Elem.isNull() );

    // Check old elem

    if ( ! oldMD5Elem.isNull() &&
         oldMD5Elem.tagName() != "Directory" && 
         oldMD5Elem.tagName() != "File" && 
         oldMD5Elem.tagName() != "MD5" ) {
        LOG4CXX_WARN(logger, "Old element is not a file or directory");
        return;
    }

    if ( ! oldMD5Elem.isNull() && 
         oldMD5Elem.tagName() != "MD5" && 
         ! oldMD5Elem.hasAttribute("name") ) {
        LOG4CXX_WARN(logger, "Old element does not have 'name' attribute");
        return;
    }

    if ( ! oldMD5Elem.isNull() && 
         oldMD5Elem.tagName() == "File" && 
         ! oldMD5Elem.hasAttribute("md5") ) {
        LOG4CXX_WARN(logger, "Old file element does not have 'md5' "
                     "attribute");
        return;
    }

    // Check new elem

    if ( ! newMD5Elem.isNull() &&
         newMD5Elem.tagName() != "Directory" && 
         newMD5Elem.tagName() != "File" && 
         newMD5Elem.tagName() != "MD5" ) {
        LOG4CXX_WARN(logger, "New element is not a file or directory");
        return;
    }

    if ( ! newMD5Elem.isNull() && 
         oldMD5Elem.tagName() != "MD5" && 
         ! newMD5Elem.hasAttribute("name") ) {
        LOG4CXX_WARN(logger, "New element does not have 'name' attribute");
        return;
    }

    if ( ! newMD5Elem.isNull() && 
         newMD5Elem.tagName() == "File" && 
         ! newMD5Elem.hasAttribute("md5") ) {
        LOG4CXX_WARN(logger, "New file element does not have 'md5' "
                     "attribute");
        return;
    }

    QRegExp bracketsRegExp("\\(.*\\)");

    bool oldElemExists = ! oldMD5Elem.isNull();
    bool newElemExists = ! newMD5Elem.isNull();

    ElementType elemType = DirectoryElement;
    if ( oldElemExists && oldMD5Elem.tagName() == "File" ||
         newElemExists && newMD5Elem.tagName() == "File" )
        elemType = FileElement;

    QString oldSt = (oldElemExists ? 
                     oldMD5Elem.attribute("status").remove(bracketsRegExp) :
                     "");
    QString newSt = (newElemExists ?
                     newMD5Elem.attribute("status").remove(bracketsRegExp) :
                     "");

    QString oldMD5 = (oldElemExists ? oldMD5Elem.attribute("md5") : "");
    QString newMD5 = (newElemExists ? newMD5Elem.attribute("md5") : "");

    QString statusAttr = ::md5CompareStatus( 
                                        elemType, oldElemExists, newElemExists,
                                        oldSt, newSt, oldMD5, newMD5 );

    if ( statusAttr == UNDEFINED ) {
        LOG4CXX_ERROR(logger, "Returned status is UNDEFINED!");
        return;
    }


    QDomElement aliveElem;

    if ( oldElemExists )
        aliveElem = oldMD5Elem;
    else
        aliveElem = newMD5Elem;

    QDomElement rootElem;
    // Import if not MD5 tag
    if ( aliveElem.tagName() != "MD5" ) {
        rootElem = doc.importNode( aliveElem, false ).toElement();
        Q_ASSERT( ! rootElem.isNull() );
        rootElem.setAttribute( "status", statusAttr );
        parent.appendChild( rootElem );
    }
    else
        rootElem = parent;

    // Get children for directory
    if ( elemType == DirectoryElement ) {

        QHash<QString, QDomElement> oldMD5Elems;
        QHash<QString, QDomElement> newMD5Elems;
        QHash<QString, QDomElement> mergedElems;

        QDomNode n;
        // Get all tags for oldMD5
        if ( oldElemExists ) {
            for ( n = oldMD5Elem.firstChild(); !n.isNull(); 
                  n = n.nextSibling() ) {
                if ( n.isNull() || ! n.isElement() ) {
                    LOG4CXX_WARN(logger, "Element is null or not an element");
                    continue;
                }
                QDomElement e = n.toElement();
                Q_ASSERT( ! e.isNull() );
                if ( e.tagName() != "Directory" && e.tagName() != "File" ) {
                    LOG4CXX_WARN(logger, "Element is not a file or directory");
                    continue;
                }
                if ( ! e.hasAttribute("name") ) {
                    LOG4CXX_WARN(logger, "Element does not have 'name' "
                                 "attribute");
                    continue;
                }
                if ( e.tagName() == "File" && ! e.hasAttribute("md5") ) {
                    LOG4CXX_WARN(logger, "File element does not have 'md5' "
                                 "attribute");
                    continue;
                }

                oldMD5Elems[e.attribute("name")] = e;
            }
        }

        // Get all tags for newMD5
        if ( newElemExists ) {
            for ( n = newMD5Elem.firstChild(); !n.isNull(); 
                  n = n.nextSibling() ) {
                if ( n.isNull() || ! n.isElement() ) {
                    LOG4CXX_WARN(logger, "Element is null or not an element");
                    continue;
                }
                QDomElement e = n.toElement();
                Q_ASSERT( ! e.isNull() );
                if ( e.tagName() != "Directory" && e.tagName() != "File" ) {
                    LOG4CXX_WARN(logger, "Element is not a file or directory");
                    continue;
                }
                if ( ! e.hasAttribute("name") ) {
                    LOG4CXX_WARN(logger, "Element does not have 'name' "
                                 "attribute");
                    continue;
                }
                if ( e.tagName() == "File" && ! e.hasAttribute("md5") ) {
                    LOG4CXX_WARN(logger, "File element does not have 'md5' "
                                 "attribute");
                    continue;
                }
                // Tags of new and old should be equal
                if ( oldMD5Elems.contains(e.attribute("name")) &&
                     e.tagName() != oldMD5Elems[e.attribute("name")].
                                      tagName() ) {
                    LOG4CXX_WARN(logger, "Tags are different");
                    continue;
                }

                newMD5Elems[e.attribute("name")] = e;
            }
        }

        // Compare old and new children
        QSet<QString> setKeys = oldMD5Elems.keys().toSet() | 
                                newMD5Elems.keys().toSet();
        QList<QString> keys = setKeys.toList();
        qSort( keys.begin(), keys.end() );
        foreach ( QString key, keys )
            ::md5Compare( doc, rootElem, oldMD5Elems[key], newMD5Elems[key] );
    }
}

/*****************************************************************************
 * md5 Comparator
 *****************************************************************************/

QDomDocument MD5Comparator::md5Compare ( const QDomDocument& oldMD5,
                                         const QDomDocument& newMD5 )
{
    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF-8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement md5Header = doc.createElement( "MD5" );
    doc.appendChild( md5Header );

    QDomElement oldMD5Elem = oldMD5.documentElement();
    QDomElement newMD5Elem = newMD5.documentElement();

    if ( oldMD5Elem.isNull() || oldMD5Elem.tagName() != "MD5" )
        oldMD5Elem = QDomElement();

    if ( newMD5Elem.isNull() || newMD5Elem.tagName() != "MD5" )
        newMD5Elem = QDomElement();

    ::md5Compare( doc, md5Header, oldMD5Elem, newMD5Elem );

    return doc;
}

/*****************************************************************************/
