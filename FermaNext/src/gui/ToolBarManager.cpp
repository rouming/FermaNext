
#include "ToolBarManager.h"

#include <algorithm>
#include <qtoolbar.h>
#include <qtoolbutton.h>

/*****************************************************************************
 * Tabbed Widget
 *****************************************************************************/

TabbedWidget::TabbedWidget ( ToolBarManager& tb,
                                             QWidget& w, 
                                             const QString& n,
                                             const QIconSet & i ) : 
    name(n), widget(w),
    button( new QToolButton(i, n, n, this, SLOT(focus()),  &tb) ), 
    windowState(0)
{}

void TabbedWidget::activate ()
{
}

void TabbedWidget::disactivate ()
{
}

void TabbedWidget::focus ()
{
}

void TabbedWidget::defocus ()
{
}

bool TabbedWidget::isActive () const
{
    return true;
}

bool TabbedWidget::isFocused () const
{
    return true;
}

/*****************************************************************************
 * Tool Bar Manager
 *****************************************************************************/

ToolBarManager::ToolBarManager ( QMainWindow* parent, const char* name ) :
    QToolBar(parent, name),
    currentActiveWidget(0)
{}

ToolBarManager::~ToolBarManager ()
{
    clear();
}

void ToolBarManager::clear ()
{
    TabbedWidgetsIter iter = widgets.begin();
    for ( ; iter != widgets.end(); ++iter ) {
        delete *iter;
        widgets.erase(iter);
    }
    widgets.clear();
}

TabbedWidget* ToolBarManager::findByWidget ( QWidget* ) const
{/*
    TabbedWidgetsConstIter iter = widgets.begin();
    for ( ; iter != widgets.end(); ++iter )
        if ( (*iter)->widget == w )
            return *iter;
 */
    return 0;
}

void ToolBarManager::activateWidget ( QWidget*, bool )
{/*
    uint activate = activationFlag ? WindowActive : ~WindowActive;
    TabbedWidget* tw = findByWidget(w);
    if ( tw ) {
        if ( currentActiveWidget ) {
            currentActiveWidget->windowState = 
                currentActiveWidget->widget->windowState();
        }        
        tw->widget->setWindowState( tw->windowState & activate );
        currentActiveWidget = tw;
        }*/
}

void ToolBarManager::activateWidget ( QWidget* w )
{
    activateWidget( w, true );
}

void ToolBarManager::disactivateWidget ( QWidget* w )
{
    activateWidget( w, false );
}

void ToolBarManager::addWidget ( QWidget*  )
{/*
    TabbedWidget* tw = new TabbedWidget(w);
    widgets.push_back(tw);    */
}

void ToolBarManager::removeWidget ( QWidget* )
{/*
    TabbedWidget* tw = findByWidget(w);
    if ( tw == 0 )
        return;
    if ( currentActiveWidget->widget == w )
        currentActiveWidget = 0;

    TabbedWidgetsIter iter = std::find( widgets.begin(), widgets.end(), tw );
    delete *iter;
    widgets.erase(iter);*/
}

/****************************************************************************/
