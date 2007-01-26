#include "arc.h"
#include "spoint.h"
#include "arcprop.h"
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <math.h>

#define PI 3.14159265

//using namespace Vasya;

Arc::Arc(QGraphicsItem * parent, QGraphicsScene * scene) 
: QObject(0), QGraphicsEllipseItem (parent,scene)
{
    file=new QFile((const QString &)"arc_out.txt");
    file->open(QIODevice::WriteOnly);
    out.setDevice(file);
    spec=FALSE;
    setSelect(false);
	boundaryCondition_="0";
}

Arc::~Arc()
{
    hide();
}

void Arc::setCenterNode(SPoint *p)
{
    centerNode_=p;
    QObject::connect(centerNode(),SIGNAL(positionChanged(QPointF)),this,SLOT(setCenter(QPointF)));
}
void Arc::setStartNode(SPoint *p)
{
    startNode_=p;
    QObject::connect(startNode(),SIGNAL(positionChanged(QPointF)),this,SLOT(setStart(QPointF)));
}
void Arc::setSpanNode(SPoint *p)
{
    spanNode_=p;
    QObject::connect(spanNode(),SIGNAL(positionChanged(QPointF)),this,SLOT(setSpan(QPointF)));
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	painter->setPen(Qt::black);
    if (select())
    {
        painter->setPen(Qt::green);
    }
    painter->drawArc(rect(),startAngle(),spanAngle());
}

QPainterPath Arc::shape() const
{   
    return QGraphicsEllipseItem::shape();
}

QRectF Arc::boundingRect() const
{
    return QGraphicsEllipseItem::boundingRect();
}

void  Arc::setStart(QPointF p)
{
    start=mapFromScene(p);
    if (spec==TRUE)
    {
        qreal radius=sqrt((center.y()-start.y())*(center.y()-start.y())+(center.x()-start.x())*(center.x()-start.x()));
        setRect(center.x()-radius, center.y()-radius,2*radius,2*radius);
    double strang=atan((start.y()-center.y())/(start.x()-center.x()))*180/PI+45.0/2;
        if(start.x()<center.x()) strang=strang+180;
        if(strang<0) strang=strang+360;
    double spnang=atan((span.y()-center.y())/(span.x()-center.x()))*180/PI+45.0/2;
        QPointF pp=(QPointF(radius*cos((spnang-45.0/2)*PI/180),radius*sin((spnang-45.0/2)*PI/180)));
        if(span.x()<center.x()) spnang=spnang+180;
        if(spnang<0) spnang=spnang+360;

        if(spnang>strang)
            spnang=spnang-strang;
        else 
            spnang=360+spnang-strang;

        setStartAngle(360-strang*16);
        setSpanAngle(-1*spnang*16);
        scene()->update();
        startNode()->setPosition(p);
        if(span.x()<center.x())
            {spanNode()->setPosition(mapToScene(center-pp));}
        else
            {spanNode()->setPosition(mapToScene(center+pp));}
    }
    else
        startNode()->setPosition(mapToScene(p));
}

void  Arc::setSpan(QPointF p)
{
    span=mapFromScene(p);
    if (spec==TRUE)
    {
        qreal radius=sqrt((center.y()-span.y())*(center.y()-span.y())+(center.x()-span.x())*(center.x()-span.x()));
        setRect(center.x()-radius, center.y()-radius,2*radius,2*radius);
    double strang=atan((start.y()-center.y())/(start.x()-center.x()))*180/PI+45.0/2;
        QPointF pp=(QPointF(radius*cos((strang-45.0/2)*PI/180),radius*sin((strang-45.0/2)*PI/180)));
        if(start.x()<center.x()) strang=strang+180;
        if(strang<0) strang=strang+360;
    double spnang=atan((span.y()-center.y())/(span.x()-center.x()))*180/PI+45.0/2;
        if(span.x()<center.x()) spnang=spnang+180;
        if(spnang<0) spnang=spnang+360;
        if(spnang>strang)
            spnang=spnang-strang;
        else 
            spnang=360+spnang-strang;
        setStartAngle(360-strang*16);
        setSpanAngle(-1*spnang*16);
        scene()->update();
        spanNode()->setPosition(p);
        if(start.x()<center.x())
            {startNode()->setPosition(mapToScene(center-pp));}
        else
            {startNode()->setPosition(mapToScene(center+pp));}
    }
    else
        spanNode()->setPosition(mapToScene(p));
}

