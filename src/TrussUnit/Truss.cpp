
#include "Truss.h"
#include <algorithm>

/*****************************************************************************
 * Truss Emitter
 *****************************************************************************/

TrussEmitter::TrussEmitter ( ObjectStateManager* mng ) :
    StatefulObject(mng)
{}

void TrussEmitter::stateIsChanged ()
{
    emit onStateChange();
}

/*****************************************************************************
 * Node
 *****************************************************************************/

Node::Node ( ObjectStateManager* mng ) : 
    StatefulObject(mng),
    x(0), y(0), 
    fix(Unfixed),
    number(0)
{}

Node::Node ( int x_, int y_, ObjectStateManager* mng ) :
    StatefulObject(mng),
    x(x_), y(y_), 
    fix(Unfixed),
    number(0)
{}

Node::Node ( int x_, int y_, Fixation fix_, ObjectStateManager* mng ) :
    StatefulObject(mng),
    x(x_), y(y_), 
    fix(fix_),
    number(0)
{}

Node::Node ( int x_, int y_, Fixation fix_, int num_, ObjectStateManager* mng ) :
    StatefulObject(mng),
    x(x_), y(y_), 
    fix(fix_),
    number(num_)
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

void Node::setPoint ( QPoint p )
{
    setPoint(p.x(), p.y());
}

void Node::setPoint ( int x_, int y_ )
{
    x = x_;
    y = y_;
    emit onPositionChange(x, y);    
}

QPoint Node::getPoint () const
{
    return QPoint(x, y);
}

int Node::getX () const
{
    return x;
}

int Node::getY () const
{
    return y;
}

void Node::setNumber ( int num_ )
{
    number = num_;
    emit onNumberChange(number);
}

int Node::getNumber () const
{
    return number;
}

/****************************************************************************/
