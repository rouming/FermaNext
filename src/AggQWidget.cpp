
#include "AggQWidget.h"

using namespace agg;

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
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->in_rect(x, y) )
            return true;
    return false;
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


bool AggCtrlContainer::mousePressEvent ( double x, double y )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_button_down(x, y) ) 
            return true;
    return false;
}

bool AggCtrlContainer::mouseReleaseEvent ( double x, double y )
{
    bool flag = false;
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_button_up(x, y) ) 
            flag = true;
    return flag;
}

bool AggCtrlContainer::mouseMoveEvent ( double x, double y, bool buttonFlag )
{
    CtrlList::iterator iter = ctrls.begin();
    for ( ; iter != ctrls.end(); ++iter )
        if( (*iter)->on_mouse_move(x, y, buttonFlag) ) 
            return true;
    return false;
}

bool AggCtrlContainer::arrowKeysPressEvent ( bool left, bool right, 
                                             bool down, bool up )
{
    if ( current != 0 )
        return current->on_arrow_keys( left, right, down, up );
    return false;
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
					
    rendering_buffer rbufTmp;
    rbufTmp.attach( tmpImg.bits(), tmpImg.width(), tmpImg.height(), 
                    aggFlipY ? tmpImg.bytesPerLine() : -tmpImg.bytesPerLine() );
    
        
    color_conv(&rbufTmp, &aggBuffer, color_conv_rgb24_to_rgba32());
    bitBlt( this, 0,0, &tmpImg );  
}

void AggQWidget::resizeEvent ( QResizeEvent * event )
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

void AggQWidget::keyPressEvent ( QKeyEvent * event )
{
    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;

    switch( event->key() )
    {
	    case Key_Up:    up    = true; break;
        case Key_Down:  down  = true; break;
        case Key_Left:  left  = true; break;
        case Key_Right: right = true; break;
        default:    break;
	}
				
	if( ctrlContainer.arrowKeysPressEvent( left, right, down, up ) )
    {
        //m_ptr->on_ctrl_change();
        //m_ptr->force_redraw();
    }
    else
    {
        //m_ptr->on_key(QCursor::pos().x(),
       //flipY ? mainQWidget()->height() - QCursor::pos().y() : 
               //QCursor::pos().y(), ke->key(), // Note: I don't know if this maps directly to AGG...
        //0);
    }    
}

void AggQWidget::mouseMoveEvent ( QMouseEvent* me )
{			
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= mouse_right;
    // for some reason, Qt doesnt set the button on mouse move
    // events, even though I know I'm holding the button down!
    // Since I know that I wont get mouse move events unless
    // the mouse is pressed, I'll make a 90%-correct assumption
    // here and just set the flag anyway.
    if( !flags ) flags |= mouse_left; 
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();
		
    if ( ctrlContainer.mouseMoveEvent( curX, curY, 
                                      (flags & mouse_left) != 0 ) )
    {
        //m_ptr->on_ctrl_change();
        //m_ptr->force_redraw();
    }
    else
    {
        if ( !ctrlContainer.inRect(curX, curY) )
        {
            //aggMouseMoveEvent(curX, curY, flags);
        }
    } 
} 

void AggQWidget::mouseReleaseEvent ( QMouseEvent* me )
{
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= mouse_right;
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();

    if ( flags & mouse_left )
    {
        if ( ctrlContainer.mouseReleaseEvent(curX, curY) )
        {
         //   m_ptr->on_ctrl_change();
            //m_ptr->force_redraw();
        }
    }
    if ( flags & (mouse_left | mouse_right) )
    {
        //m_ptr->on_mouse_button_up(curX, curY, flags);
    }
}

void AggQWidget::mousePressEvent ( QMouseEvent* me )
{
    uint flags = 0;
    if ( me->button() & Qt::ShiftButton)   flags |= kbd_shift;
    if ( me->button() & Qt::ControlButton) flags |= kbd_ctrl;
    if ( me->button() == Qt::LeftButton)   flags |= mouse_left;
    if ( me->button() == Qt::RightButton)  flags |= mouse_right;
				
    int curX = me->pos().x();
    int curY = flipY() ? height() - me->pos().y() : me->pos().y();

    if ( flags & mouse_left )
    {
        if( ctrlContainer.mousePressEvent(curX, curY) )
        {
            ctrlContainer.currentCtrl(curX, curY);
            //m_ptr->on_ctrl_change();
            //m_ptr->force_redraw();
        }
        else
        {
            if ( ctrlContainer.inRect(curX, curY) )
            {
		        if ( ctrlContainer.currentCtrl(curX, curY) )
                {
                    //m_ptr->on_ctrl_change();
					//m_ptr->force_redraw();
			    }
            }
            else
            {
                //m_ptr->on_mouse_button_down(cur_x, cur_y, flags);
            }    
        }
	}
    if ( flags & mouse_right )
    {
        //m_ptr->on_mouse_button_down(cur_x, cur_y, flags);
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


