
#include "AggQWidget.h"

#include <qpainter.h>
#include <qsize.h>
#include <platform/agg_platform_support.h>

/*****************************************************************************
 * Agg Ctrls Container
 *****************************************************************************/

AggCtrlContainer::AggCtrlContainer () : 
    current(0)
{}

void AggCtrlContainer::addCtrl ( agg::ctrl& c )
{
    ctrls.push_back(&c);
}

bool AggCtrlContainer::removeCtrl ( agg::ctrl& c )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if ( (*iter) == &c ) {            
            ctrls.erase(iter);
            return true;
        }
    return false;
}

bool AggCtrlContainer::inRect ( double x, double y )
{
    const agg::ctrl* ctrl = whoIsInRect( x, y );
    if ( ctrl != 0 ) return true;
    return false;
}

const agg::ctrl* AggCtrlContainer::whoIsInRect ( double x, double y )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->in_rect(x, y) )
            return *iter;
    return 0;
}

bool AggCtrlContainer::currentCtrl ( double x, double y )
{ 
    for ( uint i = 0; i < ctrls.size(); ++i ) {
        if( ctrls[i]->in_rect(x, y) ) {
            if ( current != ctrls[i] ) {
                current = ctrls[i];
                return true;
            }
            return false;
        }
    }
    if ( current != 0 ) { 
        current = 0;
        return true;
    }
    return false;
}

const agg::ctrl& AggCtrlContainer::currentCtrl ()
{
    return *current;
}


const agg::ctrl* AggCtrlContainer::mousePressEvent ( double x, double y )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_button_down(x, y) ) 
            return *iter;
    return 0;
}

const agg::ctrl* AggCtrlContainer::mouseReleaseEvent ( double x, double y )
{
    const agg::ctrl* releasedCtrl = 0;
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_button_up(x, y) ) 
            releasedCtrl = *iter;
    return releasedCtrl;
}

const agg::ctrl* AggCtrlContainer::mouseMoveEvent ( double x, double y, bool buttonFlag )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_move(x, y, buttonFlag) ) 
            return *iter;
    return 0;
}

const agg::ctrl* AggCtrlContainer::arrowKeysPressEvent ( bool left, bool right, 
                                                         bool down, bool up )
{
    if ( current != 0 && 
         current->on_arrow_keys( left, right, down, up ) )
        return current;
    return 0;
}


/*****************************************************************************
 * Agg QWidget
 *****************************************************************************/

AggQWidget::AggQWidget ( QWidget* parent,                         
                         bool flip_y ) :
    QWidget( parent ),
    aggFlipY( flip_y ),
    mainQImage( new QImage )
    
{
    setBackgroundMode( QWidget::NoBackground );
    setMinimumSize( QSize(100, 100) );
    resize( QSize(360, 330) );
    initialWidth  = width();
    initialHeight = height();
}

AggQWidget::~AggQWidget ()
{
    delete mainQImage;
}

agg::rendering_buffer& AggQWidget::getAggRenderingBuffer ()
{
    return aggBuffer;
}

agg::trans_affine AggQWidget::getAggResizeMatrix ()
{
    return agg::trans_affine_scaling(
                      double(width()) / double(initialWidth),
                      double(height()) /double(initialHeight) );
}

QImage& AggQWidget::getMainImage ()
{
    return *mainQImage;
}

bool AggQWidget::flipY ()
{
    return aggFlipY;
}

void AggQWidget::flipY ( bool flip )
{
    aggFlipY = flip;
}


/*****************************************************************************
 * Private QT event handlers. Override AGG handlers instead.
/****************************************************************************/

void AggQWidget::paintEvent ( QPaintEvent* event )
{
    // Notice other agg paintable elements 
    aggPaintEvent( event );


    // This conversion was taken from the original agg_plarform_support.cpp
    QImage tmpImg( mainQImage->width(), mainQImage->height(), 32 );
					
    agg::rendering_buffer rbufTmp;
    rbufTmp.attach( tmpImg.bits(), tmpImg.width(), tmpImg.height(), 
                    aggFlipY ? tmpImg.bytesPerLine() : -tmpImg.bytesPerLine() );
    
        
    agg::color_conv(&rbufTmp, &aggBuffer, agg::color_conv_rgb24_to_rgba32());
    bitBlt( this, 0,0, &tmpImg );  
}

void AggQWidget::resizeEvent ( QResizeEvent* event )
{
    const QSize& oldSize = event->oldSize();
    const QSize& newSize = event->size();


    // Resize the internal buffer and reattach the main 
    // AGG rendering_buffer to the QImage 
    mainQImage->create( newSize, 32 );
    aggBuffer.attach( mainQImage->bits(), mainQImage->width(), 
                      mainQImage->height(), 
                      aggFlipY ?  mainQImage->bytesPerLine() : 
                                 -mainQImage->bytesPerLine() );
	
    // Notice other agg paintable elements 
    aggResizeEvent( event ); 
}

