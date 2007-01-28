#include <QtGui>
#include <QGraphicsScene>

#include "editor.h"
#include "mainWindow.h"

MainWindow::MainWindow()
{
    canvas=new QGraphicsScene();
    editor=new Editor(canvas,this);
    setCentralWidget(editor);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();

    setWindowTitle("LRSolver");
    connect(editor,SIGNAL(statusChanged(const QString)),this,SLOT(changeStatus(const QString)));
	connect(editor,SIGNAL(constructionLocked()),this,SLOT(lockConstructionActions()));
	connect(editor,SIGNAL(constructionUnlocked()),this,SLOT(unlockConstructionActions()));
	connect(editor,SIGNAL(unlockCalculation()),this,SLOT(unlockCalculationAction()));
	connect(editor,SIGNAL(lockCalculation()),this,SLOT(lockCalculationAction()));
}

MainWindow::~MainWindow()
{
	delete zoomInAct;
	delete zoomOutAct;
	delete delAct;
	delete addLineAct;
	delete addArcAct;
	delete breakAreaAct;
}

void MainWindow::createActions()
{
    zoomInAct=new QAction(QIcon("../images/zoomin.png"), tr("Zoom In"), this);
    zoomInAct->setStatusTip(tr("Zoom In Editors Field"));
    connect(zoomInAct, SIGNAL(triggered()), editor, SLOT(zoomIn()));
    zoomOutAct=new QAction(QIcon("../images/zoomout.png"), tr("Zoom Out"), this);
    zoomOutAct->setStatusTip(tr("Zoom Out Editors Field"));
    connect(zoomOutAct, SIGNAL(triggered()), editor, SLOT(zoomOut()));
	delAct=new QAction(QIcon("../images/del.png"),tr("Delete Item"),this);
	connect(delAct,SIGNAL(triggered()),editor,SLOT(del()));
	delAct->setStatusTip(tr("Delete Item"));
	addLineAct=new QAction(QIcon("../images/add.png"),tr("Add Line"),this);
	connect(addLineAct,SIGNAL(triggered()),editor,SLOT(addLine()));
	addLineAct->setStatusTip(tr("Add Line Item"));
	addArcAct=new QAction(QIcon("../images/add.png"),tr("Add Arc"),this);
	connect(addArcAct,SIGNAL(triggered()),editor,SLOT(addArc()));
	addArcAct->setStatusTip(tr("Add Arc Item"));
	breakAreaAct=new QAction(QIcon("../images/break.png"), tr("Break area"),this);
	breakAreaAct->setStatusTip(tr("Destroy Area"));
	connect(breakAreaAct,SIGNAL(triggered()),editor,SLOT(breakArea()));
	breakAreaAct->setEnabled(false);
	propertiesAct=new QAction(QIcon("../images/prop.png"), tr("Random Walk Properties"),this);
	propertiesAct->setStatusTip(tr("Show walk properties"));
	connect(propertiesAct,SIGNAL(triggered()),editor,SLOT(walkProperties()));
	calculateAct=new QAction(QIcon("../images/calc.png"), tr("Calculate"),this);
	calculateAct->setStatusTip(tr("Calculate"));
	calculateAct->setEnabled(false);	
	connect(calculateAct,SIGNAL(triggered()),editor,SLOT(calculate()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(delAct);
	editMenu->addAction(addLineAct);
	editMenu->addAction(addArcAct);
	editMenu->addAction(breakAreaAct);
	editMenu->addAction(propertiesAct);
	editMenu->addAction(calculateAct);
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(delAct);
	editToolBar->addAction(addLineAct);
	editToolBar->addAction(addArcAct);
	editToolBar->addAction(breakAreaAct);
	editToolBar->addAction(propertiesAct);
	editToolBar->addAction(calculateAct);
    viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("none", "sul");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("none", "sul");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::changeStatus(const QString &str)
{
    statusBar()->showMessage(str);
}

void MainWindow::lockConstructionActions()
{
	delAct->setEnabled(false);
	addLineAct->setEnabled(false);
	addArcAct->setEnabled(false);
	breakAreaAct->setEnabled(true);	
}

void MainWindow::unlockConstructionActions()
{
	delAct->setEnabled(true);
	addLineAct->setEnabled(true);
	addArcAct->setEnabled(true);
	breakAreaAct->setEnabled(false);
}

void MainWindow::lockCalculationAction()
{
	calculateAct->setEnabled(false);	
}

void MainWindow::unlockCalculationAction()
{
	calculateAct->setEnabled(true);	
}

