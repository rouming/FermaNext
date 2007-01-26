#include "line.h"
#include "spoint.h"
#include <QLineF>
#include <QPointF>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

Line::Line(QGraphicsItem * parent, QGraphicsScene * scene) 
: QObject(0), QGraphicsLineItem (parent,scene)
{
//    setCursor(QCursor(Qt::OpenHandCursor));
    //setFlag(ItemIsMovable);
    //setFlag(ItemIsSelectable);
    file=new QFile((const QString &)"line_out.txt");
    file->open(QIODevice::WriteOnly);
    out.setDevice(file);
    setSelect(false);
	boundaryCondition_="0";
}

Line::~Line()
{
    hide();
}

void Line::setSource(QPointF point)
{
    setLine(QLineF(point,line().p2()));
    sourceNode()->setPosition(point);
    scene()->update();
}

void Line::setDest(QPointF point)
{
    setLine(QLineF(line().p1(),point));
    destNode()->setPosition(point);
    scene()->update();
}

QPointF Line::source()
{   
    return this->line().p1();
}

QPointF Line::dest()
{
    return this->line().p2();
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Line::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

QPainterPath Line::shape() const
{
QPointF point[6];
QPointF pointA;
QPointF pointB;
QPainterPath path;
    if (line().p1().x()<line().p2().x())
    {
        pointA=line().p1();
        pointB=line().p2();
    }
    else
    {
        pointA=line().p2();
        pointB=line().p1();
    }
    point[0]=pointA+QPointF(-1,-1);
    point[1]=pointA+QPointF(-1,1);
    point[3]=pointB+QPointF(1,1);
    point[4]=pointB+QPointF(1,-1);
    if (pointA.y()>pointB.y())
    {
        point[2]=pointA+QPointF(1,1);
        point[5]=pointB+QPointF(-1,-1);
    }
    else
    {
        point[2]=pointA+QPointF(-1,1);
        point[5]=pointB+QPointF(1,-1);
    }
    path.moveTo(point[0]);
    path.lineTo(point[1]);
    path.lineTo(point[2]);
    path.lineTo(point[3]);
    path.lineTo(point[4]);
    path.lineTo(point[5]);
    path.lineTo(point[0]);
    return path;
return QGraphicsLineItem::shape();
} 

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    if (select())
    {
        painter->setPen(Qt::green);
    }
//    QGraphicsLineItem::paint(painter,option,widget);
	painter->drawLine(line());
}

void Line::setSourceNode(SPoint *node)
{
    sourceNode_=node;
    QObject::connect(sourceNode(),SIGNAL(positionChanged(QPointF)),this,SLOT(setP1(QPointF)));
}

void Line::setDestNode(SPoint *node)
{
    destNode_=node;
    QObject::connect(destNode(),SIGNAL(positionChanged(QPointF)),this,SLOT(setP2(QPointF)));
}

SPoint* Line::sourceNode()
{return sourceNode_;}

SPoint* Line::destNode()
{return destNode_;}

QVariant Line::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionChange:
        sourceNode()->setPosition(mapToScene(source()));
        destNode()->setPosition(mapToScene(dest()));
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void Line::setP1(QPointF point)
{
    setLine(QLineF(mapFromScene(point),line().p2()));
    scene()->update();
}

void Line::setP2(QPointF point)
{
    setLine(QLineF(line().p1(),mapFromScene(point)));
    scene()->update();
}

void Line::setActive(bool b)
{
    sourceNode()->setActive(b);
    destNode()->setActive(b);
}

void Line::execProp()
{
    LineProp dialog(this);
    dialog.exec();
}

bool Line::setBoundaryCondition(const QString &str)
{
	boundaryCondition_=str;
return true;
}

QString Line::boundaryCondition()
{
return boundaryCondition_;
}
