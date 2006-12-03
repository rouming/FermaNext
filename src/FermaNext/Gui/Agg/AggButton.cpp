
#include "AggButton.h"

/*****************************************************************************
 * Agg Button
 *****************************************************************************/

AggButton::AggButton ( const QString& text ) :
    width(0), height(0),
    buttonText( text ),
    pressed(false),
    enabled(true),
    highlighted(false),
    leftTopPos(QPoint(0,0))
{}

AggButton::AggButton () :
    width(0), height(0),
    pressed(false),
    enabled(true),
    highlighted(false),
    leftTopPos(QPoint(0,0))
{}

AggButton::~AggButton ()
{}

QPoint AggButton::getPosition () const
{
    return leftTopPos;
}

void AggButton::setPosition ( QPoint newPos )
{
    leftTopPos = newPos;
    emit onChangeButtonState();
}

int AggButton::getWidth () const
{
    return width;
}

void AggButton::setWidth ( int w )
{
    width = w;
    emit onChangeButtonState();
}

int AggButton::getHeight () const
{
    return height;
}

void AggButton::setHeight ( int h )
{
    height = h;
    emit onChangeButtonState();
}

QString AggButton::text () const
{
    return buttonText;
}

void AggButton::setText ( const QString& text )
{
    buttonText = text;
    emit onChangeButtonState();
}

const QString& AggButton::getHint () const
{
    return hint;
}

void AggButton::setHint ( const QString& hint_ )
{
    hint = hint_;
    emit onChangeButtonState();
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

bool AggButton::isEnabled () const
{
    return enabled;
}

void AggButton::setEnabled ( bool e_ )
{
    if ( enabled == e_ )
        return;
    enabled = e_;
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
    emit onChangeButtonState();
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
    setPressed( true );
}

void AggButton::paint ( ren_dynarow& ) const
{
    // Reimplement this function in child classes
}

/****************************************************************************/
