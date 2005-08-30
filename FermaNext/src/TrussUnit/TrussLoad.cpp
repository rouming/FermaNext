
#include "TrussLoad.h"
#include <algorithm>

/*****************************************************************************
 * Truss Load
 *****************************************************************************/

TrussLoad::TrussLoad () :
    x(0), y(0)
{}

TrussLoad::TrussLoad ( double x_, double y_ ) :
    x(x_), y(y_)
{}

TrussLoad::TrussLoad ( const TrussLoad& load ) :
    x(load.x), y(load.y)
{}
 
TrussLoad& TrussLoad::operator= ( const TrussLoad& load )
{
    x = load.x;
    y = load.y;
    return *this;
}

double TrussLoad::getXForce () const
{
    return x;
}

double TrussLoad::getYForce () const
{
    return y;
}

void TrussLoad::setXForce ( double x_ )
{
    x = x_;
}

void TrussLoad::setYForce ( double y_ )
{
    y = y_;
}

void TrussLoad::setForces ( double x_, double y_ )
{
    x = x_;
    y = y_;
}

/*****************************************************************************/
