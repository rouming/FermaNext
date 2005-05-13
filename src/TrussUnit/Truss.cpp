
#include "Truss.h"
#include <algorithm>

/*****************************************************************************
 * Node
 *****************************************************************************/

Node::Node () : 
    x(0), y(0), fix(Unfixed)
{}

Node::Node ( int x_, int y_ ) :
    x(x_), y(y_), fix(Unfixed)
{}

Node::Node ( int x_, int y_, Fixation fix_ ) :
    x(x_), y(y_), fix(fix_)
{}

Node::Node ( const Node& n ) :
    QObject(),
    x(n.x), y(n.y), fix(n.fix)
{}

void Node::setFixation ( Node::Fixation fix_ )
{
    fix = fix_;
    emit onFixationChange(fix);
}

Node::Fixation Node::getFixation () const
{
    return fix;
}

void Node::loadState ( const Node& n ) 
{
    emit beforeStateLoaded();    
    x = n.x;
    y = n.y;    
    emit onPositionChange(x, y);
    fix = n.fix;
    emit onFixationChange(fix);
    emit afterStateLoaded();
}

void Node::setX (int newX )
{
    x = newX;
}

void Node::setY (int newY )
{
    y = newY;
}

int Node::getX () const
{
    return x;
}

int Node::getY () const
{
    return y;
}

/****************************************************************************/


