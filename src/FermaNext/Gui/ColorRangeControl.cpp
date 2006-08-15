
#include <QMouseEvent>

#include "ColorRangeControl.h"
#include "TrussSolutionResults.h"

/*****************************************************************************
 * Color Control Button
 *****************************************************************************/

ColorControlButton::ColorControlButton ( QPoint pos, int w, int h ) :
    AggButton( pos, w, h )
{}

void ColorControlButton::paint ( base_renderer& baseRend, 
                                 bool plusButton ) const
{
    QPoint pos = getPosition();
    int w = getWidth(), h = getHeight();

    agg::rgba8 color( 230, 220, 210 );
    agg::rgba8 markerLineCol( agg::rgba8( 125, 135, 145 ) );
    agg::rounded_rect roundRect( pos.x(), pos.y(),
                                 pos.x() + w, pos.y() + h, 3 );
    if ( ! isEnabled() ) {
        color = agg::rgba8( 250, 240, 230 );
        markerLineCol = agg::rgba8( 145, 155, 170 );
    }
    else if ( isPressed() ) {
        roundRect.rect( pos.x() + 1, pos.y() + 1,
                        pos.x() + w - 1, pos.y() + h - 1 );
        color = agg::rgba8( 220, 200, 190 );
        markerLineCol = agg::rgba8( 105, 120, 130 );
    }
    
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    ras.add_path( roundRect );
    solid_renderer solidRend( baseRend );
    solidRend.color( color );
    agg::render_scanlines( ras, sl, solidRend );
   
    PrimRenderer primRend( baseRend );
    primRend.line_color( markerLineCol );
    primRend.fill_color( agg::rgba8( 210, 180, 175 ) );
    
    if ( plusButton ) {
        if ( isPressed() ) {
            primRend.outlined_rectangle( pos.x() + 4, pos.y() + h / 2 - 1,
                                         pos.x() + w - 5, pos.y() + h / 2 + 1 );
            primRend.outlined_rectangle( pos.x() + w / 2 - 1, pos.y() + 4,
                                         pos.x() + w / 2 + 1, pos.y() + h - 5 );
        }
        else {
            primRend.outlined_rectangle( pos.x() + 3, pos.y() + h / 2 - 1,
                                         pos.x() + w - 4, pos.y() + h / 2 + 1 );
            primRend.outlined_rectangle( pos.x() + w / 2 - 1, pos.y() + 3,
                                         pos.x() + w / 2 + 1, pos.y() + h - 4 );
        }
    } 
    else
        if ( isPressed() )
            primRend.outlined_rectangle( pos.x() + 4, pos.y() + h / 2 - 1,
                                         pos.x() + w - 5, pos.y() + h / 2 + 1 );
        else
            primRend.outlined_rectangle( pos.x() + 3, pos.y() + h / 2 - 1,
                                         pos.x() + w - 4, pos.y() + h / 2 + 1 );
    
    agg::conv_stroke<agg::rounded_rect> outline( roundRect );
    outline.width( 1.0 );
    ras.add_path( outline );
    solidRend.color( agg::rgba8( 80, 80, 93 ) );
    agg::render_scanlines( ras, sl, solidRend );
}

/*****************************************************************************
 * Color Range Control
 *****************************************************************************/

ColorRangeControl::ColorRangeControl ( QWidget* parent ) :
    AggWidget( parent, Global::flipY ),
    levelsNumb( 10 ),
    plusButton( new ColorControlButton( QPoint( width() - 18, 20 ), 
                                        15, 15 ) ),
    minusButton( new ColorControlButton( QPoint( width() - 18, 
                                         height() - 20 ), 15, 15 ) ),
    pressedButton( 0 ),
    maxStress( 0 ),
    minStress( 0 ),
    mousePressed( false )
{
    setMouseTracking( true );
    QObject::connect( plusButton, SIGNAL( onChangeButtonState() ),
                                  SLOT( update() ) );

    QObject::connect( minusButton, SIGNAL( onChangeButtonState() ),
                                   SLOT( update() ) );
}

ColorRangeControl::~ColorRangeControl ()
{
    delete plusButton;
    delete minusButton;
}

