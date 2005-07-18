
#include "ToolBarManager.h"

#include <algorithm>
#include <qtoolbar.h>
#include <qaction.h>

/*****************************************************************************
 * Tool Bar Manager
 *****************************************************************************/

ToolBarManager::ToolBarManager () :
    toolBar(0),
    currentActiveWidget(0)
{}

ToolBarManager::ToolBarManager ( QToolBar* tb ) :
    toolBar(tb),
    currentActiveWidget(0)
{}

ToolBarManager::~ToolBarManager ()
{
    setToolBar(0);
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

void ToolBarManager::setToolBar ( QToolBar* tb )
{
    TabbedWidgetsIter iter = widgets.begin();
    for ( ; iter != widgets.end(); ++iter ) {
        TabbedWidget* tw = *iter;
        if ( toolBar )
            tw->action->removeFrom( toolBar );
        if ( tb )
            tw->action->addTo( toolBar );
    }    
    toolBar = tb;
    emit onToolBarChange(toolBar);
}

void ToolBarManager::activateWidget ( QWidget* w, bool activationFlag )
{
    uint activate = activationFlag ? WindowActive : ~WindowActive;
    TabbedWidget tw(w);
    TabbedWidgetsIter iter = std::find( widgets.begin(), widgets.end(), &tw );
    if ( iter != widgets.end() ) {
        TabbedWidget* tw = *iter;
        if ( currentActiveWidget ) {
            currentActiveWidget->windowState = 
                currentActiveWidget->widget->windowState();
        }        
        tw->widget->setWindowState(tw->windowState & activate);
        currentActiveWidget = tw;
    }        
}

void ToolBarManager::activateWidget ( QWidget* w )
{
    activateWidget( w, true );
}

void ToolBarManager::disactivateWidget ( QWidget* w )
{
    activateWidget( w, false );
}

void ToolBarManager::addWidget ( QWidget* w )
{
    TabbedWidget* tw = new TabbedWidget(w);
    widgets.push_back(tw);    
}

void ToolBarManager::removeWidget ( QWidget* w )
{
    TabbedWidget tw(w);
    TabbedWidgetsIter iter = std::find( widgets.begin(), widgets.end(), &tw );
    if ( iter == widgets.end() )
        return;
    if ( currentActiveWidget->widget == w )
        currentActiveWidget = 0;
    
    delete *iter;
}

/****************************************************************************/
