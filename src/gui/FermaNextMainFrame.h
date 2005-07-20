
#ifndef FERMANEXTFRAME_H
#define FERMANEXTFRAME_H

#include <qmainwindow.h>
#include <qwidgetstack.h>
#include <qvbox.h>

#include "ProjectToolBox.h"

class QAction;

class FermaNextMainFrame : public QMainWindow
{
    Q_OBJECT
public:
    FermaNextMainFrame ( QWidget * parent = 0, const char * name = 0, 
                         WFlags f = WType_TopLevel );
    
    
public slots:
    void someProjectRemoved ( FermaNextProject& );
    void someProjectCreated ( FermaNextProject& );

    void createProject ();
    
protected:
    void init ();
    void setupFileActions ();
    void setupEditActions ();
    void setupProjectActions ();
    void setupWindowActions ();
    void setupHelpActions ();

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

    // Set actions enabled/disabled
    void refreshUndoRedoActions ();

private:    
    QDockWindow* dw;
    ProjectToolBox* projectToolBox;
    FermaNextWorkspace* wsp;
    // Some actions
    QAction* undoAction;
    QAction* redoAction;
};

#endif //FERMANEXTFRAME_H
