
#include "ResultsTabWidget.h"
#include "TrussDeformPicture.h"
#include "TrussSolutionResults.h"
#include "TrussUnitCopy.h"

/*****************************************************************************
 * Truss Deform Picture
 *****************************************************************************/

TrussDeformPicture::TrussDeformPicture ( const ColorRangeControl& colCtrl,
                                         QWidget* parent /* = 0 */ ) :
    AggWidget( parent, Global::flipY ),
    trussCopy( 0 ),
    canvasBuf( new rbuf_dynarow( Global::defaultCanvasWidth, 
                                 Global::defaultCanvasHeight ) ),
    scaleMult( 0 ), 
    currentScaleMult( 0 ),
    nodesDeformCoeff( 0 ),
    numbersDrawing( true ),
    deformDrawing( true ),
    pluginResults( 0 ),
    colorCtrl( colCtrl )
{
    canvasBufRect.setCoords( Global::canvasHorIndent, Global::canvasVertIndent, 
                             Global::canvasHorIndent + canvasBuf->width(),
                             Global::canvasVertIndent + canvasBuf->height() );

    paintAreaRect.setCoords( canvasBufRect.left() + Global::areaBufferIndent,
                             canvasBufRect.top() + Global::areaBufferIndent,
                             canvasBufRect.right() - Global::areaBufferIndent,
                             canvasBufRect.bottom() - Global::areaBufferIndent );
    connect( &colorCtrl, SIGNAL(onLevelsNumberChange()),
                         SLOT(update()) );
    connect( this, SIGNAL(loadCaseSwitched(const LoadCaseResults&)),
             &colorCtrl, SLOT(updateStressRange(const LoadCaseResults&)) );
}

TrussDeformPicture::~TrussDeformPicture ()
{
    delete canvasBuf;
}

void TrussDeformPicture::fill ( TrussUnitCopy& copy, const PluginResults& res )
{
    if ( trussCopy != &copy ) {
        if ( trussCopy ) {
            disconnect( trussCopy, SIGNAL(displacementLoaded(bool)),
                             this, SIGNAL(displacementLoaded(bool)) );
            disconnect( trussCopy, SIGNAL(stateIsChanged()),
                             this, SLOT(update()) );
            disconnect( trussCopy, SIGNAL(trussDataLoaded()),
                             this, SLOT(updateScaleMultiplier()) );
        }
        trussCopy = &copy;
        connect( trussCopy, SIGNAL(displacementLoaded(bool)),
                            SIGNAL(displacementLoaded(bool)) );
        connect( trussCopy, SIGNAL(stateIsChanged()),
                            SLOT(update()) );
        connect( trussCopy, SIGNAL(trussDataLoaded()),
                            SLOT(updateScaleMultiplier()) );
        updateScaleMultiplier();
    }
    pluginResults = &res;
    update();
}

void TrussDeformPicture::setCanvasPosition ( const QPoint& pos )
{
    canvasBufRect.moveTo( pos );
    update();
}

void TrussDeformPicture::resizeCanvas ( const QSize &size )
{
    canvasBufRect.setSize( size );
    paintAreaRect.setWidth( canvasBufRect.width() - 
                            2 * Global::areaBufferIndent );
    paintAreaRect.setHeight( canvasBufRect.height() - 
                             2 * Global::areaBufferIndent );
    canvasBuf->init( (uint)canvasBufRect.width(), 
                     (uint)canvasBufRect.height() );
    updateScaleMultiplier();
    update();
}

void TrussDeformPicture::updateScaleMultiplier ()
{
    if ( ! trussCopy )
        return;

    DoubleSize trussAreaSize = trussCopy->getTrussAreaSize();
    if ( trussAreaSize.width() > trussAreaSize.height() )
        scaleMult = paintAreaRect.width() / trussAreaSize.width();
    else
        scaleMult = paintAreaRect.height() / trussAreaSize.height();

    currentScaleMult = scaleMult;
}

