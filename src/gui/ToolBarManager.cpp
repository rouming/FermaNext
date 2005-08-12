
#include "ToolBarManager.h"

#include <algorithm>
#include <qtoolbar.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qlabel.h>

/*****************************************************************************
 * Text Tool Button
 *****************************************************************************/

TextToolButton::TextToolButton ( const QIconSet & i, const QString & l, 
                                 const QString & gt, QObject* r, 
                                 const char* slot, QToolBar* p, 
                                 const char * n ) :
    QToolButton( i, l, gt, r, slot, p, n )
{}

QSize TextToolButton::sizeHint() const
{
    int w = 0;
    int h = 0;

    QString s( textLabel() );
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size( ShowPrefix, s );
    w = sz.width() + 5;
    h = sz.height() + sz.height()/8 + 10;
    w += h;	
    return QSize( w, h );
}

void TextToolButton::drawButtonLabel( QPainter *p )
{
    QRect r1 = rect();
    int x, y, w, h;
    r1.rect( &x, &y, &w, &h );
    
    int x1, y1, x2, y2;
    rect().coords( &x1, &y1, &x2, &y2 );

    QPixmap pm;
    QIconSet::Size size = usesBigPixmap() ? QIconSet::Large : QIconSet::Small;
    QIconSet::State state = isOn() ? QIconSet::On : QIconSet::Off;
    QIconSet::Mode mode;

    if ( ! isEnabled() ) {
        mode = QIconSet::Disabled;
    } else {
        mode = QIconSet::Normal;
    }

    pm = iconSet().pixmap( size, mode, state );

    x += 2;  y += 2;  w -= 4;  h -= 4;

    int w2 = pm.width() + 8;
        
    p->setFont( font() );
    QStyle::SFlags flags = QStyle::Style_Default;
    QColor btext = colorGroup().buttonText();

    QRect pr( x, y, w2, h );
    QRect tr( x+pm.width() - 9, y, w, h);
    
    style().drawItem( p, pr, AlignCenter|ShowPrefix, colorGroup(), 
                      TRUE, &pm, QString::null );
    style().drawItem( p, tr, AlignCenter|ShowPrefix, colorGroup(), flags, 0, 
                      textLabel(), textLabel().length(), &btext );
}

/*****************************************************************************
 * Tabbed Widget
 *****************************************************************************/

TabbedWidget::TabbedWidget ( ToolBarManager* tb,
                             QWidget& w,
                             const QString& n,
                             const QIconSet& i ) : 
    toolBar(tb),
    name(n), widget(w),
    button( new TextToolButton(i, n, n, 0, 0, tb) ),
    windowState(0),
    toolBarIsDestroyed(false)
{
    // Direct connect
    QObject::connect( button, SIGNAL(clicked()), SLOT(showOrHide()) );
    // Catch tool bar decease
    QObject::connect( tb, SIGNAL(destroyed()), SLOT(toolBarDestroyed()) );
    // Small font
    QFont f = button->font();
    f.setPointSize( 10 );
    button->setFont(f);
}

TabbedWidget::~TabbedWidget ()
{
    if ( ! toolBarIsDestroyed )
        delete button;
}

void TabbedWidget::toolBarDestroyed ()
{
    toolBarIsDestroyed = true;
}

void TabbedWidget::activate ( bool flag )
{
    if ( flag )
        activate();
    else
        disactivate();
}

void TabbedWidget::activate ()
{
    if ( isActive() )
        return;
    button->setEnabled( true );
    emit onActivate( *this );
}

void TabbedWidget::disactivate ()
{
    if ( ! isActive() )
        return;
    button->setEnabled( false );
    widget.hide();
    emit onDisactivate( *this );
}

void TabbedWidget::showOrHide ()
{
    // Always is shown for now
    show();
}

void TabbedWidget::show ()
{
    if ( ! isActive() )
        return;
    widget.raise();
    emit onShow( *this );
}

void TabbedWidget::hide ()
{
    if ( ! isActive() )
        return;
    widget.hide();
    emit onHide( *this );
}

void TabbedWidget::saveWidgetState ()
{
    windowState = widget.windowState();
}

void TabbedWidget::loadWidgetState ()
{
    widget.setWindowState( windowState );
}

bool TabbedWidget::isActive () const
{
    return button->isEnabled();
}

bool TabbedWidget::isShown () const
{
    return widget.isShown();
}

QWidget& TabbedWidget::getWidget ()
{
    return widget;
}

/*****************************************************************************
 * Tool Bar Manager
 *****************************************************************************/

ToolBarManager::ToolBarManager ( QMainWindow* parent, const char* name ) :
    QToolBar(parent, name),
    blankLabel(0)
{}

ToolBarManager::~ToolBarManager ()
{
    clear();
}

void ToolBarManager::clear ()
{
    TabbedWidgetListIter iter = widgets.begin();
    for ( ; iter != widgets.end(); ++iter )
        delete *iter;
    widgets.clear();
}

TabbedWidget* ToolBarManager::findByWidget ( QWidget& w ) const
{
    TabbedWidgetListConstIter iter = widgets.begin();
    for ( ; iter != widgets.end(); ++iter )
        if ( &(*iter)->getWidget() == &w )
            return *iter;
    return 0;
}

void ToolBarManager::activateWidget ( QWidget& w, bool activationFlag )
{
    TabbedWidget* tw = findByWidget(w);
    if ( ! tw )
        return;
    tw->activate( activationFlag );
}

void ToolBarManager::activateWidget ( QWidget& w )
{
    activateWidget( w, true );
}

void ToolBarManager::disactivateWidget ( QWidget& w )
{
    activateWidget( w, false );
}

TabbedWidget& ToolBarManager::createTabbedWidget ( QWidget& w, 
                                                   const QString& name, 
                                                   const QIconSet& iconSet )
{    
    TabbedWidget* tb = new TabbedWidget( this, w, name, iconSet );    
    if ( blankLabel )
        delete blankLabel;
    blankLabel = new QLabel(this);
    setStretchableWidget( blankLabel );
    widgets.push_back( tb );
    return *tb;
}

void ToolBarManager::removeWidget ( QWidget& w )
{
    TabbedWidget* tw = findByWidget(w);
    if ( tw == 0 )
        return;
    TabbedWidgetListIter iter = std::find( widgets.begin(), widgets.end(), tw);
    delete *iter;
    widgets.erase(iter);
}

/****************************************************************************/
