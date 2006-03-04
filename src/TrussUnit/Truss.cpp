
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

TrussDimension::TrussDimension ( const QString& lengthMeasure_,
                                 const QString& forceMeasure_ ) 
    throw (WrongArgsException)
{
    LengthMeasure lengthMeasure = mm;
    if ( lengthMeasure_ == "mm" )
        lengthMeasure = mm;
    else if ( lengthMeasure_ == "sm" )
        lengthMeasure = sm;
    else if ( lengthMeasure_ == "m" )
        lengthMeasure = m;
    else
        throw WrongArgsException();

    ForceMeasure forceMeasure = newton;
    if ( forceMeasure_ == "newton" )
        forceMeasure = newton;
    else if ( forceMeasure_ == "kg" )
        forceMeasure = kg;
    else
        throw WrongArgsException();

    length = lengthMeasure;
    force = forceMeasure;
}

TrussDimension& TrussDimension::operator= ( const TrussDimension&  dim )
{ length = dim.length; force = dim.force; return *this; }

TrussDimension::LengthMeasure TrussDimension::getLengthMeasure () const
{ return length; }

TrussDimension::ForceMeasure TrussDimension::getForceMeasure () const
{ return force; }

QString TrussDimension::getLengthMeasureStr () const
{
    if ( length == mm )
        return "mm";
    else if ( length == sm )
        return "sm"; 
    else
        return "m";
}

QString TrussDimension::getForceMeasureStr () const
{
    if ( force == newton )
        return "newton";
    else
        return "kg";
}

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
    if ( fix == fix_ )
        return;
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
    emit onPositionChange( pos.x(), pos.y() );
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
