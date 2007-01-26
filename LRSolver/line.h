#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>
#include <QFile>
#include <QTextStream>
#include "lineprop.h"

class SPoint;
class QPointF;
class QGraphicsSceneMouseEvent;

class Line : public QObject, public QGraphicsLineItem
{
Q_OBJECT

public:
    Line(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
    ~Line();
    void setSource(QPointF point);
    void setDest(QPointF point);    
    void setSourceNode(SPoint *node);
    void setDestNode(SPoint *node);
    SPoint* sourceNode();
    SPoint* destNode();
    QPointF source();
    QPointF dest();
    QPainterPath shape() const;
    void setActive(bool b);
    enum {Type=UserType+1};
    int type() const { return Type; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
    QTextStream out;
    QFile *file;
    void setSelect(bool b) {selection=b;}
    bool select() {return selection;}
	bool setBoundaryCondition(const QString &str);
	QString boundaryCondition();
public slots:
    void setP1(QPointF point);
    void setP2(QPointF point);
    void execProp();
protected:
//    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    SPoint *sourceNode_;
    SPoint *destNode_;
    bool selection;
	QString boundaryCondition_;
};

#endif
