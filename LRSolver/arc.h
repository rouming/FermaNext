#ifndef ARC_H
#define ARC_H

#include <QGraphicsItem>
#include <QFile>
#include <QTextStream>
#include <QPointF>

class SPoint;
class QPointF;
class QGraphicsSceneMouseEvent;

//namespace Vasya {

class Arc : public QObject, public QGraphicsEllipseItem
{
Q_OBJECT

public:
    Arc(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
    ~Arc();
    void setCenterNode(SPoint *p);
    void setStartNode(SPoint *p);
    void setSpanNode(SPoint *p);
    SPoint* centerNode(){return centerNode_;}
    SPoint* startNode(){return startNode_;}
    SPoint* spanNode(){return spanNode_;}
    void setActive(bool b);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
    enum {Type=UserType+3};
    int type() const { return Type; }

    void setReady() {spec=TRUE;}
    bool ready() {return spec;}

    QTextStream out;
    QFile *file;

    void setSelect(bool b) {selection=b;}
    bool select() {return selection;}
	bool setBoundaryCondition(const QString &str);
	QString boundaryCondition();
public slots:
    void setCenter(QPointF p);
    void setStart(QPointF p);
    void setSpan(QPointF p); 
    void setArc(QPointF cent,int startAn,int SpanAn, qreal rad);
    void execProp();
protected:
/*    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);*/
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    SPoint *centerNode_;
    SPoint *startNode_;
    SPoint *spanNode_;
    QPointF center;
    QPointF start;
    QPointF span;
    bool spec;
    bool selection;
	QString boundaryCondition_;
};

//} //namespace Vasya

#endif
