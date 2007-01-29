#include "gridNode.h"

GridNode::GridNode(qreal x, qreal y)
:QPointF(x ,y)
{
	borderNode=false;
	borderF=0;
}