DoublePoint TrussDeformPicture::getTrussCoordFromWidgetPos ( 
                                                   double x, double y ) const
{
    double trussX = ( x - paintAreaRect.left() ) / currentScaleMult;
    double trussY( Global::flipY ? ( paintAreaRect.bottom() - y ) / 
                   currentScaleMult : ( y - paintAreaRect.bottom() ) / 
                   currentScaleMult );

    return DoublePoint( trussX, trussY );
}

DoublePoint TrussDeformPicture::getTrussCoordFromWidgetPos ( 
                                               const DoublePoint& pos ) const
{
    return getTrussCoordFromWidgetPos( pos.x(), pos.y() );
}

DoublePoint TrussDeformPicture::getWidgetPosFromTrussCoord ( 
                                               double x, double y ) const
{
    double widgetX = x * currentScaleMult + paintAreaRect.left();
    double widgetY = ( Global::flipY ? paintAreaRect.bottom() - 
                       y * currentScaleMult : y * currentScaleMult + 
                       paintAreaRect.top() );

    return DoublePoint( widgetX, widgetY );
}

DoublePoint TrussDeformPicture::getWidgetPosFromTrussCoord ( 
                                                const DoublePoint& coord ) const
{
    return getWidgetPosFromTrussCoord( coord.x(), coord.y() );
}

double TrussDeformPicture::getBaseScaleMultiplier () const
{
    return scaleMult;
}

void TrussDeformPicture::setCurrentScaleMultiplier ( double scaleNew )
{
    currentScaleMult = scaleNew;
    update();
}

void TrussDeformPicture::showElementNumbers ( bool status )
{
    if ( status != numbersDrawing ) {
        numbersDrawing = status;
        update();
    }
}

/*-------------------------- Handlers on events -----------------------------*/

void TrussDeformPicture::aggPaintEvent ( QPaintEvent* )
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    baseRend.clear ( agg::rgba( 1, 1, 1, 0 ) );
    paint( baseRend );
}

void TrussDeformPicture::aggResizeEvent ( QResizeEvent* )
{}

void TrussDeformPicture::aggKeyPressEvent ( QKeyEvent* )
{}

void TrussDeformPicture::aggMouseMoveEvent ( QMouseEvent* )
{}

void TrussDeformPicture::aggMouseReleaseEvent ( QMouseEvent* )
{}

void TrussDeformPicture::aggMousePressEvent ( QMouseEvent* )
{}

void TrussDeformPicture::aggCtrlChangedEvent ( const agg::ctrl* )
{}

void TrussDeformPicture::changeScale ( double sliderValue )
{
    setCurrentScaleMultiplier( scaleMult * sliderValue );
}

void TrussDeformPicture::changeDeform ( double sliderValue )
{
    nodesDeformCoeff = sliderValue;
    deformDrawing = true;
    trussCopy->displaceNodes( sliderValue );
}

void TrussDeformPicture::showNumbers ( bool checkBoxValue )
{
    showElementNumbers( checkBoxValue );
}

void TrussDeformPicture::showDeform ( bool status, double sliderValue )
{
    // save deform showing for switch load case purposes
    nodesDeformCoeff = sliderValue;
    deformDrawing = status;

    if ( ! status ) {
        trussCopy->restoreNodesBasePosition();
        return;
    }
    
    trussCopy->displaceNodes( sliderValue );
}

void TrussDeformPicture::switchLoadCaseResults ( int indx )
{
    if ( ! pluginResults )
        return;

    const LoadCaseResults* loadCaseRes = 
        pluginResults->getLoadCaseResults( ++indx );
    if ( ! loadCaseRes )
        return;

    trussCopy->loadResults( *loadCaseRes );

    if ( ! deformDrawing )
        trussCopy->restoreNodesBasePosition();
    else
        trussCopy->displaceNodes( nodesDeformCoeff );
    
    emit loadCaseSwitched( *loadCaseRes );
}

