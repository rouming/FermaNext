
#ifndef TRUSSWINDOWHIDEBUTTON_H
#define TRUSSWINDOWHIDEBUTTON_H

#include "AggButton.h"
#include "AggSubsidiary.h"

class TrussUnitWindowButton : public AggButton
{
    Q_OBJECT
public:
    TrussUnitWindowButton ( const QPoint& pos, const QString& fname );
    virtual ~TrussUnitWindowButton ();
    virtual void paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                         agg::scanline_p8& sl, solidRenderer& solidRend  ) const;

protected slots:
    void setButtonHighlightType ( bool );

private:
    mutable pathRenderer pathRend;
    bool windowHighlighted;
    color_type edgingCol, normalCol, highlightCol, pressedCol, lineCol;
};

#endif //TRUSSWINDOWHIDEBUTTON_H
