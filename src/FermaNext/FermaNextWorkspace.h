
#ifndef FERMANEXTWORKSPACE_H
#define FERMANEXTWORKSPACE_H

#include <vector>

#include <QObject>
#include <QString>
#include <QMutex>

#include "XMLSerializableObject.h"
#include "Config.h"
#include "FermaNextProject.h"
#include "FermaNextMainWindow.h"
#include "PluginExecutorDialog.h"
#include "PluginManager.h"

class QStackedWidget;
class FermaNextMainFrame;

class FermaNextWorkspace : public QObject, public XMLSerializableObject
{
    Q_OBJECT
public:
    // File format extension
    static const QString& formatExtension ();

public:
    // Exceptions
    class IOException {};
    class WrongXMLDocException {};
    class FileNameIsNotDefinedException {};
    class ProjectFileNameIsNotDefinedException {};

    // Basic typedefs    
    typedef std::vector<FermaNextProject*> ProjectList;
    typedef ProjectList::iterator ProjectListIter;
    typedef ProjectList::const_iterator ProjectListConstIter;

    // Entry point to workspace instance
    static FermaNextWorkspace& workspace ();

    ProjectList getProjectList () const;

    virtual void loadFromFile ( const QString& ) /*throw (IOException, 
                                                          WrongXMLDocException,
                                                          LoadException)*/;
    virtual void saveToFile () 
        /*throw (FileNameIsNotDefinedException,
                 IOException, ProjectFileNameIsNotDefinedException)*/;

    virtual void saveToFile ( const QString& ) 
        /*throw (IOException, ProjectFileNameIsNotDefinedException)*/;

    const QString& getWorkspaceFileName () const;
    bool isFileNameDefined () const;
    
    void reset ();

    /**
     * Clean, shutdown everything and exit.
     */
    void quitFermaNextApplication ();

    FermaNextProject& createProject ( const QString& name );
    FermaNextProject& createProjectFromFile ( const QString& ) 
        /*throw(LoadException)*/;

    bool removeProject ( FermaNextProject& );
    bool removeProject ( const QString& name );
    void activateProject ( const QString& prjName );
    FermaNextProject* currentActivatedProject () const;

    FermaNextProject* findProjectByName ( const QString& name ) const;

    int countProjects () const;
    FermaNextProject& getProject ( int index );

    const QString& getName () const;
    void setName ( const QString& );

    Config& config ();
    PluginManager& pluginManager ();

    FermaNextMainWindow* mainWindow () const;
    PluginExecutorDialog* pluginExecutor () const;

public slots:
    void clearProjects ();

protected slots:
    void projectIsActivated ( FermaNextProject& );

protected:
    // XML serialization
    virtual void loadFromXML ( const QDomElement&, const QString& wspFileName )
        /*throw (LoadException)*/;

    virtual QDomElement saveToXML ( QDomDocument& );

    void setWorkspaceFileName ( const QString& );

    /** Removed project and returns next iterator. */
    ProjectListIter removeProject ( ProjectListIter );

    virtual ~FermaNextWorkspace ();

signals:
    void onProjectActivated ( FermaNextProject& );
    void onProjectCreate ( FermaNextProject& );
    void onBeforeProjectRemove ( FermaNextProject& );
    void onAfterProjectRemove ();
    void onReset ();
    void onNameChange ( const QString& name );
    void onWorkspaceFileNameChange ( const QString& name );

public:
    /** Hidden constructor. Use static workspace() instead. */
    FermaNextWorkspace ();
    /** Hidden copy constructor. */
    FermaNextWorkspace ( const FermaNextWorkspace& );
    /** Hidden copy operator. */
    FermaNextWorkspace& operator= ( const FermaNextWorkspace& );
    
private:
    static FermaNextWorkspace* instance;
    static QMutex mutex;

    QString name;    
    QString workspaceFileName;
    ProjectList projects;
    Config& fermaConfig;
    PluginManager pluginMng;
    FermaNextMainWindow* fermaMainWindow;
    PluginExecutorDialog* plgExecutorDialog;
    QStackedWidget* stackedWidget;
};

#endif //FERMANEXTWORKSPACE_H
