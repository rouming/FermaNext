
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

    if ( inButtonRect( xGlobal, yGlobal ) && ! isPressed() )
        setPressed( true );
    else
        setPressed( false );
}

void AggComboBoxButton::mouseRelease ( int, int )
{
    if ( ! isEnabled() )
        return;

    setPressed( false );    
}

void AggComboBoxButton::drawButton ( ren_dynarow& baseRend, 
                                     DoublePoint pos, int width, 
                                     int height ) const
{
    solidRenderer solidRend( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::trans_affine mtx;

    color_type firstColor( aggColor( 130, 130, 150 ) );
    color_type middleColor( aggColor( 255, 255, 255 ) );
    color_type lastColor( aggColor( 130, 130, 150 ) );

    double lineWidth = 0.8;
    double rad = 2;
    DoublePoint p1( pos ),
                p2( pos.x() + width, pos.y() + height );

    if ( isPressed() ) {
        p1 = p1 + DoublePoint( 1, 1 );
        p2 = p2 + DoublePoint( -1, -1 );
        lineWidth = 0.5;
    }

    color_type lineColor( aggColor( 255, 255, 255, 195 ) );
    if ( isHighlighted() ) {
        lineColor = aggColor( 255, 255, 255, 255 );
        firstColor = aggColor( 150, 150, 170 );
        lastColor = aggColor( 150, 150, 170 );
    }

    color_array_type gradColors;
    fillColorArray( gradColors, firstColor, middleColor, lastColor );

    // Draw button shape
    drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors,
                            mtx, p1, p2, lineColor, rad, 
                            lineWidth, p1.y(), p2.y() );    
/*
    // Draw shadow 
    drawLine( ras, solidRend, sl, DoublePoint( p1.x(), p2.y() - rad - 1 ), 
              DoublePoint( p1.x(), p1.y() + rad + 1 ), 0.3, 
              aggColor( 0, 0, 0, 200 ) );
    drawLine( ras, solidRend, sl, DoublePoint( p2.x() + 1, p2.y() - rad - 1 ), 
              DoublePoint( p2.x() + 1, p1.y() + rad + 1 ), 0.3, 
              aggColor( 0, 0, 0, 200 ) );
*/
    // Draw triangle icon
    double xCentre = ( p2.x() - p1.x() ) / 2;
    double symbIndent = 4.5;
    DoublePoint pt1( p1.x() + xCentre, p1.y() + symbIndent );
    DoublePoint pt2( p2.x() - symbIndent, p2.y() - symbIndent );
    DoublePoint pt3( p1.x() + symbIndent, p2.y() - symbIndent );
    drawTriangle( baseRend, pt1, pt2, pt3, aggColor( 100, 100, 125 ), lineColor );
}

