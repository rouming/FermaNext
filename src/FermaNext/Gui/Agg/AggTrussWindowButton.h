
#ifndef AGGTRUSSWINDOWBUTTON_H
#define AGGTRUSSWINDOWBUTTON_H

#include "AggButton.h"
#include "AggSubsidiary.h"

class AggTrussWindowButton : public AggButton
{
    Q_OBJECT
public:
    AggTrussWindowButton ( const QPoint& pos, const QString& fname );
    virtual ~AggTrussWindowButton ();
    virtual void paint ( ren_dynarow& baseRend, 
                         scanline_rasterizer& ras,
                         agg::scanline_p8& sl, 
                         solidRenderer& solidRend  ) const;

protected slots:
    void setButtonHighlightType ( bool );

private:
    mutable pathRenderer pathRend;
    bool windowHighlighted;
    color_type edgingCol, normalCol, highlightCol, pressedCol, lineCol;
};

#endif //AGGTRUSSWINDOWBUTTON_H
