
#include "AggPopupHint.h"
#include "AggPaintThread.h"
#include <QWidget>

/*****************************************************************************
 * Agg Popup Hint
 *****************************************************************************/

AggPopupHint::AggPopupHint ( QWidget& widget ) :
    font_( agg::verdana13 ),
    widgetSize_( widget.size() ),
    alphaCoeff( 0 ),
    rendered_( false ),
    buf_( new rbuf_dynarow( 100, 20 ) ),
    thread_( new AggPaintThread( widget, 10, 1, 1 ) )
{}

AggPopupHint::~AggPopupHint ()
{
    delete buf_;
    if ( thread_->isRunning() )
        thread_->wait();
    delete thread_;
}

void AggPopupHint::renewWidgetSize ( const QSize& wSize )
{
    widgetSize_ = wSize;
}

void AggPopupHint::show ( const QString& hintText, 
                          const QPoint cursorPos, bool smooth )
{
    // nothing to show
    if ( ! &hintText )
    {
        alphaCoeff = 0;
        return;
    }

    rendered_ = false;

    text_ = hintText;
    cursorPos_ = cursorPos;
    renewHintGeometry ();

    if ( smooth )
    {
        smooth_ = smoothShow;
        thread_->setAlphaChannelCoeff( &alphaCoeff, 0.1 );
        thread_->start( QThread::TimeCriticalPriority );
    }
    else
    {
        alphaCoeff = 1.0;
        smooth_ = noSmooth;
    }
}

void AggPopupHint::hide ( bool smooth )
{
    if ( thread_->isRunning() )
        thread_->wait();

    if ( smooth )
    {
        rendered_ = false;
        smooth_ = smoothHide;
        thread_->setAlphaChannelCoeff( &alphaCoeff, -0.1 );
        thread_->start( QThread::TimeCriticalPriority );
    }
    else
    {
        alphaCoeff = 0;
        smooth_ = noSmooth;
    }
}

void AggPopupHint::renewHintGeometry ()
{
    glyph_gen glyph( font_ );

    int width = (int)glyph.width( text_.toAscii().data() ) + 12,
        height = (int)glyph.height() + 6;

    size_ = QSize( width, height );

    if( (int)buf_->width() < size_.width() )
        buf_->init( size_.width(), size_.height() );
}

QPoint AggPopupHint::getHintPosition () const
{
    QPoint pos( cursorPos_.x() - 3, cursorPos_.y() - size_.height() );

    if( pos.x() < 0 )
        pos.setX( 0 );
    if( pos.y() < 0 )
        pos.setY( 0 );

    int dx = pos.x() + size_.width() - widgetSize_.width();
    if( dx > 0 )
        pos.setX( pos.x() - dx );
    
    return pos;
}    

void AggPopupHint::paint ( base_renderer& baseRenderer ) const
{
    if ( alphaCoeff < 0.1 )
        return; // hint is currently invisible

    scanline_rasterizer ras;    
    agg::scanline_p8 sl;
    pixf_dynarow pixf( *buf_ ); 

    if ( ! rendered_ )
    {
        ren_dynarow baseRend( pixf );
        baseRend.clear ( agg::rgba( 100, 100, 100, 0 ) );
        solidRenderer solidRend ( baseRend );
        glyph_gen glyph( font_ );

        agg::trans_affine mtx;
        color_array_type gradColors;
        color_type first( agg::rgba( 1, 1, 1, 0.9 * alphaCoeff ) ),
                   middle( agg::rgba( 30, 30, 30, 0.7 * alphaCoeff ) ),
                   last( agg::rgba( 1, 1, 1, 0.9 * alphaCoeff ) );
        fillColorArray( gradColors, first, middle, last );

        QPoint leftTop( 0, 0 ),
               rightBottom ( size_.width(), size_.height() );

        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, 
                                gradColors, mtx, leftTop, rightBottom, 
                                agg::rgba( 1, 1, 1, 0.3 * alphaCoeff ), 
                                size_.height()/2 - 1, 1, leftTop.y(), 
                                rightBottom.y() );

        QPoint textPos( leftTop.x() + 6, rightBottom.y() - 6 );
        color_type textCol( agg::rgba(0, 0, 0, 1.0 * alphaCoeff) );
        textRenderer textRend ( baseRend, glyph );
        drawText( textRend, text_, textCol, textPos );

        if ( smooth_ == noSmooth )
            rendered_ = true;
    }

    QPoint pos = getHintPosition();
    baseRenderer.blend_from ( pixf, 0, pos.x(), pos.y(), uint( 1.0 * 255 ) );
}

/*****************************************************************************/
