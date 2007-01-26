#ifndef GRIDNODE_H
#define GRIDNODE_H

#include <QPointF>

class GridNode : public QPointF
{
public:
//	GridNode();
    GridNode(qreal x, qreal y);
	void setBorder(bool b) {borderNode=b;}
	bool border() {return borderNode;}
	void setValue(double d) {borderF=d;}
	double value() {return borderF;}
private:
	bool borderNode; 
	double borderF;
};

#endif