/*---------------------------- paint methods --------------------------------*/

void TrussDeformPicture::drawCoordDomain ( ren_dynarow& baseRend ) const
{
    glyph_gen glyph( 0 );
    glyph.font( agg::mcs11_prop_condensed );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    solidRenderer solidRend( baseRend ); 
    textRenderer textRend( baseRend, glyph );
    
    //---------- Draw origin point
    DoublePoint textPos = DoublePoint( paintAreaRect.bottomLeft() ) + 
                          DoublePoint( -12, 12 );
    drawText( textRend, "0", agg::rgba(100, 100, 100), textPos );

    //---------- Draw coordinate lines with arrow heads
    DoublePoint p1, p2;
    
    glyph.font( agg::verdana14 );
    p1.setX( paintAreaRect.left() );
    p1.setY( canvasBufRect.bottom() );
    p2.setX( paintAreaRect.left() );
    p2.setY( canvasBufRect.top() + 5 );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    drawText( textRend, "Y", agg::rgba(100, 100, 100), 
              p2 + DoublePoint( -12, 6 ) );

    p1.setX( canvasBufRect.left() );
    p1.setY( paintAreaRect.bottom() );
    p2.setX( canvasBufRect.right() - 5 );
    p2.setY( paintAreaRect.bottom() );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    drawText( textRend, "X", agg::rgba(100, 100, 100), 
              p2 + DoublePoint( -2, 14 ) );
}

