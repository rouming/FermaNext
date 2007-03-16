
#ifndef JOBBUILDER_H
#define JOBBUILDER_H


// Directories
//
//    get
//
//  STEP_1            -- (CreateDirJob)  DO: create directory
//                                     UNDO: remove directory
//  STEP_2            --
//
//  STEP_3            --
//
//
//    delete          
//
//  STEP_1            --
//
//  STEP_2            -- 
//                       
//                       
//
//  STEP_3            -- (DeleteJob)     DO: delete directory
//                                     UNDO: CAN'T


// Files
//
//    get
//  
//  STEP_1           -- (DownloadJob)    DO: download file
//                                     UNDO: remove file
//
//  STEP_2           --
//
//  STEP_3           --
//
//    replace
//
//  STEP_1           -- (DownloadJob)    DO: download file and save it 
//                                           with special extensions
//                                     UNDO: remove file
//
//  STEP_2           -- (RenameJob)      DO: rename old file to name with 
//                                           special "deleted" extension 
//                                            (except LiveUpdate[.exe] )
//                                           rename downloaded file to original
//                                     UNDO: unrename
//
//  STEP_3           -- (DeleteJob)      DO: delete "deleted" file
//                                     UNDO: CAN'T

//
//    delete
//
//  STEP_1           --
//
//  STEP_2           -- (RenameJob)      DO: rename file to name with special 
//                                            "deleted" extension 
//                                            (except LiveUpdate[.exe] )
//                                     UNDO: unrename
//
//  STEP_3           -- (DeleteJob)      DO: delete "deleted" file
//                                     UNDO: CAN'T


#include <QList>
#include <QUrl>
#include <QString>
#include <QFile>
#include <QHttp>
#include <QStringList>
#include <QDomDocument>

class Job : public QObject
{
public:
    enum JobType {
        DownloadJob  = 0, /**< Download file */
        RenameJob,        /**< Rename file or directory */
        DeleteJob,        /**< Delete file or directory */
        CreateDirJob      /**< Create empty directory  */
    };

    enum JobProgressStatus {
        Running = 0, /**< Job is running */
        Success,     /**< Job is finished with success */
        Failed       /**< Job is finished with failure */
    };

    virtual ~Job ();

    const QString& jobUuid () const;
    
    const QString& lastError () const;
    bool isError () const;

    JobType jobType () const;
    virtual QString jobMessage () const = 0;

    virtual void doJob () = 0;
    virtual void undoJob () = 0;
    virtual void stopJob () = 0;

    virtual void getCurrentProgress ( JobProgressStatus&, double& done ) = 0;

protected:
    Job ( JobType );

    void clearLastError ();
    void setErrorString ( const QString& err );

private:
    QString m_jobUuid;
    JobType m_jobType;
    QString m_lastError;
};

class DownloadJob : public Job
{
    Q_OBJECT
public:
    DownloadJob ( const QString& url, const QString& pathToSave );
    virtual ~DownloadJob ();

    virtual QString jobMessage () const;

    virtual void doJob ();
    virtual void undoJob ();
    virtual void stopJob ();

    virtual void getCurrentProgress ( JobProgressStatus&, double& done );

protected slots:
    void httpReadProgress ( int done, int total );
    void httpDone ( bool error );
    void httpResponseHeader ( const QHttpResponseHeader& );

private:
    QUrl m_urlToDownload;
    QFile m_fileToSave;
    JobProgressStatus m_progressStatus;
    double m_progressDone;
    QHttp m_http;
    bool m_requestAborted;
};

class RenameJob : public Job
{
public:
    RenameJob ( const QString& from, const QString& to );
    virtual ~RenameJob ();

    virtual QString jobMessage () const;

    virtual void doJob ();
    virtual void undoJob ();
    virtual void stopJob ();

    virtual void getCurrentProgress ( JobProgressStatus&, double& done );

private:
    JobProgressStatus m_progressStatus;
    double m_progressDone;
    QString m_fromPath;
    QString m_toPath;
};

class DeleteJob : public Job
{
public:
    DeleteJob ( const QString& path );
    virtual ~DeleteJob ();

    virtual QString jobMessage () const;

    virtual void doJob ();
    virtual void undoJob ();
    virtual void stopJob ();

    virtual void getCurrentProgress ( JobProgressStatus&, double& done );

protected:
    virtual const QString& pathToDelete () const;

private:
    JobProgressStatus m_progressStatus;
    double m_progressDone;
    QString m_pathToDelete;
};

class DeleteLiveUpdateBinaryJob : public DeleteJob
{
public:
    DeleteLiveUpdateBinaryJob ( const QString& path );
    virtual ~DeleteLiveUpdateBinaryJob ();

    virtual void doJob ();
};

class CreateDirJob : public Job
{
public:
    CreateDirJob ( const QString& path );
    virtual ~CreateDirJob ();

    virtual QString jobMessage () const;

    virtual void doJob ();
    virtual void undoJob ();
    virtual void stopJob ();

    virtual void getCurrentProgress ( JobProgressStatus&, double& done );

private:
    JobProgressStatus m_progressStatus;
    double m_progressDone;
    QString m_dirToCreate;
};

class JobBuilder : public QObject
{
    Q_OBJECT
public:
    JobBuilder ( const QDomDocument& md5Compared );
    ~JobBuilder ();

    bool isConflict () const;
    void resolveConflict ();

    const QStringList& conflictMessages () const;

    bool doJobs ();
    bool undoJobs ();
    void stopJobs ();

    const QList<Job*>& getJobs () const;

    Job* findJobByUuid ( const QString& ) const;

private:
    /** Clears jobs */
    void clearJobs ();

    /** Recursively parses xml document */
    void parseDocument ( const QDomElement& md5CmpElem,
                         QList<QDomElement>& );

    /** Creates jobs list by sorted element list */
    void createJobsList ( const QList<QDomElement>& );

signals:
    /** Emits before do */
    void beforeDoJobs ( uint jobsToDo );
    /** Emits before do */
    void beforeUndoJobs ( uint jobsToUndo );

    /** Emits when job has been failed */
    void jobFailed ( const QString& jobUuid );

    /** Emits when job has been stopped */
    void jobStopped ( const QString& jobUuid );

    /** 
     * Works for both sides: do and undo 
     * For do it works: from 0 to 100 percent.
     * For undo works in reverse mode: from 100 to 0 percent.
     */
    void progress ( const QString& jobUuid, double percentageDone );

private:
    QList<Job*> m_jobs;
    Job* m_currentJob;
    bool m_isConflict;
    bool m_jobsTerminated;
    QStringList m_conflictMsgs;
};

#endif //JOBBUILDER_H
