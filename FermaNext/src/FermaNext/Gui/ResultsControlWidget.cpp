
#include <QComboBox>

#include "ResultsControlWidget.h"

/*****************************************************************************
 * Results Control Widget
 *****************************************************************************/

ResultsControlWidget::ResultsControlWidget ( QWidget* parent /* = 0 */ ) :
    AggWidget( parent, Global::flipY ),
    ctrlLeft( 40.0 ), 
    ctrlTop( 10.0 ), 
    ctrlVertSpacing( 25.0 ),
    deformSlider( ctrlLeft, ctrlTop + ctrlVertSpacing, 
                  ctrlLeft + 200.0, ctrlTop + ctrlVertSpacing + 7.0, 
                  Global::flipY ), // width = 200.0; height = 7.0
    scaleSlider( ctrlLeft, ctrlTop + 2 * ctrlVertSpacing, 
                 ctrlLeft + 200.0, ctrlTop + 2 * ctrlVertSpacing + 7.0, 
                 Global::flipY ), // width = 200.0; height = 7.0
    numberCheckBox( ctrlLeft, ctrlTop + 3 * ctrlVertSpacing, 
                    "Show numbers", Global::flipY ),
    deformCheckBox( ctrlLeft, ctrlTop + 4 * ctrlVertSpacing, 
                    "Show deformation", Global::flipY ),
/*
    animCheckBox( ctrlLeft, ctrlTop + 5 * ctrlVertSpacing, 
                  "Animate", Global::flipY ),*/
    loadCaseComboBox( 0 ),
    textPoly( loadCaseText ),
    deformAllowed( false )
{
    init();
}

void ResultsControlWidget::init ()
{
    AggCtrlContainer& ctrlContainer = getAggCntrlContainer();
    ctrlContainer.addCtrl( deformSlider );
    ctrlContainer.addCtrl( scaleSlider );
    ctrlContainer.addCtrl( numberCheckBox );
    ctrlContainer.addCtrl( deformCheckBox );
//    ctrlContainer.addCtrl( animCheckBox );
    
    deformSlider.num_steps( 19 );
    deformSlider.range( 1, 20 );
    deformSlider.value( 1 );
    deformSlider.label( "Deform scale %2.0f:1" ); 
    deformSlider.pointer_color( agg::rgba( 45, 165, 195, 0.5 ) );

    scaleSlider.num_steps( 19 );
    scaleSlider.range( 0.05, 1.0 );
    scaleSlider.value( 1.0 );
    scaleSlider.label( "Picture size %1.2f" ); 
    scaleSlider.pointer_color( agg::rgba( 45, 165, 195, 0.5 ) );

    double textThickness = 1.0,
           textSize = 8.0;
    
    numberCheckBox.text_size( textSize );
    numberCheckBox.status( true );
    numberCheckBox.text_thickness( textThickness );

    deformCheckBox.text_size( textSize );
    deformCheckBox.text_thickness( textThickness );
    deformCheckBox.status( true );

    loadCaseText.text( "Current load case: " );
    loadCaseText.flip( Global::flipY );
    loadCaseText.size( textSize + 0.5 );
    textPoly.width( textThickness );

    loadCaseComboBox = new QComboBox( this );
    loadCaseComboBox->setGeometry( ctrlLeft + 125, 0, 65, 20 );

    connect( loadCaseComboBox, SIGNAL(currentIndexChanged(int)),
                               SIGNAL(onSwitchLoadCase(int)) );
}

void ResultsControlWidget::fillLoadCaseComboBox ( int loadCaseNumb )
{
    loadCaseComboBox->clear();
    for ( uint i = 1; i <= loadCaseNumb; ++i )
        loadCaseComboBox->addItem( "Case " + QString::number( i ) );
}

