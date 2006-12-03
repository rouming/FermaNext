
#include "AggTrussToolBar.h"
#include "TrussDesignerWidget.h"

/*****************************************************************************
 * Agg Truss Tool Bar
 *****************************************************************************/

AggTrussToolBar::AggTrussToolBar ( TrussDesignerWidget* parentWidget ) :
    AggToolBar( parentWidget ),
    designerWidget( parentWidget )
{
    initButtons(); 
}

AggTrussToolBar::~AggTrussToolBar ()
{}

void AggTrussToolBar::initButtons ()
{
    if ( ! designerWidget )
        return;

    //-------------------------------------------------------------------------

    AggToolBarButton* select = 
        new AggToolBarButton( Global::imagesSvgPath() + "/arrowIcon.svg" );
    select->setHint( "Select 'Esc'" );
    addButton( *select );

    connect( select, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToSelect() ) );

    connect( designerWidget, SIGNAL( pressSelectButton() ), 
                     select, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* nodeDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/nodeIcon.svg" );
    nodeDraw->setHint( "Draw node 'N'" );
    addButton( *nodeDraw );

    designerWidget->changeBehaviourToSelect();
    connect( nodeDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToNodeDraw() ) );

    connect( designerWidget, SIGNAL( pressNodeDrawButton() ), 
                     nodeDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* pivotDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/pivotIcon.svg" );
    pivotDraw->setHint( "Draw pivot 'P'" );
    addButton( *pivotDraw );

    connect( pivotDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToPivotDraw() ) );

    connect( designerWidget, SIGNAL( pressPivotDrawButton() ), 
                     pivotDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* fixDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/fixIcon.svg" );
    fixDraw->setHint( "Set node fixation 'F'" );
    addButton( *fixDraw );

    connect( fixDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToFixDraw() ) );

    connect( designerWidget, SIGNAL( pressFixDrawButton() ), 
                     fixDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* loadDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/forceIcon.svg" );
    loadDraw->setHint( "Load node 'L'" );
    addButton( *loadDraw );

    connect( loadDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToLoadDraw() ) );

    connect( designerWidget, SIGNAL( pressLoadDrawButton() ), 
                     loadDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* erase = 
        new AggToolBarButton( Global::imagesSvgPath() + "/eraseIcon.svg" );
    erase->setHint( "Erase 'R'" );
    addButton( *erase );

    connect( erase, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToErase() ) );

    connect( designerWidget, SIGNAL( pressEraseButton() ), 
                      erase, SLOT( pressButton() ) );
}

/*****************************************************************************/

