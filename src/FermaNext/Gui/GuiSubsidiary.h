
#ifndef GUISUBSIDIARY_H
#define GUISUBSIDIARY_H

#include <QValidator>

class QString;

/*****************************************************************************/

class RangeValidator : public QDoubleValidator
{
public:
    RangeValidator ( QObject* parent );
    RangeValidator ( double bottom, double top, int decimals, 
                     QObject* parent );
    virtual QValidator::State validate ( QString& input, int& ) const;
};

#endif //GUISUBSIDIARY_H
