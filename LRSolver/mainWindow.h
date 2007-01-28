#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QTextEdit;
class QGraphicsScene;
class Editor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
protected:
    

private slots:
    void changeStatus(const QString &str);    
	void lockConstructionActions();
	void unlockConstructionActions();
	void lockCalculationAction();
	void unlockCalculationAction();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    
    QTextEdit *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;

    Editor *editor;
    QGraphicsScene *canvas;
    QAction *zoomInAct;
    QAction *zoomOutAct;
	QAction *delAct;
	QAction *addLineAct;
	QAction *addArcAct;
	QAction *breakAreaAct;
	QAction *propertiesAct;
	QAction *calculateAct;
	QAction *exitAct;
};

#endif
