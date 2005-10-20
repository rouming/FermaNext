
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
    void reloadPlugins ();
    
protected:
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

    // Catch designer widget focus change
    void trussWindowLostFocus ( TrussUnitWindow& );
    void trussWindowReceivedFocus ( TrussUnitWindow& );

private:
    // Dock window on wich all projects are located
    QDockWindow* projectsDockWindow;
    QWidget* undoRedoHistoryWidget;
    ProjectToolBox* projectToolBox;
    FermaNextWorkspace* wsp;
    // Some actions
    QAction* undoAction;
    QAction* redoAction;
    QPopupMenu* pluginsMenu;
};

#endif //FERMANEXTFRAME_H
