
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


#include <QObject>
#include <QList>
#include <QString>
#include <QDomDocument>

class Job
{
public:
    enum JobType {
        DownloadJob  = 0, /**< Download file */
        RenameJob,        /**< Rename file or directory */
        DeleteJob,        /**< Delete file or directory */
        CreateDirJob      /**< Create empty directory  */
    };

    virtual ~Job ();

    bool isConflict () const;
    void resolveConflict ();
    virtual QString conflictMessage () const = 0;

    const QString& jobUuid () const;

    JobType jobType () const;
    virtual QString jobMessage () const = 0;

    virtual bool doJob () = 0;
    virtual bool undoJob () = 0;

//signals:
    virtual void progress ( const QString& jobUuid, double done ) = 0;

protected:
    Job ( JobType, bool isConflict );

private:
    QString m_jobUuid;
    JobType m_jobType;
    bool m_isConflict;
};

class DownloadJob : public QObject, public Job
{
    Q_OBJECT
public:
    DownloadJob ( const QString& url, const QString& pathToSave,
                  bool isConflict );
    virtual ~DownloadJob ();

    virtual QString conflictMessage () const;
    virtual QString jobMessage () const;

    virtual bool doJob ();
    virtual bool undoJob ();

signals:
    void progress ( const QString& jobUuid, double done );

private:
    QString m_urlToDownload;
    QString m_pathToSave;
};

class RenameJob : public QObject, public Job
{
    Q_OBJECT
public:
    RenameJob ( const QString& from, const QString& to, bool isConflict );
    virtual ~RenameJob ();

    virtual QString conflictMessage () const;
    virtual QString jobMessage () const;

    virtual bool doJob ();
    virtual bool undoJob ();

signals:
    void progress ( const QString& jobUuid, double done );

private:
    QString m_fromPath;
    QString m_toPath;
};

class DeleteJob : public QObject, public Job
{
    Q_OBJECT
public:
    DeleteJob ( const QString& path, bool isConflict );
    virtual ~DeleteJob ();

    virtual QString conflictMessage () const;
    virtual QString jobMessage () const;

    virtual bool doJob ();
    virtual bool undoJob ();

signals:
    void progress ( const QString& jobUuid, double done );

private:
    QString m_pathToDelete;
};

class CreateDirJob : public QObject, public Job
{
    Q_OBJECT
public:
    CreateDirJob ( const QString& path, bool isConflict );
    virtual ~CreateDirJob ();

    virtual QString conflictMessage () const;
    virtual QString jobMessage () const;

    virtual bool doJob ();
    virtual bool undoJob ();

signals:
    void progress ( const QString& jobUuid, double done );

private:
    QString m_dirToCreate;
};

class JobBuilder : public QObject
{
    Q_OBJECT
public:
    JobBuilder ( const QDomDocument& md5Compared );
    ~JobBuilder ();

    void doJobs ();
    void undoJobs ();

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

    /** Emits when job has failed */
    void jobFailed ( const QString& jobUuid );

    /** Works for both sides: do and undo */
    void progress ( const QString& jobUuid, double percentageDone );

private:
    QList<Job*> m_jobs;
    Job* m_currentJob;
};

#endif //JOBBUILDER_H
