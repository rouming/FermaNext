
#ifndef PROJECTTOOLBOX_H
#define PROJECTTOOLBOX_H

#include <QToolBox>
#include <QMap>

#include "FermaNextWorkspace.h"
#include "FermaNextProject.h"

// Container for smart FermaNext projects navigation
class ProjectToolBox : public QToolBox
{
    Q_OBJECT
protected:
    virtual QWidget* createSubsidiaryWidget ( FermaNextProject& );

public:
    ProjectToolBox ( FermaNextWorkspace&,
                     QWidget* parent = 0, 
                     Qt::WFlags f = 0 );

    virtual FermaNextProject* currentProject () const;

    virtual bool eventFilter( QObject*, QEvent* );

protected slots:
    virtual int addProject ( FermaNextProject& );
    virtual int removeProject ( FermaNextProject& );
    virtual void projectRename ( const QString& );
    virtual void projectIsActivated ( FermaNextProject& );
 
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