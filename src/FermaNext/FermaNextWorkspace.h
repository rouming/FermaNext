
#ifndef FERMANEXTWORKSPACE_H
#define FERMANEXTWORKSPACE_H

#include <vector>
#include <qobject.h>
#include <qstring.h>
#include <qmutex.h> 
#include "XMLSerializableObject.h"
#include "FermaNextProject.h"
#include "FermaNextConfig.h"
#include "PluginManager.h"

class QWidgetStack;
class FermaNextMainFrame;

class FermaNextWorkspace : public QObject, public XMLSerializableObject
{
    Q_OBJECT
public:
    // Inner exceptions
    class WorkspaceIsNotInitedCorrectly {};

    typedef std::vector<FermaNextProject*> ProjectList;
    typedef std::vector<FermaNextProject*>::iterator ProjectListIter;

    static FermaNextWorkspace& workspace ();
    virtual void reset ();
    virtual void createWidgetStack ( QMainWindow& );

    virtual FermaNextProject& createProject ( const QString& name ) 
                                    throw (WorkspaceIsNotInitedCorrectly);
    virtual bool removeProject ( FermaNextProject& );
    virtual bool removeProject ( const QString& name );
    virtual void activateProject ( const QString& prjName );
    virtual FermaNextProject* findProjectByName ( const QString& name );

    virtual int countProjects () const;
    virtual FermaNextProject& getProject ( int index );

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual FermaNextConfig& config ();
    virtual PluginManager& pluginManager ();

public slots:
    virtual bool loadPlugins ();
    virtual void clearProjects ();

private:
    FermaNextWorkspace ();
    FermaNextWorkspace ( const FermaNextWorkspace& );

protected:
    FermaNextWorkspace& operator= ( const FermaNextWorkspace& );
    virtual ~FermaNextWorkspace ();

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
    PluginManager plgManager;
};

#endif //FERMANEXTWORKSPACE_H
