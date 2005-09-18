
#include "AggButton.h"

/*****************************************************************************
 * Agg Button
 *****************************************************************************/

AggButton::AggButton ( QString l, QPoint pos, int w, int h ) :
    leftTopPos ( pos ),
    width ( w ),
    height ( h ),
    label ( l ),
    pressed ( false ),
    highlighted ( false )
{}

AggButton::AggButton ( QPoint pos, int w, int h ) :
    leftTopPos ( pos ),
    width ( w ),
    height ( h ),
    pressed ( false ),
    highlighted ( false )
{}

AggButton::~AggButton ()
{}

void AggButton::setLabel ( QString l )
{
    label = l;
}

QString AggButton::getLabel () const
{
    return label;
}

void AggButton::setHint ( QString hint_ )
{
    label = hint_;
}

QString AggButton::getHint () const
{
    return hint;
}

void AggButton::setPosition ( QPoint newPos )
{
    leftTopPos = newPos;
}

QPoint AggButton::getPosition () const
{
    return leftTopPos;
}

void AggButton::setWidth ( int w )
{
    width = w;
}

int AggButton::getWidth () const
{
    return width;
}

void AggButton::setHeight ( int h )
{
    height = h;
}

int AggButton::getHeight () const
{
    return height;
}

void AggButton::setHighlighted ( bool h_ )
{
    if ( highlighted == h_ )
        return;
    highlighted = h_;
    emit onButtonHighlightChange ();
}

bool AggButton::isHighlighted () const
{
    return highlighted;
}

bool AggButton::inButtonRect ( int x, int y ) const
{
    if ( x > leftTopPos.x() && x < leftTopPos.x() + width &&
         y > leftTopPos.y() && y < leftTopPos.y() + height )
        return true;
    return false;
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

void AggButton::pressButton ()
{
    setPressed ( true );
}

/****************************************************************************/