agg::rgba8 ColorRangeControl::getColorForStress ( double stress ) const
{
    if ( ! maxStress && ! minStress )
        return agg::rgba8( 0, 0, 0 );
    
    double stressInc = maxStress / ( levelsNumb / 2 );
    agg::rgba8 beginColor( 0, 10, 180 );
    agg::rgba8 endColor( 150, 155, 255 );
    if ( stress < 0 ) {
        stressInc = minStress / ( levelsNumb / 2 - 1 );
        beginColor = agg::rgba8( 220, 180, 165 );
        endColor = agg::rgba8( 125, 70, 40 );
    }
    int dr = int( ( endColor.r - beginColor.r ) / ( levelsNumb / 2 ) ),
        dg = int( ( endColor.g - beginColor.g ) / ( levelsNumb / 2 ) ),
        db = int( ( endColor.b - beginColor.b ) / ( levelsNumb / 2 ) );
    double n = stress / stressInc;
    n < 1.0 ? n = 0 : n = ceil( n );
    if ( stress > 0 )
        return agg::rgba8( endColor.r - dr * n, 
                           endColor.g - dg * n, 
                           endColor.b - db * n );
    return agg::rgba8( beginColor.r + dr * n, 
                       beginColor.g + dg * n, 
                       beginColor.b + db * n );
}

void ColorRangeControl::changeLevelsNumber ( bool inc )
{
    if ( inc && levelsNumb < 12 )
        levelsNumb += 2;
    else if ( ! inc && levelsNumb > 4 )
        levelsNumb -= 2;
    emit onLevelsNumberChange();
}

void ColorRangeControl::updateStressRange ( const LoadCaseResults& res )
{
    maxStress = 0; 
    minStress = 0;
    
    if ( ! res.countStresses() ) {
        plusButton->setEnabled( false );
        minusButton->setEnabled( false );
        return;
    }
    plusButton->setEnabled( true );
    minusButton->setEnabled( true );

    for ( uint i = 0; i < res.countStresses(); ++i ) {
        bool valid;
        double stress = res.getStress( i, &valid );
        if ( ! valid )
            continue;
        if ( stress > maxStress )
            maxStress = stress;
        else if ( stress < minStress )
            minStress = stress;
    }
    update();
}

void ColorRangeControl::aggPaintEvent ( QPaintEvent* event )
{
    pixfmt pixf( getAggRenderingBuffer() );
    base_renderer baseRend( pixf );
    baseRend.clear( agg::rgba( 1, 1, 1 ) );
    plusButton->paint( baseRend, true );
    minusButton->paint( baseRend, false );
    paint( baseRend );
}

void ColorRangeControl::aggResizeEvent ( QResizeEvent* )
{
    colorLineGeometry.setCoords( width() - 18, 40, 
                                 width() - 3, height() - 40 );
    plusButton->setPosition( QPoint( width() - 18, 20 ) );
    minusButton->setPosition( QPoint( width() - 18, height() - 35 ) );
}

void ColorRangeControl::aggKeyPressEvent ( QKeyEvent* )
{}

void ColorRangeControl::aggMouseMoveEvent ( QMouseEvent* me )
{
    int x = me->x(),
        y = Global::flipY ? height() - me->y() : me->y();

    if ( plusButton->inButtonRect( x, y ) )
    {
        if( mousePressed && plusButton == pressedButton )
            plusButton->setPressed( true );
    }
    else if ( minusButton->inButtonRect( x, y ) )
    {
        if( mousePressed && minusButton == pressedButton )
            minusButton->setPressed( true );
    }
    else {
        plusButton->setPressed( false );
        minusButton->setPressed( false );
    }
}

void ColorRangeControl::aggMouseReleaseEvent ( QMouseEvent* me )
{
    int x = me->x(),
        y = Global::flipY ? height() - me->y() : me->y();
    
    if ( plusButton->inButtonRect( x, y ) )
    {
        if ( plusButton->isPressed() )
        {
            plusButton->setPressed( false );
            changeLevelsNumber( true );
        }
    }
    else if ( minusButton->inButtonRect( x, y ) )
    {
        if ( minusButton->isPressed() )
        {
            minusButton->setPressed( false );
            changeLevelsNumber( false );
        }
    }
    pressedButton = 0;
    mousePressed = false;
}

void ColorRangeControl::aggMousePressEvent ( QMouseEvent* me )
{
    int y = Global::flipY ? height() - me->y() : me->y();
    if ( plusButton->inButtonRect( me->x(), y ) && 
        plusButton->isEnabled() ) {
        mousePressed = true;
        plusButton->setPressed( true );
        pressedButton = plusButton;
    }

    else if ( minusButton->inButtonRect( me->x(), y ) && 
              minusButton->isEnabled() )
    {
        mousePressed = true;
        minusButton->setPressed( true );
        pressedButton = minusButton;
    }
}

