
#ifndef FERMANEXTFRAME_H
#define FERMANEXTFRAME_H

#include <qmainwindow.h>
#include <qworkspace.h>
#include <qvbox.h>

#include "ProjectToolBox.h"

class FermaNextMainFrame : public QMainWindow
{
    Q_OBJECT
public:
    FermaNextMainFrame ( QWidget * parent = 0, const char * name = 0, 
                         WFlags f = WType_TopLevel );
    
    
public slots:
    void someProjectRemoved ();
    void someProjectCreated ();

    void createProject ();

private:    
    QWorkspace* workspaceWidget;
    QVBox* vbox;
    QDockWindow* dw;
    ProjectToolBox* projectToolBox;
    FermaNextWorkspace* wsp;
};

#endif //FERMANEXTFRAME_H
