
#ifndef COOLORRANGECONTROL_H_
#define COOLORRANGECONTROL_H_

#include <QRectF>

#include "AggSubsidiary.h"
#include "AggWidget.h"
#include "AggButton.h"

class LoadCaseResults;

typedef agg::renderer_primitives<base_renderer> PrimRenderer;

/*****************************************************************************/

class ColorControlButton : public AggButton
{
public:
    ColorControlButton ();
    void paint ( base_renderer&, bool ) const;
};

/*****************************************************************************/

class ColorRangeControl : public AggWidget
{
    Q_OBJECT
public:
    ColorRangeControl ( QWidget* parent = 0 );
    ~ColorRangeControl ();

    agg::rgba8 getColorForStress ( double ) const;

    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

    void paint ( base_renderer& baseRend ) const;

protected:
    void changeLevelsNumber ( bool );
    double drawAHalfLine ( base_renderer&, const agg::rgba8&, 
                           const agg::rgba8&, double ) const;
    void drawStrokeNumber ( text_renderer&, glyph_gen&, double, double ) const;
    void signColorLine ( base_renderer& ) const;
    void drawColorLine ( base_renderer& ) const;

protected slots:
    void updateStressRange ( const LoadCaseResults& );

signals:
    void onLevelsNumberChange ();

private:
    QRectF colorLineGeometry;
    double maxStress, minStress;
    int levelsNumb;
    ColorControlButton *plusButton, *minusButton, *pressedButton;
    bool mousePressed;
};

#endif // COOLORRANGECONTROL_H_
