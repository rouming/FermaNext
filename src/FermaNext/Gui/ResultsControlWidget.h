
#ifndef RESULTSCONTROLWIDGET_H_
#define RESULTSCONTROLWIDGET_H_

#include "AggWidget.h"
#include "AggSubsidiary.h"

class QComboBox;

/*****************************************************************************/

class ResultsControlWidget : public AggWidget
{
    Q_OBJECT
public:
    ResultsControlWidget ( QWidget* parent = 0 );
    void fillLoadCaseComboBox ( int loadCaseNumb );
    void initControlsState ();

    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );
    
protected:
    void init ();
    void paint ( base_renderer& );

signals:
    void onScaleSliderValueChanged ( double );
    void onDeformSliderValueChanged ( double );
    void onNumberCheckBoxValueChanged ( bool );
    void onDeformCheckBoxValueChanged ( bool, double );
    void onSwitchLoadCase ( int );

protected slots:
    void setDeformSliderEnabled ( bool );

private:
    double ctrlLeft, ctrlTop, ctrlVertSpacing;
    AggSlider deformSlider, scaleSlider;
    AggCheckBox numberCheckBox, deformCheckBox;
    QComboBox* loadCaseComboBox;
    agg::scanline_p8 scanline;
    agg::gsv_text loadCaseText;
    TextPoly textPoly;
    bool deformAllowed, deformCbState;
};

#endif // RESULTSCONTROLWIDGET_H_
