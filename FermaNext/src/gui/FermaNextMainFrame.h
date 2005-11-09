
#ifndef FERMANEXTFRAME_H
#define FERMANEXTFRAME_H

#include <qmainwindow.h>

class QAction;
class ProjectToolBox;
class FermaNextProject;
class TrussUnitWindow;
class FermaNextWorkspace;
class UndoRedoListBox;
class QFile;

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
    void reloadPlugins ();
    
protected:
    //Exceptions
    class WrongXMLFormatException {};

    // Provides correct last cleanup before quit.
    void cleanBeforeQuit ();

    void init ();
    void setupFileActions ();
    void setupEditActions ();
    void setupProjectActions ();
    void setupWindowActions ();
    void setupHelpActions ();
    void setupPluginActions ();

    // Close handler. Calls 'clean' for correct close.
    void closeEvent ( QCloseEvent* );

protected slots:
    void fileNew ();
    void fileOpen ();
    bool fileSave ();
    bool fileSaveAs ();
    void fileClose ();
    void fileOpenWsp ();
    bool fileSaveWsp ();
    bool fileSaveWspAs ();
    void fileCloseWsp ();
    bool fileSaveAll ();
    void filePageSetup ();
    void filePrintPreview ();
    void filePrint ();
    void fileExit ();

    void editUndo ();
    void editRedo ();
    void editCopy ();
    void editPaste ();
    void editCut ();    
    void editSelectAll ();

    void helpContents ();
    void helpAbout ();

    // Refresh undo/redo
    void refreshUndoRedoActions ();
    // Refresh project actions
    void refreshProjectActions ();

    // Catch designer widget focus change
    void trussWindowLostFocus ( TrussUnitWindow& );
    void trussWindowReceivedFocus ( TrussUnitWindow& );

private:
    // Dock window on wich all projects are located
    QDockWindow* projectsDockWindow;
    // Undo/redo history widget
    QWidget* undoRedoHistoryWidget;
    // Undo/redo list box to control truss states
    UndoRedoListBox* undoRedoListBox;
    // Tool box of all workspace projects
    ProjectToolBox* projectToolBox;
    // Current system workspace
    FermaNextWorkspace* wsp;

    // Some actions
    QAction* undoAction;
    QAction* redoAction;
    QAction* saveProjectAction;
    QAction* saveAsProjectAction;
    QAction* closeProjectAction;
    QAction* printAction;
    
    // Plugins menu
    QPopupMenu* pluginsMenu;
};

#endif //FERMANEXTFRAME_H
