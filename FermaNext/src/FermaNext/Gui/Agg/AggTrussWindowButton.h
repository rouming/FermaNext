
#ifndef AGGTRUSSWINDOWBUTTON_H
#define AGGTRUSSWINDOWBUTTON_H

#include "AggButton.h"
#include "AggSubsidiary.h"

class WindowButtonType
{
public:
    enum Type 
    {
        HideButton = 0,
        RollUpButton,
        PropSizeButton
    };
};

class AggTrussWindowButton : public AggButton
{
    Q_OBJECT
public:
    AggTrussWindowButton ( const QPoint& pos, const QString& fname,
                           WindowButtonType::Type type );
    virtual ~AggTrussWindowButton ();
    virtual WindowButtonType::Type getButtonType () const;
    
    virtual void paint ( ren_dynarow& baseRend ) const;

protected slots:
    void setButtonHighlightType ( bool );

private:
    WindowButtonType::Type buttonType;
    mutable pathRenderer pathRend;
    bool windowHighlighted;
    color_type edgingCol, normalCol, highlightCol, pressedCol, lineCol;
};

#endif //AGGTRUSSWINDOWBUTTON_H
