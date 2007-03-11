
#ifndef LIVEUPDATECHECKER_H
#define LIVEUPDATECHECKER_H

#include <QHttp>
#include <QUrl>
#include <QDomDocument>
#include <QBuffer>
#include <QByteArray>

class LiveUpdateChecker : public QObject
{
    Q_OBJECT
public:
    // Exceptions
    class ConfigIsWrongException {};

    LiveUpdateChecker () /*throw (ConfigIsWrongException)*/;
    ~LiveUpdateChecker ();

    /** Starts check */
    void startCheck ();
    
    /** Stops check */
    void stopCheck ();

    /** Waits for check */
    void wait ();

    /** Returns downloaded MD5 file */
    QDomDocument getDownloadedMD5File () const;

signals:
    /** 
     * The signal is emitted when check is complete: 
     *   true if new version is available,
     *   false if current version is up to date.
     */
    void newVersionIsAvailable ( bool newVersion, QString versionNumber );

    /** The signal is emitted if error has been occured while check */
    void error ( QString );

private slots:
    void httpDone ( bool );
    

private:
    volatile int m_httpGetId;
    QUrl m_url;
    QHttp m_http;
    QByteArray m_md5ByteArray;
    QBuffer m_md5Buffer;
};

#endif //LIVEUPDATECHECKER_H
