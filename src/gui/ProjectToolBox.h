
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
 
    void activateSelected ( int index );

public slots:
    virtual void clear ();
    virtual void importIsPressed ();
    virtual void newTrussIsPressed ();
    virtual void calculateAllIsPressed ();

private:
    typedef QMap<FermaNextProject*, QWidget*> ProjectMap;
    typedef QMap<FermaNextProject*, QWidget*>::iterator ProjectMapIter;

    ProjectMap projects;
    FermaNextProject* currentPrj;
    FermaNextWorkspace& workspace;    
};

#endif //PROJECTTOOLBOX_H