void ResultsControlWidget::initControlsState ()
{
    // clear state of the sliders
    deformSlider.value( 1 );
    emit onDeformSliderValueChanged( deformSlider.value() );
    scaleSlider.value( 1.0 );
    emit onScaleSliderValueChanged( scaleSlider.value() );

    // emit signals about previous state of the check boxes
    emit onNumberCheckBoxValueChanged( numberCheckBox.status() );
    emit onDeformCheckBoxValueChanged( deformCheckBox.status(), 
                                           deformSlider.value() );
}

void ResultsControlWidget::aggPaintEvent ( QPaintEvent* event )
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    baseRend.clear ( agg::rgba( 1, 1, 1, 0 ) );
    paint( baseRend );
}

void ResultsControlWidget::aggResizeEvent ( QResizeEvent* )
{}

void ResultsControlWidget::aggKeyPressEvent ( QKeyEvent* )
{}

void ResultsControlWidget::aggMouseMoveEvent ( QMouseEvent* )
{}

void ResultsControlWidget::aggMouseReleaseEvent ( QMouseEvent* )
{}

void ResultsControlWidget::aggMousePressEvent ( QMouseEvent* )
{}

void ResultsControlWidget::aggCtrlChangedEvent ( const agg::ctrl* aggCtrl )
{
    if ( aggCtrl == &deformSlider ) {
        deformCheckBox.status( true );
        emit onDeformSliderValueChanged( deformSlider.value() );
    }
    if ( aggCtrl == &scaleSlider )
        emit onScaleSliderValueChanged( scaleSlider.value() );
    if ( aggCtrl == &numberCheckBox )
        emit onNumberCheckBoxValueChanged( numberCheckBox.status() );
    if ( aggCtrl == &deformCheckBox )
        emit onDeformCheckBoxValueChanged( deformCheckBox.status(), 
                                           deformSlider.value() );
}

void ResultsControlWidget::setDeformSliderEnabled ( bool status )
{
    if ( ! deformAllowed && status )
        deformSlider.value( 1 );
    deformAllowed = status;
    deformCbState = deformCheckBox.status();
    update();
}

void ResultsControlWidget::paint ( base_renderer& base )
{
    scanline_rasterizer ras;
    solid_renderer solid( base );   

    loadCaseText.start_point( ctrlLeft, 13 );
    ras.add_path( textPoly );
    solid.color( agg::rgba( 0, 0, 0 ) );
    agg::render_scanlines( ras, scanline, solid );
 
    ras.filling_rule( agg::fill_non_zero );
    if ( ! deformAllowed ) {
        deformSlider.pointer_color( agg::rgba( 1, 1, 1, 0 ) );
        deformSlider.num_steps( 0 );
        deformSlider.range( 1, 1 );
        deformSlider.value( 1 );
        deformSlider.label("");

        deformCheckBox.active_color( agg::rgba( 0, 0, 0, 0.4 ) );
        deformCheckBox.inactive_color( agg::rgba( 0, 0, 0, 0.4 ) );
        deformCheckBox.text_color( agg::rgba( 0, 0, 0, 0.4 ) );
        deformCheckBox.status( deformCbState );
    }
    else {
        deformSlider.num_steps( 19 );
        deformSlider.range( 1, 20 );
        deformSlider.label( "Deform scale %2.0f:1" );         
        deformSlider.pointer_color( agg::rgba( 45, 165, 195, 0.5 ) );

        deformCheckBox.active_color( agg::rgba( 0, 0, 0 ) );
        deformCheckBox.inactive_color( agg::rgba( 0, 0, 0 ) );
        deformCheckBox.text_color( agg::rgba( 0, 0, 0 ) );
    }
    agg::render_ctrl( ras, scanline, solid, deformSlider );
    agg::render_ctrl( ras, scanline, solid, scaleSlider );
    agg::render_ctrl( ras, scanline, solid, numberCheckBox );
    agg::render_ctrl( ras, scanline, solid, deformCheckBox );
}

/*****************************************************************************/
