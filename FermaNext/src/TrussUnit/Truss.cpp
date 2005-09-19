
#include "Truss.h"
#include <algorithm>

/*****************************************************************************
 * Truss Dimenstion
 *****************************************************************************/

TrussDimension::TrussDimension () :
    length(sm), force(newton)
{}

TrussDimension::TrussDimension ( LengthMeasure lm, ForceMeasure fm ) :
    length(lm), force(fm)
{}

TrussDimension::LengthMeasure TrussDimension::getLengthMeasure () const
{ return length; }

TrussDimension::ForceMeasure TrussDimension::getForceMeasure () const
{ return force; }

void TrussDimension::setLengthMeasure ( LengthMeasure lm )
{ length = lm; }

void TrussDimension::setForceMeasure ( ForceMeasure fm )
{ force = fm; }

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
 * Pivot Emitter
 *****************************************************************************/

PivotEmitter::PivotEmitter ( ObjectStateManager* mng ) :
    StatefulObject(mng)
{}

/*****************************************************************************
 * Node
 *****************************************************************************/

Node::Node ( ObjectStateManager* mng ) : 
    StatefulObject(mng),
    fix(Unfixed),
    number(0)
{}

Node::Node ( double x_, double y_, ObjectStateManager* mng ) :
    StatefulObject(mng),
    pos(x_, y_), 
    fix(Unfixed),
    number(0)
{}

Node::Node ( double x_, double y_, Fixation fix_, ObjectStateManager* mng ) :
    StatefulObject(mng),
    pos(x_, y_), 
    fix(fix_),
    number(0)
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

void Node::setPoint ( DoublePoint p )
{
    pos = p;
    emit onPositionChange(pos.x(), pos.y());
}

void Node::setPoint ( double x_, double y_ )
{
    setPoint(DoublePoint(x_, y_));
}

const DoublePoint& Node::getPoint () const
{
    return pos;
}

double Node::getX () const
{
    return pos.x();
}

double Node::getY () const
{
    return pos.y();
}

void Node::setNumber ( int num_ )
{
    number = num_;
}

int Node::getNumber () const
{
    return number;
}

/****************************************************************************/
