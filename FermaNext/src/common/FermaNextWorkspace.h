
#ifndef FERMANEXTWORKSPACE_H
#define FERMANEXTWORKSPACE_H

#include <vector>
#include <qobject.h>
#include <qstring.h>
#include <qmutex.h> 
#include "FermaNextProject.h"
#include "FermaNextConfig.h"

class QWidgetStack;
class FermaNextMainFrame;

class FermaNextWorkspace : public QObject
{
    Q_OBJECT
public:
    typedef std::vector<FermaNextProject*> ProjectList;
    typedef std::vector<FermaNextProject*>::iterator ProjectListIter;

    static FermaNextWorkspace& workspace ();
    virtual void reset ();

    virtual FermaNextProject& createProject ( const QString& name );
    virtual bool removeProject ( FermaNextProject& );
    virtual bool removeProject ( const QString& name );
    virtual void activateProject ( FermaNextProject& );
    virtual void activateProject ( const QString& prjName );
    virtual FermaNextProject* findProjectByName ( const QString& name );

    virtual int countProjects () const;
    virtual FermaNextProject& getProject ( int index );

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual QWidgetStack& getWidgetStack ();    

    virtual FermaNextConfig& config ();    

private:
    FermaNextWorkspace ();
    FermaNextWorkspace ( const FermaNextWorkspace& );

protected:
    FermaNextWorkspace& operator= ( const FermaNextWorkspace& );
    virtual ~FermaNextWorkspace ();

protected slots:
    virtual void clear ();

signals:
    void onProjectCreate ( FermaNextProject& );
    void onProjectRemove ( FermaNextProject& );
    void onReset ();
    void onNameChange ( const QString& name );
    
private:
    static FermaNextWorkspace* instance;
    static QMutex mutex;
    QString name;
    QWidgetStack* widgetStack;    
    ProjectList projects;
    FermaNextConfig fermaConfig;    
};

#endif //FERMANEXTWORKSPACE_H