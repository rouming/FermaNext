
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
    // File format extension
    static const QString FormatExtension;

    // Exceptions
    class WorkspaceIsNotInitedCorrectly {};
    class IOException {};
    class WrongXMLDocException {};
    class ProjectIsNotSavedException {};

    // Basic typedefs    
    typedef std::vector<FermaNextProject*> ProjectList;
    typedef std::vector<FermaNextProject*>::iterator ProjectListIter;

    // Entry point to workspace instance
    static FermaNextWorkspace& workspace ();

    virtual void loadFromFile ( const QString& ) throw (IOException, 
                                                        WrongXMLDocException,
                                                        LoadException);
    virtual void saveToFile ( const QString& ) 
                         throw (IOException, ProjectIsNotSavedException);

    virtual const QString& getWorkspaceFileName () const;
    
    virtual void reset ();
    virtual void createWidgetStack ( QMainWindow& );

    virtual FermaNextProject& createProject ( const QString& name ) 
                                    throw (WorkspaceIsNotInitedCorrectly);
    virtual FermaNextProject& createProject ( QDomElement& ) 
                                    throw (WorkspaceIsNotInitedCorrectly,
                                           LoadException);

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
    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual void setWorkspaceFileName ( const QString& );

    virtual bool removeProject ( ProjectListIter& );

    FermaNextWorkspace& operator= ( const FermaNextWorkspace& );
    virtual ~FermaNextWorkspace ();

signals:
    void onProjectCreate ( FermaNextProject& );
    void onBeforeProjectRemove ( FermaNextProject& );
    void onAfterProjectRemove ();
    void onReset ();
    void onNameChange ( const QString& name );
    void onWorkspaceFileNameChange ( const QString& name );
    
private:
    static FermaNextWorkspace* instance;
    static QMutex mutex;

    QString name;    
    QString workspaceFileName;
    QWidgetStack* widgetStack;    
    ProjectList projects;
    FermaNextConfig fermaConfig;    
    PluginManager plgManager;
};

#endif //FERMANEXTWORKSPACE_H
