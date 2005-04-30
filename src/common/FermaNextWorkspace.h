
#ifndef FERMANEXTWORKSPACE_H
#define FERMANEXTWORKSPACE_H

#include <vector>
#include <qobject.h>
#include <qstring.h>
#include <qmutex.h> 
#include "FermaNextProject.h"

class FermaNextWorkspace : public QObject
{
    Q_OBJECT
public:
    typedef std::vector<FermaNextProject*> ProjectList;

    static FermaNextWorkspace& workspace ();
    
    virtual void reset ();

    virtual FermaNextProject& createProject ( const QString& name );
    virtual bool removeProject ( FermaNextProject& );
    virtual bool removeProject ( const QString& name );

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
    
private:
    static FermaNextWorkspace* instance;
    static QMutex mutex;
    ProjectList projects;
    QString name;
};

#endif //FERMANEXTWORKSPACE_H