void  Arc::setCenter(QPointF p)
{
    center=mapFromScene(p);
    if (spec==TRUE)
    {
        qreal radius=sqrt((center.y()-span.y())*(center.y()-span.y())+(center.x()-span.x())*(center.x()-span.x()));
        setRect(center.x()-radius, center.y()-radius,2*radius,2*radius);
    double strang=atan((start.y()-center.y())/(start.x()-center.x()))*180/PI+45.0/2;
        QPointF pp2=(QPointF(radius*cos((strang-45.0/2)*PI/180),radius*sin((strang-45.0/2)*PI/180)));
        if(start.x()<center.x()) strang=strang+180;
        if(strang<0) strang=strang+360;
    double spnang=atan((span.y()-center.y())/(span.x()-center.x()))*180/PI+45/2;
        QPointF pp1=(QPointF(radius*cos((spnang-45.0/2)*PI/180),radius*sin((spnang-45.0/2)*PI/180)));
        if(span.x()<center.x()) spnang=spnang+180;
        if(spnang<0) spnang=spnang+360;
        if(spnang>strang)
            spnang=spnang-strang;
        else 
            spnang=360+spnang-strang;

        setStartAngle(360-strang*16);
        setSpanAngle(-1*spnang*16);
        scene()->update();
        centerNode()->setPosition(p);
        if(span.x()<center.x())
            {spanNode()->setPosition(mapToScene(center-pp1));}
        else
            {spanNode()->setPosition(mapToScene(center+pp1));}
        if(start.x()<center.x())
            {startNode()->setPosition(mapToScene(center-pp2));}
        else
            {startNode()->setPosition(mapToScene(center+pp2));}
    }
    else centerNode()->setPosition(mapToScene(p));
}

void Arc::setActive(bool b)
{
    centerNode()->setActive(b);
    startNode()->setActive(b);
    spanNode()->setActive(b);
}

/*QVariant Line::itemChange(GraphicsItemChange change, const QVariant &value)
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
}*/

void Arc::setArc(QPointF cent,int startAn,int spanAn, qreal rad)
{
double strang;
double spnang;
    center=cent;  
    setRect(center.x()-rad, center.y()-rad,2*rad,2*rad);
    strang=startAn+45/2;
    spnang=spanAn;
out<<"st "<<QString::number(strang)<<"sp "<<QString::number(spnang)<<endl;
    QPointF pp2=(QPointF(rad*cos((strang-45.0/2)*PI/180),rad*sin((strang-45.0/2)*PI/180)));
//    if(start.x()<center.x()) strang=strang+180;
//    if(strang<0) strang=strang+360;
out<<"st "<<QString::number(strang)<<"sp "<<QString::number(spnang)<<endl;
    QPointF pp1=(QPointF(rad*cos((spnang+strang-45.0/2)*PI/180),rad*sin((spnang+strang-45.0/2)*PI/180)));
//    if(span.x()<center.x()) spnang=spnang+180;
//    if(spnang<0) spnang=spnang+360;
out<<"st "<<QString::number(strang)<<"sp "<<QString::number(spnang)<<endl;
/*    if(spnang>strang)
        spnang=spnang-strang;
    else 
    spnang=360+spnang-strang;*/
out<<"st "<<QString::number(strang)<<"sp "<<QString::number(spnang)<<endl;
    setStartAngle(360-strang*16);
    setSpanAngle(-1*spnang*16);
    centerNode()->setPosition(center);
 /*   if(span.x()<center.x())
        {spanNode()->setPosition(mapToScene(center-pp1));
        span=center-pp1;}
    else */
        {spanNode()->setPosition(mapToScene(center+pp1));
         span=center+pp1;}
/*    if(start.x()<center.x())
        {startNode()->setPosition(mapToScene(center-pp2));
        start=center-pp2;}
    else*/
        {startNode()->setPosition(mapToScene(center+pp2));
        start=center+pp2;}
    scene()->update();
}

void Arc::execProp()
{
    ArcProp dialog(this);
    dialog.exec();
}

bool Arc::setBoundaryCondition(const QString &str)
{
	boundaryCondition_=str;
return true;
}

QString Arc::boundaryCondition()
{
return boundaryCondition_;
}
