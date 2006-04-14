
#include <QString>

#include "GuiSubsidiary.h"

/*****************************************************************************
 * Range Validator
 *****************************************************************************/

RangeValidator::RangeValidator ( QObject* parent ) :
    QDoubleValidator( parent )
{}

RangeValidator::RangeValidator ( double bottom, double top, int decimals, 
                                  QObject* parent ) :
    QDoubleValidator( bottom, top, decimals, parent )
{}   
    
QValidator::State RangeValidator::validate ( QString& input, int& pos ) const
{
    State s = QDoubleValidator::validate( input, pos );
    if ( s != Acceptable )
       return Invalid;
    return s;
}

/***************************************************************************/