void AggQWidget::keyPressEvent ( QKeyEvent* ke )
{
    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;

    switch( ke->key() )
    {
        case Key_Up:    up    = true; break;
        case Key_Down:  down  = true; break;
        case Key_Left:  left  = true; break;
        case Key_Right: right = true; break;
        default:    break;
    }
				
    const agg::ctrl* ctrl = ctrlContainer.arrowKeysPressEvent( left, right, down, up );
    if( ctrl != 0 )
    {
        aggCtrlChangedEvent( ctrl );
        // Force repaint
        QWidget::update();
    }
    else
    {
        aggKeyPressEvent( ke );
    }    
}

void AggQWidget::mouseMoveEvent ( QMouseEvent* me )
{			
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= agg::kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= agg::kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= agg::mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= agg::mouse_right;
    // for some reason, Qt doesnt set the button on mouse move
    // events, even though I know I'm holding the button down!
    // Since I know that I wont get mouse move events unless
    // the mouse is pressed, I'll make a 90%-correct assumption
    // here and just set the flag anyway.
    if( !flags ) flags |= agg::mouse_left; 
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();
		

    const agg::ctrl* ctrl = ctrlContainer.mouseMoveEvent( curX, curY,
                                                          (flags & agg::mouse_left) != 0 );
    if ( ctrl != 0 )
    {
        aggCtrlChangedEvent( ctrl );
        // Force repaint
        QWidget::update();

    }
    else
    {
        ctrl = ctrlContainer.whoIsInRect(curX, curY);
        if ( ctrl == 0 )
        {            
            QPoint  newPos( curX, curY );
            QMouseEvent newEvent( me->type(), newPos, me->button(), me->state() );
            aggMouseMoveEvent( &newEvent );
        }
    } 
} 

void AggQWidget::mouseReleaseEvent ( QMouseEvent* me )
{
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= agg::kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= agg::kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= agg::mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= agg::mouse_right;
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();

    if ( flags & agg::mouse_left )
    {
        const agg::ctrl* ctrl = ctrlContainer.mouseReleaseEvent(curX, curY);
        if ( ctrl != 0 )
        {
            aggCtrlChangedEvent( ctrl );
            // Force repaint
            QWidget::update();

        }
    }
    if ( flags & (agg::mouse_left | agg::mouse_right) )
    {
        QPoint  newPos( curX, curY );
        QMouseEvent newEvent( me->type(), newPos, me->button(), me->state() );
        aggMouseReleaseEvent( &newEvent );
    }
}

void AggQWidget::mousePressEvent ( QMouseEvent* me )
{
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= agg::kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= agg::kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= agg::mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= agg::mouse_right;
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();

    if ( flags & agg::mouse_left )
    {
        const agg::ctrl* ctrl = ctrlContainer.mousePressEvent(curX, curY);
        if( ctrl != 0 )
        {
            ctrlContainer.currentCtrl(curX, curY);
            aggCtrlChangedEvent( ctrl );            
            // Force repaint
            QWidget::update();
        }
        else
        {
            const agg::ctrl* ctrl = ctrlContainer.whoIsInRect(curX, curY);
            if ( ctrl != 0 )
            {                
                if ( ctrlContainer.currentCtrl(curX, curY) )
                {
                    aggCtrlChangedEvent( ctrl );  
				    // Force repaint
                    QWidget::update();
                }
            }
            else
            {
                QPoint  newPos( curX, curY );
                QMouseEvent newEvent( me->type(), newPos, me->button(), me->state() );
                aggMousePressEvent( &newEvent );                
            }    
        }
    }
    if ( flags & agg::mouse_right )
    {
        QPoint  newPos( curX, curY );
        QMouseEvent newEvent( me->type(), newPos, me->button(), me->state() );
        aggMousePressEvent( &newEvent );
    }
}
 

/*****************************************************************************
 * Override this AGG event handlers in descendant classes.
/****************************************************************************/

void AggQWidget::aggPaintEvent ( QPaintEvent* )     {}
void AggQWidget::aggResizeEvent ( QResizeEvent* )   {}
void AggQWidget::aggKeyPressEvent ( QKeyEvent* )    {}
void AggQWidget::aggMouseMoveEvent ( QMouseEvent* ) {}
void AggQWidget::aggMouseReleaseEvent ( QMouseEvent* ) {}
void AggQWidget::aggMousePressEvent ( QMouseEvent* )   {}
void AggQWidget::aggCtrlChangedEvent ( const agg::ctrl* ) {}


