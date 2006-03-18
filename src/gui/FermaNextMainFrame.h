
#ifndef FERMANEXTFRAME_H
#define FERMANEXTFRAME_H

#include <QMainWindow>

class QAction;
class ProjectToolBox;
class FermaNextProject;
class TrussUnitWindow;
class FermaNextWorkspace;
class UndoRedoListBox;
class TrussGeometryWindow;
class QFile;

class FermaNextMainFrame : public QMainWindow
{
    Q_OBJECT
public:
    FermaNextMainFrame ( QWidget * parent = 0, 
                         // FIXME QT3TO4: WFlags f = WType_TopLevel
                         Qt::WFlags f = Qt::Window );

public slots:
    void someProjectRemoved ( FermaNextProject& );
    void someProjectCreated ( FermaNextProject& );

    void createProject ();
    void reloadPlugins ();
    
protected:
    //Exceptions
    class WrongXMLFormatException {};

    // Reload all plugins
    void reloadPlugins ( bool reload );

    // Provides correct last cleanup before quit.
    void cleanBeforeQuit ();

    void init ();
    void setupFileActions ();
    void setupEditActions ();
    void setupViewActions ();
    void setupProjectActions ();
    void setupWindowActions ();
    void setupHelpActions ();
    void setupPluginActions ();

    bool eventFilter( QObject*, QEvent* );
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
    QDockWidget* projectsDockWindow;
    // Undo/redo history widget
    QWidget* undoRedoHistoryWidget;
    // Undo/redo list box to control truss states
    UndoRedoListBox* undoRedoListBox;
    // Tool box of all workspace projects
    ProjectToolBox* projectToolBox;
    // Current system workspace
    FermaNextWorkspace* wsp;
    // Truss geometry window
    TrussGeometryWindow* geometryWindow;

    // Some actions
    QAction* undoAction;
    QAction* redoAction;
    QAction* saveProjectAction;
    QAction* saveAsProjectAction;
    QAction* closeProjectAction;
    QAction* printAction;
    QAction* showUndoRedoAction;
    QAction* showGeometryWindowAction;
    
    // Plugins menu
    QMenu* pluginsMenu;
};

#endif //FERMANEXTFRAME_H
