
#include "TrussLoad.h"
#include <algorithm>

/*****************************************************************************
 * Truss Load
 *****************************************************************************/

TrussLoad::TrussLoad () :
    enabled(true),
    removed(false),
    x(0), y(0)
{}

TrussLoad::TrussLoad ( double x_, double y_ ) :
    enabled(true),
    removed(false),
    x(x_), y(y_)
{}

TrussLoad::TrussLoad ( const TrussLoad& load ) :
    QObject(),
    enabled(load.enabled),
    removed(load.removed),
    x(load.x), y(load.y)
{}
 
TrussLoad& TrussLoad::operator= ( const TrussLoad& load )
{
    enabled = load.enabled;
    removed = load.removed;
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
    if ( x == x_ )
        return;
    x = x_;
    emit forceWasChanged();
}

void TrussLoad::setYForce ( double y_ )
{
    if ( y == y_ )
        return;
    y = y_;
    emit forceWasChanged();
}

void TrussLoad::setForces ( double x_, double y_ )
{
    if ( x == x_ && y == y_ )
        return;
    x = x_;
    y = y_;
    emit forceWasChanged();
}

bool TrussLoad::isEnabled () const
{
    return enabled;
}

bool TrussLoad::isRemoved () const
{
    return removed;
}

void TrussLoad::disable ()
{
    enabled = false;
}

void TrussLoad::enable ()
{
    enabled = true;
}

void TrussLoad::remove ()
{
    disable();
    removed = true;    
}

/*****************************************************************************/
