
#ifndef PROJECTTOOLBOX_H
#define PROJECTTOOLBOX_H

#include "FermaNextWorkspace.h"
#include "FermaNextProject.h"

#include <qtoolbox.h>

// ****************************************************************************
// BUG: C4786 Warning Is Not Disabled with #pragma Warning
// STATUS: Microsoft has confirmed this to be a bug in the Microsoft product. 
// This warning can be ignored. This occured only in the <map> container.
// ****************************************************************************
// QMap is used instead of std::map for avoiding this undisabling warning
#include <qmap.h>

class ProjectToolBox;

// This class is for internal use only. 
// Object of this class is created for emitting 
// a signal to remove a project which it owns.
// Generally ProjectToolBox eats this signal and removes
// project manually. Of course, ProjectToolBox should
// delete this remover too to avoid memory leaks.
class ProjectRemover : public QObject
{     
    Q_OBJECT
public:
    ProjectRemover ( ProjectToolBox&, FermaNextProject& );
    FermaNextProject& getProjectToRemove ();
public slots:
    void removeProject ();
signals:
    void onProjectRemove ( ProjectRemover& );
private:
    FermaNextProject& projectToRemove;    
};


// Container for smart FermaNext projects navigation
class ProjectToolBox : public QToolBox
{
    Q_OBJECT
protected:
    virtual QWidget* createSubsidiaryWidget ( FermaNextProject& );

public:
    ProjectToolBox ( FermaNextWorkspace&,
                     QWidget* parent = 0, 
                     const char * name = 0, 
                     WFlags f = 0 );

    virtual FermaNextProject* currentProject () const;

protected slots:
    virtual int addProject ( FermaNextProject& );
    virtual int removeProject ( FermaNextProject& );
    virtual void removeProjectFromWorkspace ( ProjectRemover& );
    
    void activateSelected ( int index );


public slots:
    virtual void clear ();

private:
    typedef QMap<FermaNextProject*, QWidget*> ProjectMap;
    typedef QMap<FermaNextProject*, QWidget*>::iterator ProjectMapIter;

    ProjectRemover* lastRemover;
    ProjectMap projects;
    FermaNextProject* currentPrj;
    FermaNextWorkspace& workspace;
};

#endif //PROJECTTOOLBOX_H
