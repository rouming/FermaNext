
#include <QMouseEvent>

#include "AggPopupMenu.h"
#include "Geometry.h"

/*****************************************************************************
 * Agg Abstract Menu Action
 *****************************************************************************/

AggMenuAction::AggMenuAction ( const QString& text ) :
    actionText( text ),
    highlighted( false )
{}

AggMenuAction::~AggMenuAction ()
{}

void AggMenuAction::highlight ( bool status )
{
    if ( highlighted == status )
        return;
    
    highlighted = status;
    emit stateChanged();
}

bool AggMenuAction::isHighlighted () const
{
    return highlighted;
}

void AggMenuAction::trigger ()
{
    emit triggered();
}

void AggMenuAction::setText ( const QString& text )
{
    if ( actionText == text )
        return;
    
    actionText = text;
    emit stateChanged();
}

const QString& AggMenuAction::text () const
{
    return actionText;
}

/*****************************************************************************
 * Agg Abstract Menu
 *****************************************************************************/


AggPopupMenu::AggPopupMenu () :
    leftTopPos( QPoint( 0, 0 ) ),
    actionFont( agg::verdana14_bold ),
    separatorHeight( 1 ), 
    topIndent( 5 ), 
    bottomIndent( 5 ), 
    leftIndent( 5 ), 
    rightIndent( 5 ),
    selectionWidth( 0 ),
    selectionHeight( 0 ),
    selectionHorIndent( 3 ),
    selectionVerIndent( 3 ),
    widthLimit( 100 ),
    rendered( false ),
    visible( false ),
    menuBuf( new rbuf_dynarow( 0, 0 ) )
{
    glyph_gen glyph( actionFont );
    selectionHeight = (int)glyph.height() + selectionVerIndent * 2; 
}

AggPopupMenu::~AggPopupMenu ()
{
    delete menuBuf;
    clear();
}

int AggPopupMenu::width () const
{
    return menuBuf->width();
}

int AggPopupMenu::height () const
{
    return menuBuf->height();
}

textFont AggPopupMenu::getActionFont () const
{
    return actionFont;
}

QPoint AggPopupMenu::getPosition () const
{
    return leftTopPos;
}

void AggPopupMenu::setPosition ( QPoint leftTop )
{
    leftTopPos = leftTop;
    clearRenderedFlag();
}

void AggPopupMenu::resize ( int w, int h )
{
    menuBuf->init( w, h );
    clearRenderedFlag();
}

void AggPopupMenu::updateGeometry ()
{
    int menuHeight = topIndent + bottomIndent;
    int menuWidth = leftIndent + rightIndent;

    glyph_gen glyph( actionFont );

    foreach ( AggMenuAction* action, actionList ) {
        int textLenght = (int)glyph.width( action->text().toAscii().data() );
        if ( textLenght > selectionWidth )
            selectionWidth = textLenght;

        menuHeight = menuHeight + separatorHeight + selectionHeight;
    }
    selectionWidth = selectionWidth + selectionHorIndent * 2;
    menuWidth = menuWidth + selectionWidth;

    if ( menuWidth > widthLimit ) menuWidth = widthLimit;

    resize( menuWidth, menuHeight );
}

void AggPopupMenu::addAction ( const QString& actionText )
{
    AggMenuAction* action = new AggMenuAction( actionText );
    actionList.append( action );
    connect( action, SIGNAL( stateChanged() ),
                       SLOT( clearRenderedFlag() ) );
    connect( action, SIGNAL( triggered() ),
                       SLOT( onActionTriggered() ) );
    updateGeometry();
}

void AggPopupMenu::removeAction ( AggMenuAction* action )
{
    try {
        ActionList::Iterator iter = qFind( actionList.begin(),
                                           actionList.end(),
                                           action );
        if ( iter == actionList.end() )
            return;

        actionList.erase( iter );

        delete action;
    }
    catch ( ... ) {
        return;
    }
}

void AggPopupMenu::removeAction ( int idx )
{
    if ( idx > actionList.size() )
        return;

    try {
        removeAction( actionList.at( idx ) );
    }
    catch ( ... ) {
        return;
    }
}

AggMenuAction* AggPopupMenu::getAction ( int idx ) const
{
    try {
        return actionList.at( idx );
    }
    catch ( ... ) {
        return 0;
    }
}

void AggPopupMenu::onActionTriggered ()
{
    AggMenuAction* action = dynamic_cast<AggMenuAction*>( sender() );
    if ( ! action )
        return;

    int i = 0;
    foreach ( AggMenuAction* a, actionList ) {
        if ( action == a ) {
            emit actionTriggered( i );
            break;
        }
        ++i;
    }
}

void AggPopupMenu::clear ()
{
    foreach ( AggMenuAction* action, actionList )
        delete action;
    actionList.clear();
}

QPoint AggPopupMenu::getSelectionLeftTop ( const AggMenuAction& action ) const
{
    QPoint selectionLeftTopPos( leftIndent, rightIndent );
    foreach ( AggMenuAction* a, actionList ) {
        if ( a == &action )
            return selectionLeftTopPos;

        selectionLeftTopPos.setY( selectionLeftTopPos.y() + separatorHeight +
                                  selectionHeight );
    }

    return QPoint( -1, -1 );
}

