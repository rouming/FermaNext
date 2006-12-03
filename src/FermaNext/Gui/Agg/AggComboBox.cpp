
#include <QWidget>

#include "Geometry.h"
#include "AggComboBox.h"
#include "AggPopupMenu.h"

/*****************************************************************************
 * AggComboBox Button
 *****************************************************************************/

AggComboBoxButton::AggComboBoxButton () :
    buttonBuf( new rbuf_dynarow( 0, 0 ) ),
    rendered( false )
{
    connect( this, SIGNAL( onChangeButtonState() ),
                     SLOT( clearRenderedFlag() ) );
}

AggComboBoxButton::~AggComboBoxButton ()
{
    delete buttonBuf;
}

void AggComboBoxButton::resize ( int w, int h )
{
    setWidth( w );
    setHeight( h );
    buttonBuf->init( w, h );
    clearRenderedFlag();
}

bool AggComboBoxButton::isRendered () const
{
    return rendered;
}

void AggComboBoxButton::clearRenderedFlag () 
{
    rendered = false;
}

QPoint AggComboBoxButton::fromGlobalPos ( int x, int y ) const
{
    return QPoint( x - getPosition().x(), y - getPosition().y() );
}

void AggComboBoxButton::mouseMove ( int xGlobal, int yGlobal )
{
    if ( ! isEnabled() )
        return;
    
    if ( inButtonRect( xGlobal, yGlobal ) )
        setHighlighted( true );
    else 
        setHighlighted( false );
}

void AggComboBoxButton::mousePress ( int xGlobal, int yGlobal )
{
    if ( ! isEnabled() )
        return;

    if ( inButtonRect( xGlobal, yGlobal ) )
        setPressed( true );
    else
        setPressed( false );
}

void AggComboBoxButton::mouseRelease ( int, int )
{
    if ( ! isEnabled() )
        return;

    //setPressed( false );    
}

void AggComboBoxButton::paint ( base_renderer& baseRenderer ) const
{
    pixf_dynarow pixf( *buttonBuf );

    if ( ! isRendered() ) {
        ren_dynarow baseRend( pixf );
        baseRend.clear( agg::rgba8( 1, 1, 1, 0 ) );

        solidRenderer solidRend( baseRend );
        scanline_rasterizer ras;
        agg::scanline_p8 sl;
        agg::trans_affine mtx;
    
        color_type firstColor( agg::rgba( 1, 1, 1, 0.5 ) );
        color_type middleColor;
        color_type lastColor( agg::rgba( 1, 1, 1, 0.5 ) );
        
        if ( isHighlighted() )
            middleColor = agg::rgba8( 132, 235, 255, 0.5 );
        else
            middleColor = agg::rgba8( 255, 239, 210, 0.6 );

        //if ( isPressed() )
        //    middleColor = agg::rgba8( 132, 235, 255, 0.5 );

        color_array_type gradColors;

        fillColorArray( gradColors, firstColor, middleColor, lastColor );

        double lineWidth = 1.5;
        double rad = 3;
        DoublePoint p1( lineWidth, lineWidth ),
                    p2( getWidth() - lineWidth, getHeight() - lineWidth );

        if ( isPressed() ) {
            p1 = p1 + DoublePoint( 1, 1 );
            p2 = p2 + DoublePoint( -1, -1 );
        }

        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors,
                                mtx, p1, p2, agg::rgba( 1, 1, 1, 0.6 ), rad, 
                                lineWidth, 0, getHeight() );

        glyph_gen glyph( agg::verdana14_bold );
        textRenderer textRend( baseRend, glyph );

        QString str = fitTextToWidth( text(), getWidth() - 5, glyph );
        int yIndent = getHeight() - ( getHeight() - glyph.height() ) / 2;
        DoublePoint textPos( 4, glyph.height() );
        drawText( textRend, str, agg::rgba( 0, 0, 0 ), textPos );

        rendered = true;
    }

    QPoint leftTopPos = getPosition();
    baseRenderer.blend_from( pixf, 0, leftTopPos.x(), leftTopPos.y(), 
                             unsigned( 1.0 * 255 ) );
}

