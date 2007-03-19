
#ifndef LIVEUPDATEDIALOG_H
#define LIVEUPDATEDIALOG_H

#include <QDialog>

#include "ui_LiveUpdateDialog.h"

#include "JobBuilder.h"
#include "LiveUpdateChecker.h"

class LiveUpdateDialog : public QDialog, 
                         public Ui::LiveUpdateDialog
{
    Q_OBJECT
public:
    LiveUpdateDialog ( QWidget* parent = 0 );
    ~LiveUpdateDialog ();

public slots:
    void startUpdate ();

private slots:
    /** Raises message box with error string and do quit */
    void error ( const QString& );
    /** Raises message box with warning string */
    void warning ( const QString& );
    /** Raises message box with question */
    bool question ( const QString& );
    /** Raises message box with question to resolve conflicts */
    bool resolveConflict ( const QStringList& );

    /** Sets progress */
    void setProgress ( const QString& text, int done );
    
    /** Job progress */
    void onJobProgress ( const QString& jobUuid, double done );
    /** Before do jobs */
    void onBeforeDoJobs ( uint jobsToDo );
    /** Before undo jobs */
    void onBeforeUndoJobs ( uint jobsToUndo );
    /** Before job failed */
    void onJobFailed ( const QString& jobUuid );
    /** Before job stopped */
    void onJobStopped ( const QString& jobUuid );
    /** On point of not return */
    void onPointOfNoReturn ( const QString& jobUuid );

    /** Cancel pressed */
    void onCancelPressed ();
    /** Details pressed */
    void onDetailsPressed ();

private:
    LiveUpdateChecker* m_checker;
    JobBuilder* m_jobBuilder;
    QStringList m_conflictList;
    QString m_jobFailed;
    QString m_jobStopped;
};

#endif //LIVEUPDATEDIALOG_H
