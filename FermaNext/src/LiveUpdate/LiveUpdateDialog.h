
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

    /** Sets progress */
    void setProgress ( const QString& text, int done );

    void onJobProgress ( const QString& jobUuid, double done );

    void onBeforeDoJobs ( uint jobsToDo );

    void onBeforeUndoJobs ( uint jobsToUndo );

    void onJobFailed ( const QString& jobUuid );

    void onJobStopped ( const QString& jobUuid );


private:
    LiveUpdateChecker* m_checker;
    JobBuilder* m_jobBuilder;
};

#endif //LIVEUPDATEDIALOG_H
