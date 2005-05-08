
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
    
protected:
    void init ();
    void setupFileActions ();
    void setupEditActions();
    void setupProjectActions();
    void setupWindowActions();
    void setupHelpActions();

protected slots:
    void fileNew ();
    void fileOpen ();
    void fileSave ();
    void fileSaveAs ();
    void fileClose ();
    void fileOpenWsp ();
    void fileSaveWsp ();
    void fileSaveWspAs ();
    void fileCloseWsp ();
    void fileSaveAll ();
    void filePageSetup ();
    void filePrintPreview ();
    void filePrint ();
    void fileRecentPrjs ();
    void fileRecentWsps ();
    void fileExit ();

    void editUndo ();
    void editRedo ();
    void editCopy ();
    void editPaste ();
    void editCut ();
    void editDelete ();
    void editSelectAll ();

    void helpContents ();
    void helpAbout ();

private:    
    QWorkspace* workspaceWidget;
    QVBox* vbox;
    QDockWindow* dw;
    ProjectToolBox* projectToolBox;
    FermaNextWorkspace* wsp;
};

#endif //FERMANEXTFRAME_H
