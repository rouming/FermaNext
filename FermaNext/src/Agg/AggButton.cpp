
#include "AggButton.h"
#include "AggSubsidiary.h"
#include "SubsidiaryConstants.h"

/*****************************************************************************
 * Agg Button
 *****************************************************************************/

AggButton::AggButton ( const QString& l, const QPoint& p, int w, int h ) :
    width(w), height(h),
    pressed(false),
    highlighted(false),
    leftTopPos(p),
    label(l)
{}   

AggButton::AggButton ( QPoint p, int w, int h ) :
    width(w), height(h),
    pressed(false),
    highlighted(false),
    leftTopPos(p)
{}

AggButton::~AggButton ()
{}

const QPoint& AggButton::getPosition () const
{
    return leftTopPos;
}

void AggButton::setPosition ( const QPoint& newPos )
{
    leftTopPos = newPos;
}

int AggButton::getWidth () const
{
    return width;
}

void AggButton::setWidth ( int w )
{
    width = w;
}

int AggButton::getHeight () const
{
    return height;
}

void AggButton::setHeight ( int h )
{
    height = h;
}

const QString& AggButton::getLabel () const
{
    return label;
}

void AggButton::setLabel ( const QString& l )
{
    label = l;
}

const QString& AggButton::getHint () const
{
    return hint;
}

void AggButton::setHint ( const QString& hint_ )
{
    hint = hint_;
}

bool AggButton::isPressed () const
{
    return pressed;
}

void AggButton::setPressed ( bool status )
{
    if ( pressed == status )
        return;

    if ( status )
        emit onButtonPress();
    else
        emit onButtonRelease();

    pressed = status;
    emit onChangeButtonState();
}

bool AggButton::isHighlighted () const
{
    return highlighted;
}

void AggButton::setHighlighted ( bool h_ )
{
    if ( highlighted == h_ )
        return;
    highlighted = h_;
    emit onChangeButtonState ();
}

bool AggButton::inButtonRect ( int x, int y ) const
{
    if ( x > leftTopPos.x() && x < leftTopPos.x() + width &&
         y > leftTopPos.y() && y < leftTopPos.y() + height )
        return true;
    return false;
}

void AggButton::pressButton ()
{
    setPressed ( true );
}

/****************************************************************************/