void ColorRangeControl::aggCtrlChangedEvent ( const agg::ctrl* )
{}

double ColorRangeControl::drawAHalfLine ( base_renderer& baseRend,
                                          const agg::rgba8& beginColor,
                                          const agg::rgba8& endColor,
                                          double y ) const
{
    PrimRenderer primRend( baseRend );
    primRend.line_color( agg::rgba( 1, 1, 1 ) );
    double dy = colorLineGeometry.height() / levelsNumb;
    int dr = int( ( endColor.r - beginColor.r )  / ( levelsNumb / 2 - 1 ) ),
        dg = int( ( endColor.g - beginColor.g )  / ( levelsNumb / 2 - 1 ) ),
        db = int( ( endColor.b - beginColor.b ) / ( levelsNumb / 2 - 1 ) );
    int r = beginColor.r,
        g = beginColor.g,
        b = beginColor.b;
    
    for ( uint i = 0; i < levelsNumb / 2; ++i ) {
        primRend.fill_color( agg::rgba8( b, g, r ) );
        primRend.outlined_rectangle( colorLineGeometry.left(), 
                                     y, colorLineGeometry.right(),
                                     y + dy );
        y += dy;
        r += dr;
        g += dg;
        b += db;
    } 
    return y;
}

void ColorRangeControl::drawStrokeNumber ( text_renderer& textRend, 
                                           glyph_gen& glyph,
                                           double value, 
                                           double strokeYPos ) const
{
    QString str = QString("%1").arg( value );
    double textLength = glyph.width( str.toAscii().data() ),
           textHeight = glyph.height();
    textRend.color( agg::rgba( 0, 0, 0 ) );
    textRend.render_text( colorLineGeometry.left() - 4 - textLength, 
                          strokeYPos + textHeight/4, 
                          str.toAscii().data(), Global::flipY );
}

void ColorRangeControl::signColorLine ( base_renderer& baseRend ) const
{
    glyph_gen glyph( agg::verdana12 );
    text_renderer textRend( baseRend, glyph );
    
    double dy = colorLineGeometry.height() / levelsNumb,
           y = colorLineGeometry.top();
    
    double stressInc = -maxStress / ( levelsNumb / 2 ),
           currStress = maxStress;
    for ( uint i = 0; i < levelsNumb / 2; ++i ) {
        drawStrokeNumber( textRend, glyph, currStress , y );
        currStress += stressInc;
        y += dy;
    }
    
    drawStrokeNumber( textRend, glyph, 0 , y );
    y += dy;

    stressInc = minStress / ( levelsNumb / 2 );
    currStress = stressInc;
    for ( uint i = 0; i < levelsNumb / 2; ++i ) {
        drawStrokeNumber( textRend, glyph, currStress , y );
        y += dy;
        currStress += stressInc;
    } 
}

void ColorRangeControl::drawColorLine ( base_renderer& baseRend ) const
{
    agg::rgba8 beginColor( 180, 10, 0 );
    agg::rgba8 endColor( 255, 155, 150 );
    double y = drawAHalfLine( baseRend, beginColor, 
                              endColor, 
                              colorLineGeometry.top() );
    
    beginColor = agg::rgba8( 165, 180, 220 );
    endColor = agg::rgba8( 40, 70, 125 );
    drawAHalfLine( baseRend, beginColor, endColor, y );
    signColorLine( baseRend );
}

void ColorRangeControl::paint ( base_renderer& baseRend ) const
{
    if ( maxStress && minStress )
        drawColorLine( baseRend );

    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::rounded_rect roundRect( colorLineGeometry.left(), 
                                 colorLineGeometry.top(),
                                 colorLineGeometry.right(),
                                 colorLineGeometry.bottom(), 5 );
    agg::conv_stroke<agg::rounded_rect> shading( roundRect );
    shading.width( 3.0 );
    ras.add_path( shading );
    solid_renderer solidRend( baseRend );
    solidRend.color( agg::rgba( 1, 1, 1 ) );
    agg::render_scanlines( ras, sl, solidRend );

    roundRect.rect( colorLineGeometry.left() - 1, 
                    colorLineGeometry.top() - 1,
                    colorLineGeometry.right() + 1,
                    colorLineGeometry.bottom() + 1 );
    agg::conv_stroke<agg::rounded_rect> outline( roundRect );
    shading.width( 1.0 );
    ras.add_path( outline );
    solidRend.color( agg::rgba8( 80, 80, 93 ) );
    agg::render_scanlines( ras, sl, solidRend );
}

/*****************************************************************************/