void AggComboBoxButton::paint ( base_renderer& baseRenderer ) const
{
    pixf_dynarow pixf( *buttonBuf );

    if ( ! isRendered() ) {
        ren_dynarow baseRend( pixf );
        baseRend.clear( agg::rgba( 1, 1, 1, 0 ) );

        solidRenderer solidRend( baseRend );
        scanline_rasterizer ras;
        agg::scanline_p8 sl;
        agg::trans_affine mtx;
    
        color_type firstColor( aggColor( 155, 155, 175, 155 ) );
        color_type middleColor( aggColor( 255, 255, 255, 255 ) );
        color_type lastColor( aggColor( 155, 155, 175, 155 ) );

        double lineWidth = 0.8;
        int pushButtonHeight = int(getHeight() - lineWidth * 2);
        double rad = 3;
        DoublePoint p1( lineWidth, lineWidth ),
                    p2( getWidth() - lineWidth, getHeight() - lineWidth );


        color_type lineColor( aggColor( 255, 255, 255, 255 ) );
        if ( isHighlighted() ) {
            //lineColor = aggColor( 255, 255, 255, 255 );
            middleColor = aggColor( 235, 235, 245, 205 );
            lineWidth = 1.6;
        }

        color_array_type gradColors;
        fillColorArray( gradColors, firstColor, middleColor, lastColor );
        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors,
                                mtx, p1, p2, lineColor, rad, 
                                lineWidth, 0, getHeight() );

        // width == height
        drawButton( baseRend, 
                    DoublePoint( p2.x() - pushButtonHeight, p1.y() ),
                    pushButtonHeight, pushButtonHeight );

        glyph_gen glyph( agg::verdana14_bold );
        textRenderer textRend( baseRend, glyph );

        int widthLim = getWidth() - 5 - pushButtonHeight;
        QString str = fitTextToWidth( text(), widthLim, glyph );

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
    leftTopPos( QPoint( 0, 0 ) ),
    currIdx( -1 )
{
    // Set button constant height
    textFont font = menu->getActionFont();
    glyph_gen glyph( font );
    int indent = 3;
    button->setHeight( (int)glyph.height() + indent * 2 );

    connect( menu, SIGNAL(actionTriggered( int )),
             this, SLOT(setCurrentIndex( int )) ); 

    connect( menu, SIGNAL( renderedFlagCleared() ),
             &parentWidget, SLOT( update() ) );

    connect( button, SIGNAL(onButtonPress()),
                       SLOT(showMenu()) ); 

    connect( button, SIGNAL( onChangeButtonState() ),
             &parentWidget, SLOT( update() ) );

}

AggComboBox::~AggComboBox ()
{
    clear();
    delete menu;
    delete button;
}

int AggComboBox::width () const
{
    return button->getWidth();
}

int AggComboBox::height () const
{
    return button->getHeight();
}

void AggComboBox::resize ( int w, int h ) 
{
    button->resize( w, h );
    emit sizeChanged();
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
    
    resize( menu->width(), button->getHeight() );

    updateMenuShowPosition();

    if ( currIdx == -1 )
        setCurrentIndex( 0 );

    designerWidget.update();
}

void AggComboBox::addItem ( const QString& text, QVariant data )
{
    AggMenuAction& action = menu->addAction( text );
    dataMap.insert( &action, data );
    
    resize( menu->width(), button->getHeight() );

    updateMenuShowPosition();
    
    if ( currIdx == -1 )
        setCurrentIndex( 0 );

    designerWidget.update();
}

void AggComboBox::removeItem ( int idx )
{
    menu->removeAction( idx );
    updateMenuShowPosition();

    if ( ! count() )
        setCurrentIndex( -1 );
    else if ( idx - 1 >= 0 )
        setCurrentIndex( idx - 1 );
    else
        setCurrentIndex( idx );

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

    currIdx = idx;

    designerWidget.update();
}

void AggComboBox::setItemText ( int idx, const QString& text )
{
    AggMenuAction* action = menu->getAction( idx );
    if ( action )
        action->setText( text );

    designerWidget.update();
}

void AggComboBox::setItemData ( int idx, QVariant data )
{
    AggMenuAction* action = menu->getAction( idx );
    if ( action )
        dataMap.insert( action, data );
}

int AggComboBox::count () const
{
    return menu->countActions();
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

void AggComboBox::showMenu ()
{
    if ( menu->isVisible() ) {
        menu->hide();
        return;
    }
    
    AggMenuAction* action = menu->getAction( currentIndex() );
    menu->removeHighlight();
    action->highlight( true );
    menu->show();
}

void AggComboBox::mouseMove ( int xGlobal, int yGlobal )
{
    if ( menu->isVisible() )
        menu->mouseMove( xGlobal, yGlobal );

    button->mouseMove( xGlobal, yGlobal );
}

void AggComboBox::mousePress ( int xGlobal, int yGlobal )
{
    button->mousePress( xGlobal, yGlobal );

    if ( menu->isVisible() && ! button->inButtonRect( xGlobal, yGlobal ) )
        menu->mousePress( xGlobal, yGlobal );

}

void AggComboBox::mouseRelease ( int xGlobal, int yGlobal )
{
    button->mouseRelease( xGlobal, yGlobal );

    if ( menu->isVisible() )
        menu->mouseRelease( xGlobal, yGlobal );
}

void AggComboBox::paint ( base_renderer& baseRenderer ) const
{
    if ( menu->isVisible() && count() )
        menu->paint( baseRenderer );
    button->paint( baseRenderer );
}