void TrussDeformPicture::drawTrussAreaBoundaries ( ren_dynarow& baseRend ) const
{
    glyph_gen glyph( 0 );
    glyph.font ( agg::mcs11_prop_condensed );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    solidRenderer solidRend( baseRend ); 
    textRenderer textRend( baseRend, glyph );

    DoubleSize trussAreaSize = trussCopy->getTrussAreaSize();
    DoublePoint leftTop = 
        getWidgetPosFromTrussCoord( 0, trussAreaSize.height() );
    DoublePoint rightBottom = 
        getWidgetPosFromTrussCoord( trussAreaSize.width(), 0 );
    DoublePoint p1, p2;

    p1.setX( leftTop.x() - Global::scalePieceLength );
    p1.setY( leftTop.y() );
    p2.setX( rightBottom.x() );
    p2.setY( leftTop.y() );
    drawDashLine( baseRend, p1, p2, agg::rgba( 40, 40, 40 ), 1, 7, 5, true );
    p1 = rightBottom;
    p1.setY( p1.y() + Global::scalePieceLength );
    drawDashLine( baseRend, p1, p2, agg::rgba( 40, 40, 40 ), 1, 7, 5, true );

    //---------- Draw scale strokes and figure it
    double areaLenInPix = paintAreaRect.width();
    double areaWidInPix = paintAreaRect.height();

    int strokeNumbX = int( areaLenInPix / 36 );
    int strokeNumbY = int( areaWidInPix / 36 );

    double scaleFactorXInPix = areaLenInPix / strokeNumbX;
    double scaleFactorYInPix = areaWidInPix / strokeNumbY;

    double areaSizeXAfterScaling = trussAreaSize.width() / 
                                   fabs( rightBottom.x() - leftTop.x() ) * 
                                   areaLenInPix;
    double areaSizeYAfterScaling = trussAreaSize.height() / 
                                   fabs( rightBottom.y() - leftTop.y() ) * 
                                   areaWidInPix;

    double scaleFactorXInAbs = areaSizeXAfterScaling / strokeNumbX;
    double scaleFactorYInAbs = areaSizeYAfterScaling / strokeNumbY;

    // sign Y-axis
    // left point of the scale stroke
    DoublePoint strokePnt1( paintAreaRect.left() - Global::scalePieceLength,
                            paintAreaRect.top() + scaleFactorYInPix );
    // right point of the scale stroke
    DoublePoint strokePnt2( paintAreaRect.left(),strokePnt1.y() );
    DoublePoint textPos;
    QString str;
    for ( int i = 0; i < strokeNumbY; i++ )
    {
        strokePnt1.setY( paintAreaRect.top() + i * scaleFactorYInPix );
        strokePnt2.setY( strokePnt1.y() );
        drawLine ( ras, solidRend, sl, strokePnt1, strokePnt2 );
        str = QString("%1").arg( areaSizeYAfterScaling - 
                                 i * scaleFactorYInAbs,0,'f',2 );   
        double textLength = glyph.width( str.toAscii().data() ); 
        textPos = DoublePoint( strokePnt1.x() - textLength - 3, 
                               strokePnt1.y() + 3 );
        drawText ( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }

    // sign X-axis
    // top point of the scale stroke
    strokePnt1.setX( paintAreaRect.left() + scaleFactorXInPix );
    strokePnt1.setY( paintAreaRect.bottom() );
    // bottom point of the scale stroke
    strokePnt2.setX( strokePnt1.x() );
    strokePnt2.setY( paintAreaRect.bottom() + Global::scalePieceLength );

    textPos.setY( strokePnt2.y() + 10 );
    for ( int i = 0; i < strokeNumbX; i++ )
    {
        strokePnt1.setX( paintAreaRect.right() - i * scaleFactorXInPix );
        strokePnt2.setX( strokePnt1.x() );
        drawLine( ras, solidRend, sl, strokePnt1, strokePnt2 );
        textPos.setX( strokePnt1.x() - 12 );
        str = QString("%1").arg( areaSizeXAfterScaling - 
                                 i * scaleFactorXInAbs,0,'f',2 );   
        drawText( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }
}

void TrussDeformPicture::drawNodeNumber( const TrussCopyNode& node, 
                                         ren_dynarow& baseRend, 
                                         solidRenderer& solidRend, 
                                         scanline_rasterizer& ras, 
                                         agg::scanline_p8& sl ) const
{
    DoublePoint nodePos = getWidgetPosFromTrussCoord( node.getCurrentCoord() );
    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    DoublePoint textPos, backLeftTopPos, backRightBottomPos;

    backColor = agg::rgba( 1, 1, 1, 0.7 );
    numbFont = agg::gse5x9;
    textColor = agg::rgba( 0, 100, 0 );
    if ( node.getNumber() < 10 )
    {
        textPos.setX( nodePos.x() + 1 );
        textPos.setY( nodePos.y() - 5 );
        backLeftTopPos.setX( textPos.x() - 2 );
        backLeftTopPos.setY( textPos.y() );
        backRightBottomPos.setX( textPos.x() + 6 );
        backRightBottomPos.setY( textPos.y() - 9 );
    }
    else
    {
        textPos.setX( nodePos.x() - 1 );
        textPos.setY( nodePos.y() - 5 );
        backLeftTopPos.setX( textPos.x() - 1 );
        backLeftTopPos.setY( textPos.y() );
        backRightBottomPos.setX( textPos.x() + 11 );
        backRightBottomPos.setY( textPos.y() - 9 );
    }
    double rad = 3.0;
    agg::rounded_rect backRect( backLeftTopPos.x(), 
                                backLeftTopPos.y(), 
                                backRightBottomPos.x(), 
                                backRightBottomPos.y(), rad );
    ras.add_path( backRect );
    solidRend.color( backColor );
    agg::render_scanlines( ras, sl, solidRend );
    glyph.font( numbFont );
    textRenderer textRend( baseRend, glyph );
    QString str;
    str = QString("%1").arg( node.getNumber() );
    drawText( textRend, str, textColor, textPos );
}

void TrussDeformPicture::drawNodeFixation ( const TrussCopyNode& node,
                                            scanline_rasterizer& ras, 
                                            solidRenderer& solidRend, 
                                            agg::scanline_p8& sl,
                                            color_type color ) const
{
    DoublePoint nodePos = getWidgetPosFromTrussCoord( node.getCurrentCoord() );
    DoublePoint leftPnt, rightPnt;
    Node::Fixation fix = node.getFixation();
    double lineWidth = 2.0;

    if ( fix == Node::FixationByX )
    {
        leftPnt.setX ( nodePos.x() - 4 );
        leftPnt.setY ( nodePos.y() - 8 );
        rightPnt.setX ( nodePos.x() - 4 );
        rightPnt.setY ( nodePos.y() + 8 );
        drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX ( leftPnt.x() - 4 );
        leftPnt.setY ( leftPnt.y() - 4 );
        rightPnt.setX ( rightPnt.x() - 1 );
        rightPnt.setY ( leftPnt.y() + 3 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setY ( rightPnt.y() + 4 );
            leftPnt.setY ( leftPnt.y() + 4 );
            drawLine ( ras, solidRend, sl, leftPnt, rightPnt, 
                       lineWidth - 1, color );
        }
    }

    else if ( fix == Node::FixationByY )
    {
        leftPnt.setX( nodePos.x() - 8 );
        leftPnt.setY( nodePos.y() + 4 );
        rightPnt.setX( nodePos.x() + 9 );
        rightPnt.setY( nodePos.y() + 4 );
        drawLine( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX( nodePos.x() - 11 );
        leftPnt.setY( nodePos.y() + 8 );
        rightPnt.setX( nodePos.x() - 8 );
        rightPnt.setY( nodePos.y() + 5 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX( rightPnt.x() + 4 );
            leftPnt.setX( leftPnt.x() + 4 );
            drawLine( ras, solidRend, sl, leftPnt, 
                      rightPnt, lineWidth - 1, color );
        }
    }

    else if ( fix == Node::FixationByXY )
    {
        leftPnt.setX( nodePos.x() - 4 );
        leftPnt.setY( nodePos.y() + 9 );
        rightPnt.setX( nodePos.x() + 4 );
        rightPnt.setY( nodePos.y() + 9 );
        drawLine( ras, solidRend, sl, nodePos, leftPnt, lineWidth, color );
        drawLine( ras, solidRend, sl, nodePos, rightPnt, lineWidth, color );
    
        leftPnt.setX( leftPnt.x() - 4 );
        rightPnt.setX( rightPnt.x() + 5 );
        drawLine( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );

        rightPnt.setX( leftPnt.x() );
        rightPnt.setY( leftPnt.y() );
        leftPnt.setX( leftPnt.x() - 4 );
        leftPnt.setY( leftPnt.y() + 4 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX( rightPnt.x() + 4 );
            leftPnt.setX( leftPnt.x() + 4 );
            drawLine( ras, solidRend, sl, leftPnt, 
                      rightPnt, lineWidth - 1, color );
        }
    }
}

void TrussDeformPicture::drawNode ( const TrussCopyNode& node,
                                    ren_dynarow& baseRend ) const
{
    const DoublePoint& nodeCoord = node.getCurrentCoord();
    DoublePoint nodePos, originPos = paintAreaRect.bottomLeft();
    nodePos.setX( nodeCoord.x() * currentScaleMult + 
                  Global::areaBufferIndent );
    nodePos.setY( Global::flipY ? originPos.y() - 
                  nodeCoord.y() * currentScaleMult : 
                  nodeCoord.y() * currentScaleMult + 
                  Global::areaBufferIndent );  
    solidRenderer solidRend( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::ellipse ell;

    double highlightKoeff = 0.0;

    //  draw node' outline
    solidRend.color( agg::rgba( 10, 10, 10 ) );
    ell.init( nodePos.x(), nodePos.y(), 
              Global::nodesRadius + highlightKoeff + 1.0, 
              Global::nodesRadius + highlightKoeff + 1.0, 16 );
    ras.add_path( ell );
    agg::render_scanlines( ras, sl, solidRend );

    //  draw node
    agg::trans_affine mtx;
    mtx *= agg::trans_affine_scaling( 1 / 2.0 );
    mtx *= agg::trans_affine_translation( nodePos.x(), nodePos.y() );
    mtx.invert();
    color_array_type gradColors;
    radial_gradient gradFunc;
    
    if ( node.getFixation() == Node::Unfixed )
    {
        color_type begin( 255, 255, 255 ); 
        color_type middle( 230, 200, 195 ); 
        color_type end( uint(130 - highlightKoeff * 100), 
                        uint(90 - highlightKoeff * 60), 
                        uint(70 - highlightKoeff * 50) );
        fillColorArray( gradColors, begin, middle, end );
        drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, 
                             mtx, nodePos.x(), nodePos.y(), 
                             Global::nodesRadius + highlightKoeff, 0, 10 ); 
    }
    else 
    {
        drawNodeFixation( node, ras, solidRend, sl, agg::rgba(0, 0, 35) ); 
        color_type begin( 255, 255, 255 ); 
        color_type middle( 80, 100, 195 ); 
        color_type end( 0, 0, 80 );
        fillColorArray( gradColors, begin, middle, end );
        drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, 
                             mtx, nodePos.x(), nodePos.y(), 
                             Global::nodesRadius + highlightKoeff, 0, 10 ); 
    }
}

void TrussDeformPicture::drawPivotNumber( const TrussCopyPivot& pivot, 
                                          ren_dynarow& baseRend, 
                                          solidRenderer& solidRend, 
                                          scanline_rasterizer& ras, 
                                          agg::scanline_p8& sl ) const
{
    DoublePoint p1 = 
        getWidgetPosFromTrussCoord( pivot.getFirstNode().getCurrentCoord() );
    DoublePoint p2 = 
        getWidgetPosFromTrussCoord( pivot.getLastNode().getCurrentCoord() );

    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    DoublePoint textPos, backLeftTopPos, backRightBottomPos;

    backColor = agg::rgba( 1, 1, 1, 0.9 );
    numbFont = agg::gse5x9;
    textColor = agg::rgba( 30, 0, 0 );
    if ( pivot.getNumber() < 10 )
    {
        textPos.setX( ( p1.x() + p2.x() ) / 2 - 2 );
        textPos.setY( ( p1.y() + p2.y() ) / 2 + 4 );
        backLeftTopPos.setX( textPos.x() - 2 );
        backLeftTopPos.setY( textPos.y() );
        backRightBottomPos.setX( textPos.x() + 6 );
        backRightBottomPos.setY( textPos.y() - 9 );
    }
    else
    {
        textPos.setX( ( p1.x() + p2.x() ) / 2 - 4 );
        textPos.setY( ( p1.y() + p2.y() ) / 2 + 4 );
        backLeftTopPos.setX( textPos.x() - 1 );
        backLeftTopPos.setY( textPos.y() );
        backRightBottomPos.setX( textPos.x() + 11 );
        backRightBottomPos.setY( textPos.y() - 9 );
    }

    double rad = 3.0;
    agg::rounded_rect backRect( backLeftTopPos.x(), backLeftTopPos.y(), 
                                backRightBottomPos.x(), 
                                backRightBottomPos.y(), rad );
    ras.add_path( backRect );
    solidRend.color( backColor );
    agg::render_scanlines( ras, sl, solidRend );
    glyph.font( numbFont );
    textRenderer textRend( baseRend, glyph );
    QString str;
    str = QString("%1").arg( pivot.getNumber() );
    drawText( textRend, str, textColor, textPos );
}

void TrussDeformPicture::drawPivot ( const TrussCopyPivot& pivot, 
                                     ren_dynarow& baseRend, 
                                     color_type color ) const
{
    solidRenderer solidRend( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::ellipse ell;

    TrussCopyNode& first = pivot.getFirstNode(),
                   last = pivot.getLastNode();
    DoublePoint pos1, pos2, base1, base2,
                originPos = paintAreaRect.bottomLeft();
    const DoublePoint& coord1 = first.getCurrentCoord();
    const DoublePoint& coord2 = last.getCurrentCoord();
    const DoublePoint& baseCoord1 = first.getBaseCoord();
    const DoublePoint& baseCoord2 = last.getBaseCoord();

    pos1.setX( coord1.x() * currentScaleMult + Global::areaBufferIndent );
    pos1.setY( Global::flipY ? originPos.y() - coord1.y() * currentScaleMult :
               coord1.y() * currentScaleMult + Global::areaBufferIndent );
    pos2.setX( coord2.x() * currentScaleMult + Global::areaBufferIndent );
    pos2.setY( Global::flipY ? originPos.y() - coord2.y() * currentScaleMult :
               coord2.y() * currentScaleMult + Global::areaBufferIndent );

    base1.setX( baseCoord1.x() * currentScaleMult + Global::areaBufferIndent );
    base1.setY( Global::flipY ? originPos.y() - baseCoord1.y() * currentScaleMult :
                baseCoord1.y() * currentScaleMult + Global::areaBufferIndent );
    base2.setX( baseCoord2.x() * currentScaleMult + Global::areaBufferIndent );
    base2.setY( Global::flipY ? originPos.y() - baseCoord2.y() * currentScaleMult :
                baseCoord2.y() * currentScaleMult + Global::areaBufferIndent );

    drawLine( ras, solidRend, sl, base1, base2, 1.0, agg::rgba(1, 0, 0) );
    drawLine( ras, solidRend, sl, pos1, pos2, 3.0, color );
}

void TrussDeformPicture::drawTruss ( ren_dynarow& baseRend ) const
{
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    solidRenderer solidRend( baseRend ); 
    agg::ellipse ell;

    // draw truss elements
    TrussUnitCopy::TrussCopyPivotList pivots = trussCopy->getPivotList();
    TrussUnitCopy::TrussCopyPivotList::const_iterator  pivotIter = 
                                                            pivots.begin();
    for ( ; pivotIter != pivots.end(); ++pivotIter ) {
        TrussCopyPivot* pivot = *pivotIter;
        drawPivot( *pivot, baseRend, 
                   colorCtrl.getColorForStress( pivot->getStress() ) );
    }

    TrussUnitCopy::TrussCopyNodeList nodes = trussCopy->getNodeList();
    TrussUnitCopy::TrussCopyNodeList::const_iterator  nodeIter = 
                                                            nodes.begin();
    for ( ; nodeIter != nodes.end(); ++nodeIter )
        drawNode( **nodeIter, baseRend );


    // draw element numbers
    if ( numbersDrawing ) {
        for ( pivotIter = pivots.begin(); 
              pivotIter != pivots.end(); ++pivotIter ) 
            drawPivotNumber( **pivotIter, baseRend, solidRend, ras, sl );
        for ( nodeIter = nodes.begin(); 
              nodeIter != nodes.end(); ++nodeIter )
            drawNodeNumber( **nodeIter, baseRend, solidRend, ras, sl );
    }
}

void TrussDeformPicture::paint ( base_renderer& baseRenderer ) const
{
    if ( ! trussCopy )
        return;
    
    pixf_dynarow canvasPixf( *canvasBuf ); 

    ren_dynarow baseRend( canvasPixf );
    baseRend.clear( agg::rgba( 1, 1, 1, 0 ) );
    drawCoordDomain ( baseRend );
    drawTrussAreaBoundaries( baseRend );
    drawTruss( baseRend );

    //QPoint pos = canvasBufRect.topLeft().toPoint();
    //baseRenderer.blend_from( canvasPixf, 0, pos.x(), pos.y(), uint(1.0 * 255) );
    blendBuffer( baseRenderer, canvasPixf, canvasBufRect.topLeft().toPoint() );
}

/****************************************************************************/