bool AggPopupMenu::inActionRect ( int x, int y, 
                                  const AggMenuAction& action ) const
{
    QPoint leftTop = getSelectionLeftTop( action );

    // uninitialized action
    if ( leftTop.x() == -1 || leftTop.y() == -1 )
        return false;

    if ( x > leftTop.x() && x < leftTop.x() + selectionWidth &&
         y > leftTop.y() && y < leftTop.y() + selectionHeight )
        return true;
    
    return false;    
}

bool AggPopupMenu::inMenuRect ( int x, int y ) const
{
    if ( x > leftTopPos.x() && x < leftTopPos.x() + width() &&
         y > leftTopPos.y() && y < leftTopPos.y() + height() )
        return true;
    
    return false;    
}

QPoint AggPopupMenu::fromGlobalPos ( int x, int y ) const
{
    return QPoint( x - leftTopPos.x(), y - leftTopPos.y() );
}

void AggPopupMenu::mouseMove ( int xGlobal, int yGlobal )
{
    QPoint localPos = fromGlobalPos( xGlobal, yGlobal );
    foreach ( AggMenuAction* action, actionList )
        action->highlight( inActionRect( localPos.x(), localPos.y(), *action ) );
}

void AggPopupMenu::mousePress ( int xGlobal, int yGlobal )
{
    if ( ! inMenuRect( xGlobal, yGlobal ) )
        setVisible( false );
}

void AggPopupMenu::mouseRelease ( int xGlobal, int yGlobal )
{
    QPoint localPos = fromGlobalPos( xGlobal, yGlobal );
    foreach ( AggMenuAction* action, actionList )
        if ( inActionRect( localPos.x(), localPos.y(), *action ) ) {
            action->trigger();
            setVisible( false );
            break;
        }
}

void AggPopupMenu::clearRenderedFlag () 
{
    rendered = false;
    emit renderedFlagCleared();
}

bool AggPopupMenu::isRendered () const
{
    return rendered;
}

void AggPopupMenu::setVisible ( bool status )
{
    if ( visible == status )
        return;
    
    visible = status;

    clearRenderedFlag();
}

bool AggPopupMenu::isVisible () const
{
    return visible;
}

void AggPopupMenu::show ()
{
    setVisible( true );
}

void AggPopupMenu::hide ()
{
    setVisible( false );
}

void AggPopupMenu::drawAction ( ren_dynarow& baseRend,
                                const AggMenuAction& action ) const
{
    primitivesRenderer primRend( baseRend );

    QPoint selectionLeftTop = getSelectionLeftTop( action );

    if ( action.isHighlighted() ) {
        primRend.fill_color( agg::rgba( 1, 1, 1 ) );
        primRend.line_color( agg::rgba( 10, 10, 10 ) );

        QPoint selectionRightBottom( selectionLeftTop.x() + selectionWidth,
                                     selectionLeftTop.y() + selectionHeight );

        //drawOutlineRoundedRect( )
        primRend.outlined_rectangle( selectionLeftTop.x(), 
                                     selectionLeftTop.y(), 
                                     selectionRightBottom.x(), 
                                     selectionRightBottom.y() );
    }

    glyph_gen glyph( actionFont );
    textRenderer textRend( baseRend, glyph );

    DoublePoint textPos( selectionLeftTop.x() + selectionHorIndent,
                         selectionLeftTop.y() + selectionHeight - 
                                                    selectionVerIndent );

    int pixLimit = width() - leftIndent - rightIndent;
    QString actText = fitTextToWidth( action.text(), pixLimit, glyph );

    drawText( textRend, actText.toAscii().data(), 
              agg::rgba( 0, 0, 0 ), textPos );

}

void AggPopupMenu::paint ( base_renderer& baseRenderer ) const
{
    pixf_dynarow pixf( *menuBuf );

    if ( ! isRendered() ) {
        ren_dynarow baseRend( pixf );
        baseRend.clear( agg::rgba8( 150, 150, 150, 0 ) );

        solidRenderer solidRend( baseRend );
        scanline_rasterizer ras;
        agg::scanline_p8 sl;
        agg::trans_affine mtx;
    
        color_type firstColor( agg::rgba( 1, 1, 1, 0.5 ) );
        color_type middleColor( agg::rgba8( 180, 130, 100, 0.5 ) );
        color_type lastColor( agg::rgba( 1, 1, 1, 0.5 ) );

        color_array_type gradColors;

        fillColorArray( gradColors, firstColor, middleColor, lastColor );

        double lineWidth = 1.5;
        double rad = 10;
        DoublePoint p1( lineWidth, lineWidth ),
                    p2( width() - lineWidth, height() - lineWidth );

        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors,
                                mtx, p1, p2, agg::rgba( 50, 50, 50, 0.6 ), rad, 
                                lineWidth, 0, height() );

        foreach ( AggMenuAction* action, actionList )
            drawAction( baseRend, *action );

        rendered = true;
    }

    baseRenderer.blend_from( pixf, 0, leftTopPos.x(), leftTopPos.y(), 
                             unsigned( 1.0 * 255 ) );
}



