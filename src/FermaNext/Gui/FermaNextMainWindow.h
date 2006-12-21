
#ifndef FERMANEXTMAINWINDOW_H
#define FERMANEXTMAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>

#include "Plugin.h"

class ProjectToolBox;
class FermaNextProject;
class TrussUnitWindow;
class FermaNextWorkspace;
class UndoRedoListBox;
class GeometryTabWidget;
class TrussPropertyTabWidget;
class PreferencesWidget;
class ResultsTabWidget;
class QFile;
class QLabel;
class QMenu;
class QSignalMapper;

class TrussSolutionResults;

/*****************************************************************************/

class FermaNextMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    FermaNextMainWindow ( FermaNextWorkspace& );
    ~FermaNextMainWindow ();

    void pluginWasExecuted ( Plugin*, const Plugin::ExecutionResult& );

public slots:
    void someProjectRemoved ( FermaNextProject& );
    void someProjectCreated ( FermaNextProject& );

    void createProject ();
    void reloadPlugins ();

private slots:
    void onReloadPluginsFromMainEventLoop ();

    // Show plugin info dialog by plugin path
    void showPluginInfo ( const QString& plgPath );

signals: 
    void reloadPluginsFromMainEventLoop ();
    void calculateTrussUnit ( const TrussUnitWindow& );
    
protected:
    //Exceptions
    class WrongXMLFormatException {};

    // Reload all plugins
    void reloadPlugins ( bool reload );

    void init ();
    void initUndoRedoWindow ();
    void initGeometryWindow ();
    void initTrussPropertyWindow ();
    void createStatusBar ();
    void setupFileActions ();
    void setupEditActions ();
    void setupViewActions ();
    void setupProjectActions ();
    void setupWindowActions ();
    void setupPluginsActions ();
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
    void editMaterials ();
    void editPreferences ();

    void showResultsWindow ( const TrussUnitWindow& );

    void helpContents ();
    void helpAbout ();

    // Refresh undo/redo
    void refreshUndoRedoActions ();
    // Refresh project actions
    void refreshProjectActions ();
	// Refresh plugins actions
	void refreshPluginsActions ();
    // Refresh truss geometry and property windows
    void refreshGeometryAndPropertyWindows ();

    // Methods for status bar update
    void updateStatusBar ();
    void setStatusBarHint ( const QString& );
    void projectModified ();

    // Catch designer widget focus change
    void trussWindowLostFocus ( TrussUnitWindow& );
    void trussWindowReceivedFocus ( TrussUnitWindow& );

private:
    // Current system workspace
    FermaNextWorkspace& workspace;
    // Dock widget on which all projects are located
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

    QDialog* resultsWindow;
    ResultsTabWidget* resultsTabWidget;

    // Some actions
    QAction* undoAction;
    QAction* redoAction;
    QAction* saveProjectAction;
    QAction* saveAsProjectAction;
    QAction* saveAllAction;
    QAction* closeProjectAction;
    QAction* printAction;
    QAction* showUndoRedoAction;
    QAction* showGeometryWindowAction;
    QAction* showTrussPropWindowAction;
    QAction* showResultsWindowAction;
    QAction* materialEditorAction;

    // Plugins menu
    QMenu* pluginsMenu;
    // Status bar labels
    
    QLabel *coordLabel, *elementLabel, *hintLabel, *modLabel;

    // Plugins signal mapper for plugin info showing
    QSignalMapper* pluginsSigMapper;

    // UUID of the material, which properties should be shown 
    // after dialog appearance
    QString materialEditorStartPage;

};

#endif //FERMANEXTMAINWINDOW_H
