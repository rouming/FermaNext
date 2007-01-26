#include "gridnode.h"

/*GridNode::GridNode()
:QPointF()
{
	borderNode=false;
}*/

GridNode::GridNode(qreal x, qreal y)
:QPointF(x ,y)
{
	borderNode=false;
	borderF=0;
}
