#ifndef SPOINT_H
#define SPOINT_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QObject>

class SPoint : public QObject, public QGraphicsItem
{
Q_OBJECT 

public:
    SPoint(QGraphicsView *view, QGraphicsItem *parent=0, QGraphicsScene *scene=0);
    ~SPoint();
    bool active;
    void setActive(bool activation);
    void setPosition(QPointF point);
//при нажатии левой кнопки мыши  на экз Line, являющейся родителем экземпляра Node, значение active устанавливает в Т
	void positChange();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
    enum { Type = UserType + 2 };
    int type() const { return Type; }
	QPointF posit() {return mapToScene(position);}
protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QPointF position;
	QGraphicsView* viewPort;
	QGraphicsView* view() const {return viewPort;}
	QRectF brect() const;
	QGraphicsItem *pItem;
signals:
	void positionChanged(const QPointF);
};
#endif
