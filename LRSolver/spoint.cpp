#include "spoint.h"
#include "line.h"
#include <QPainter>
#include <QGraphicsScene>

SPoint::SPoint(QGraphicsView *view, QGraphicsItem *parent, QGraphicsScene *scene)
:QObject(0), QGraphicsItem(parent, scene)
{
    file=new QFile((const QString &)"spoint_out.txt");
    file->open(QIODevice::WriteOnly);
    out.setDevice(file);
	viewPort=view;
}

SPoint::~SPoint()
{
    hide();
}

void SPoint::setActive(bool b)
{   
    active=b;
}

void SPoint::setPosition(QPointF point)
{
    position=mapFromScene(point);
}

void SPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(active)
	painter->drawEllipse(brect());
}

QRectF SPoint::boundingRect() const
{
qreal penWidth=1.0;
QPoint p=view()->mapFromScene(position);
QPointF p1=view()->mapToScene(p-QPoint(5,5));
QPointF p2=view()->mapToScene(p+QPoint(5,5));
QRectF rect=QRectF(p1,QSizeF((p2.x()-p1.x()),(p2.y()-p1.y())));
	return rect;
}

QPainterPath SPoint::shape() const
{
QPainterPath path;
	path.addEllipse(brect());
	return path;
}

QRectF SPoint::brect() const
{
QPoint p=view()->mapFromScene(position);
QPointF p1=view()->mapToScene(p-QPoint(4,4));
QPointF p2=view()->mapToScene(p+QPoint(4,4));
QRectF rect=QRectF(p1,QSizeF((p2.x()-p1.x()),(p2.y()-p1.y())));
	return rect;
}

/*QVariant SPoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change==ItemPositionChange) 
	{
		emit positionChanged(posit());
	//if changed position and parent Item is Line, th spoint makes parent to call adjust()
    }
    return QGraphicsItem::itemChange(change, value);
}*/

void SPoint::positChange()
{
	emit positionChanged(posit());
}
