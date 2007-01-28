#ifndef EDITOR_H
#define EDITOR_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAction>
#include <QTextStream>
#include <QFile>
#include <QMainWindow>
#include <QString>
#include <QGraphicsPathItem>

class Line;
class Arch;
class GridNode;

class Editor : public QGraphicsView {
    Q_OBJECT

public:
    Editor(QGraphicsScene *canvas, QWidget* parent=0);
    ~Editor();
    QGraphicsItem *pendingItem;
    QGraphicsItem *activeItem;
    Line *pendingLine;
    Arch *pendingArc;
	int walkNumber() {return _walkNumber;}
	double step() {return _step;}
	void setWalkNumber(int i) {_walkNumber=i;}
	void setStep(double d) {_step=d;}
	bool demoMode() {return demo;}
	void setDemoMode(bool b) {demo=b;}
public slots:
    void addLine();
    void addArc();
    void zoomIn();
    void zoomOut();
    void del();
    void properties();
    void select();
	void breakArea();
	void walkProperties();
	void calculate();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
private:
    void addItem(QGraphicsItem *item);
    void showNewItem(QGraphicsItem *item);
    void createActions();
    void setActiveItem(QGraphicsItem *item);
    void compliteSelection();
	void clearSelectionList();
	void lockConstruction();
	void createPath();
	void setRootPoint();
	void meshArea();
	void drawGrid();
    QAction *addLineAct;
    QAction *addArcAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *delAct;
    QAction *propAct;
    QAction *selectAct;
	QAction *breakAreaAct;
    QTextStream out;
    QFile *file;
	QPainterPath *areaPath;
	QPainterPath *grid;
    
    QString statStr;
    void setStatus(QString str) {statStr=str;}
    void unSetStatus() {statStr.clear();}   
    QString status() {return statStr;}
    
    QPoint lastPos;
    QPointF pointF; //floating point var containing currend cursor position    
    bool selection;
	bool construction;
	QList<QGraphicsItem*> list;

	QGraphicsPathItem pathTest;
	int _walkNumber;
	double _step;
	QPointF rootPoint;
	bool settingRootPoint;
	QList<GridNode> gridList;
	void setBorderValue(GridNode *node);
	double calculateBorderValue(QString s, QString x, QString y);
	bool demo; 
	double result;
//switches demonstration mode; demo==true means that user will see slow random walking
signals:
    void statusChanged(const QString &str);
	void constructionLocked();
	void constructionUnlocked();
	void lockCalculation();
	void unlockCalculation();
};

#endif