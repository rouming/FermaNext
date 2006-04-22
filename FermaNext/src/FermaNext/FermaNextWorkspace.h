
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

    virtual ProjectList getProjectList () const;

    virtual void loadFromFile ( const QString& ) throw (IOException, 
                                                        WrongXMLDocException,
                                                        LoadException);
    virtual void saveToFile () throw (FileNameIsNotDefinedException,
                                      IOException,
                                      ProjectFileNameIsNotDefinedException);

    virtual void saveToFile ( const QString& ) 
                         throw (IOException, 
                                ProjectFileNameIsNotDefinedException);

    virtual const QString& getWorkspaceFileName () const;
    virtual bool isFileNameDefined () const;
    
    virtual void reset ();

    /**
     * Clean, shutdown everything and exit.
     */
    virtual void quitFermaNextApplication ();

    virtual FermaNextProject& createProject ( const QString& name );
    virtual FermaNextProject& createProjectFromFile ( const QString& ) 
                                    throw (LoadException);

    virtual bool removeProject ( FermaNextProject& );
    virtual bool removeProject ( const QString& name );
    virtual void activateProject ( const QString& prjName );
    virtual FermaNextProject* currentActivatedProject () const;

    virtual FermaNextProject* findProjectByName ( const QString& name ) const;

    virtual int countProjects () const;
    virtual FermaNextProject& getProject ( int index );

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual Config& config ();
    virtual PluginManager& pluginManager ();
    virtual FermaNextMainWindow& mainWindow ();

public slots:
    virtual void clearProjects ();

protected slots:
    virtual void projectIsActivated ( FermaNextProject& );

protected:
    // XML serialization
    virtual void loadFromXML ( const QDomElement&, const QString& wspFileName )
        throw (LoadException);

    virtual QDomElement saveToXML ( QDomDocument& );

    virtual void setWorkspaceFileName ( const QString& );

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
    PluginManager pluginMng;
    FermaNextMainWindow* fermaMainWindow;
    QStackedWidget* stackedWidget;
};

#endif //FERMANEXTWORKSPACE_H
