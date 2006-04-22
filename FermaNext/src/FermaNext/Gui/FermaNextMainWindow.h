
#ifndef FERMANEXTMAINWINDOW_H
#define FERMANEXTMAINWINDOW_H

#include <QMainWindow>

class QDockWidget;
class ProjectToolBox;
class FermaNextProject;
class TrussUnitWindow;
class FermaNextWorkspace;
class UndoRedoListBox;
class GeometryTabWidget;
class TrussPropertyTabWidget;
class PreferencesWidget;
class QFile;
class QMenu;

class FermaNextMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    FermaNextMainWindow ( FermaNextWorkspace& );

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

    void init ();
    void initUndoRedoWindow ();
    void initGeometryWindow ();
    void initTrussPropertyWindow ();
    void setupFileActions ();
    void setupEditActions ();
    void setupViewActions ();
    void setupProjectActions ();
    void setupWindowActions ();
    void setupHelpActions ();

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
    void editPreferences ();

    void helpContents ();
    void helpAbout ();

    // Refresh undo/redo
    void refreshUndoRedoActions ();
    // Refresh project actions
    void refreshProjectActions ();
	// Refresh plugins actions
	void refreshPluginsActions ();

    // Catch designer widget focus change
    void trussWindowLostFocus ( TrussUnitWindow& );
    void trussWindowReceivedFocus ( TrussUnitWindow& );

private:
    // Current system workspace
    FermaNextWorkspace& workspace;
    // Dock widgget on wich all projects are located
    QDockWidget* projectsDockWidget;
    // Undo/redo history widget
    QWidget* undoRedoHistoryWidget;
    // Undo/redo list box to control truss states
    UndoRedoListBox* undoRedoListBox;
    // Tool box of all workspace projects
    ProjectToolBox* projectToolBox;
    // Truss geometry window
    QWidget* geometryWindow;
    // Tab widget for changing truss geometry
    GeometryTabWidget* geometryTabWidget;
    // Truss property window
    QWidget* trussPropertyWindow;
    // Tab widget for changing truss properties
    TrussPropertyTabWidget* trussPropTabWidget;
    // Preferences widget
    PreferencesWidget* preferencesWidget;


    // Some actions
    QAction* undoAction;
    QAction* redoAction;
    QAction* saveProjectAction;
    QAction* saveAsProjectAction;
    QAction* closeProjectAction;
    QAction* printAction;
    QAction* showUndoRedoAction;
    QAction* showGeometryWindowAction;
    QAction* showTrussPropWindowAction;
    
    // Plugins menu
    QMenu* pluginsMenu;
};

#endif //FERMANEXTMAINWINDOW_H