/*****************************************************************************
 * AggComboBox
 *****************************************************************************/

AggComboBox::AggComboBox ( QWidget& parentWidget ) :
    menu( new AggPopupMenu() ),
    button( new AggComboBoxButton() ),
    designerWidget( parentWidget ),
    leftTopPos( QPoint( 0, 0 ) )
{
    // Set button constant height
    textFont font = menu->getActionFont();
    glyph_gen glyph( font );
    int indent = 3;
    button->setHeight( (int)glyph.height() + indent * 2 );

    connect( menu, SIGNAL(actionTriggered( int )),
             this, SLOT(setCurrentIndex( int )) ); 
    
    connect( button, SIGNAL(onButtonPress()),
               menu, SLOT(show()) ); 

}

AggComboBox::~AggComboBox ()
{
    delete menu;
    delete button;
    clear();
}

QPoint AggComboBox::getPosition () const
{
    return leftTopPos;
}

void AggComboBox::setPosition ( QPoint pos )
{
    leftTopPos = pos;
    menu->setPosition( leftTopPos );
    button->setPosition( leftTopPos );
    
    updateMenuShowPosition();

    designerWidget.update();
}

void AggComboBox::addItem ( const QString& text )
{
    menu->addAction( text );
    button->setText( text );
    
    // Resize combobox button to the new content 
    button->resize( menu->width(), button->getHeight() );

    updateMenuShowPosition();

    designerWidget.update();
}

void AggComboBox::removeItem ( int idx )
{
    if ( idx == currIdx )
        setCurrentIndex( idx - 1 );
 
    menu->removeAction( idx );
    updateMenuShowPosition();

    designerWidget.update();
}

void AggComboBox::setCurrentIndex ( int idx )
{
    AggMenuAction* action = menu->getAction( idx );
    if ( action ) {
        button->setText( action->text() );
        emit currentIndexChanged( idx );
    }
    else 
        button->setText( "" );

    designerWidget.update();
}

void AggComboBox::setItemData ( int idx, QVariant data )
{
    AggMenuAction* action = menu->getAction( idx );
    if ( action )
        dataMap.insert( action, data );
}

QString AggComboBox::currentText () const
{
    return button->text();
}

int AggComboBox::currentIndex () const
{
    return currIdx;
}

QVariant AggComboBox::itemData ( int idx ) const
{
    AggMenuAction* action = menu->getAction( idx );
    return dataMap.value( action );
}

void AggComboBox::clear ()
{
    menu->clear();
    dataMap.clear();
}

void AggComboBox::updateMenuShowPosition ()
{
    QPoint menuPos;
    menuPos.setX( leftTopPos.x() );
    menuPos.setY( leftTopPos.y() - menu->height() );
    menu->setPosition( menuPos );
}

void AggComboBox::mouseMove ( int xGlobal, int yGlobal )
{
    if ( menu->isVisible() )
        menu->mouseMove( xGlobal, yGlobal );

    button->mouseMove( xGlobal, yGlobal );
}

void AggComboBox::mousePress ( int xGlobal, int yGlobal )
{
    if ( menu->isVisible() )
        menu->mousePress( xGlobal, yGlobal );

    button->mousePress( xGlobal, yGlobal );
}

void AggComboBox::mouseRelease ( int xGlobal, int yGlobal )
{
    if ( menu->isVisible() )
        menu->mouseRelease( xGlobal, yGlobal );
    
    button->mouseRelease( xGlobal, yGlobal );   
}

void AggComboBox::paint ( base_renderer& baseRenderer ) const
{
    if ( menu->isVisible() )
        menu->paint( baseRenderer );
    button->paint( baseRenderer );
}

