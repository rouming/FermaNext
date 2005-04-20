
#include "TrussUnit.h"
#include <algorithm>

/*****************************************************************************
 * Paintable Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(false), 
    enabled(false)
{}

PaintableTrussElement::PaintableTrussElement ( bool e_, bool v_ ) :
    visible(v_), 
    enabled(e_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    visible = v_;
}

void PaintableTrussElement::setEnabled ( bool e_ )
{
    enabled = e_;
    if ( !enabled ) visible = false;
}

bool PaintableTrussElement::isVisible () const
{
    return visible;
}

bool PaintableTrussElement::isEnabled () const
{
    return enabled;
}

/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit ()
{}

TrussUnit::~TrussUnit ()
{}

void TrussUnit::paint ( agg::rendering_buffer& buffer ) const
{
    // implementation goes here!
}

/****************************************************************************/


