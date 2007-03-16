
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
    enum Mode {
        Application = 0, /**< Downloads root MD5 file in daemon mode
                              and raises message box about possible update */
        LiveUpdate       /**< Just does what live update tells */
    };

    // Exceptions
    class ConfigIsWrongException {};

    LiveUpdateChecker ( Mode ) /*throw (ConfigIsWrongException)*/;
    ~LiveUpdateChecker ();

public slots:
    /** Starts check */
    void startCheck ();
    
    /** Stops check */
    void stopCheck ();

public:
    /** Waits for check */
    void wait ();

    /** Returns true if some error has happened */
    bool isError () const;

    /** Returns last error */
    const QString& lastError () const;

    /** Return root MD5 file name */
    const QString& rootMD5File () const;

    /** Returns true if check succeed and version is up to date */
    bool isUpToDate ();

    /** Returns checked version if check succeed */
    const QString& checkedVersion () const;

    /** Returns downloaded MD5 file */
    QDomDocument getDownloadedMD5File () const;

signals:
    /** 
     * The signal is emitted when check is complete: 
     *   true if new version is available,
     *   false if current version is up to date.
     */
    void newVersionIsAvailable ( bool newVersion, const QString& ver );

    /** The signal is emitted if error has been occured while check */
    void error ( const QString& );

private slots:
    void httpDone ( bool );
    void httpResponseHeader ( const QHttpResponseHeader& );

private:
    volatile int m_httpGetId;
    Mode m_mode;
    QUrl m_url;
    QHttp m_http;
    QByteArray m_md5ByteArray;
    QBuffer m_md5Buffer;
    bool m_isUpToDate;
    QString m_version;
    QString m_rootMD5File;
    QString m_lastError;
};

#endif //LIVEUPDATECHECKER_H
