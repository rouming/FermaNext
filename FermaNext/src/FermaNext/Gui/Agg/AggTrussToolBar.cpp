
#include "AggTrussToolBar.h"
#include "TrussDesignerWidget.h"
#include "TrussMaterial.h"

/*****************************************************************************
 * Agg Material ComboBox
 *****************************************************************************/

AggMaterialComboBox::AggMaterialComboBox ( QWidget& parentWidget,
                                           TrussMaterialLibrary* mLib ) :
    AggComboBox( parentWidget ),
    materialLib( mLib )
{
    connect( this, SIGNAL(currentIndexChanged(int)),
                     SLOT(setCurrentMaterial(int)) );
    if ( mLib )
        setMaterialLibrary( *mLib );
}

const TrussMaterialLibrary& AggMaterialComboBox::getMaterialLibrary () const
{
    return *materialLib;
}

void AggMaterialComboBox::setMaterialLibrary ( TrussMaterialLibrary& mLib )
{
    if ( materialLib ) {
        disconnect( materialLib, 
                    SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                    this, SLOT(addMaterialItem(const TrussMaterial&)) );    
        disconnect( materialLib, 
                    SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                    this, SLOT(removeMaterialItem(const TrussMaterial&)) ); 
        disconnect( materialLib, 
                    SIGNAL(onSelectedMaterialChange(const TrussMaterial&)),
                    this, 
                    SLOT(updateCurrentMaterial(const TrussMaterial&)) );        
    }
  
    materialLib = &mLib;
    connect( materialLib, SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                          SLOT(addMaterialItem(const TrussMaterial&)) );    
    connect( materialLib, SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                          SLOT(removeMaterialItem(const TrussMaterial&)) );
    connect( materialLib, SIGNAL(onSelectedMaterialChange(const TrussMaterial&)),
                            SLOT(updateCurrentMaterial(const TrussMaterial&)) );
    setArgList();
}

const TrussMaterial* AggMaterialComboBox::getMaterialByIndex ( int idx ) const
{
    if ( idx < 0 || idx >= count() )
        return 0;
    
    QVariant material = itemData( idx );
    Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
    const TrussMaterial* m = qVariantValue<const TrussMaterial*>(material);
    if ( ! m )
        return 0;
    return m;
}

const TrussMaterial* AggMaterialComboBox::getCurrentMaterial () const
{
    return getMaterialByIndex( currentIndex() );
}

void AggMaterialComboBox::setCurrentMaterial ( int idx )
{
    if ( currentIndex() == idx )
        return;
    
    materialLib->selectMaterial( idx );
}

void AggMaterialComboBox::updateCurrentMaterial ( const TrussMaterial& m )
{
    int idx = materialLib->getIndexOfMaterial( m );
    if ( idx < 0 )
        return;

    if ( idx == currentIndex() )
        return;

    setCurrentIndex( idx );
}

int AggMaterialComboBox::getMaterialIndex ( const TrussMaterial& material ) const
{
    for ( int i = 0; i < count(); ++i ) {
        const TrussMaterial* m = getMaterialByIndex( i );
        if ( m == &material )
            return i;
    }
    return -1;
}

void AggMaterialComboBox::setArgList ()
{
    if ( ! materialLib )
        return;

    clear();

    for ( int i = 0; i < materialLib->countMaterials(); ++i ) {
        TrussMaterial* m = materialLib->getMaterial( i );
        if ( m )
            addMaterialItem( *m );
    }
}

void AggMaterialComboBox::addMaterialItem ( const TrussMaterial& m )
{
    const TrussMaterialLibrary* lib = 
        dynamic_cast<const TrussMaterialLibrary*>(sender());

    if ( lib && materialLib != lib )
        return;

    QVariant material;
    qVariantSetValue<const TrussMaterial*>( material, &m );
    addItem( m.getMaterialName(), material );

    connect( &m, SIGNAL(onAfterNameChange(const QString&)),
                 SLOT(updateMaterialItemName(const QString&)) );

}

void AggMaterialComboBox::removeMaterialItem ( const TrussMaterial& m )
{
    int idx = getMaterialIndex( m );
    if ( idx >= 0 )
        removeItem( idx );
}

void AggMaterialComboBox::updateMaterialItemName ( const QString& name )
{
    const TrussMaterial* senderMaterial = 
        dynamic_cast<const TrussMaterial*>( sender() );

    if ( ! senderMaterial )
        return;

    int idx = getMaterialIndex( *senderMaterial );
    if ( idx >= 0 )
        setItemText( idx, name );
}

void AggMaterialComboBox::clearMaterialLibrary ()
{
    materialLib = 0;
}

/*****************************************************************************
 * Agg Truss Tool Bar
 *****************************************************************************/

AggTrussToolBar::AggTrussToolBar ( TrussDesignerWidget* parentWidget ) :
    AggToolBar( parentWidget ),
    designerWidget( parentWidget ),
    aggComboBox( 0 )
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
    select->setStatusTip( tr( "Allows to select a single truss element" ) );
    addButton( *select );

    connect( select, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToSelect() ) );

    connect( designerWidget, SIGNAL( pressSelectButton() ), 
                     select, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* nodeDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/nodeIcon.svg" );
    nodeDraw->setHint( tr( "Draw node 'N'" ) );
    nodeDraw->setStatusTip( tr( "Draws a single node" ) );
    addButton( *nodeDraw );

    designerWidget->changeBehaviourToSelect();
    connect( nodeDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToNodeDraw() ) );

    connect( designerWidget, SIGNAL( pressNodeDrawButton() ), 
                     nodeDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* pivotDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/pivotIcon.svg" );
    pivotDraw->setHint( tr( "Draw pivot 'P'" ) );
    pivotDraw->setStatusTip( tr( "Draws a single pivot" ) );
    addButton( *pivotDraw );

    connect( pivotDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToPivotDraw() ) );

    connect( designerWidget, SIGNAL( pressPivotDrawButton() ), 
                     pivotDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* fixDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/fixIcon.svg" );
    fixDraw->setHint( tr( "Set node fixation 'F'" ) );
    fixDraw->setStatusTip( tr( "Adds a fixation" ) );
    addButton( *fixDraw );

    connect( fixDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToFixDraw() ) );

    connect( designerWidget, SIGNAL( pressFixDrawButton() ), 
                     fixDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* loadDraw = 
        new AggToolBarButton( Global::imagesSvgPath() + "/forceIcon.svg" );
    loadDraw->setHint( tr( "Load node 'L'" ) );
    loadDraw->setStatusTip( tr( "Loads a node" ) );
    addButton( *loadDraw );

    connect( loadDraw, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToLoadDraw() ) );

    connect( designerWidget, SIGNAL( pressLoadDrawButton() ), 
                     loadDraw, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    AggToolBarButton* erase = 
        new AggToolBarButton( Global::imagesSvgPath() + "/eraseIcon.svg" );
    erase->setHint( tr( "Erase 'R'" ) );
    erase->setStatusTip( tr( "Erase a single truss element" ) );
    addButton( *erase );

    connect( erase, SIGNAL( onButtonPress() ),
             designerWidget, SLOT( changeBehaviourToErase() ) );

    connect( designerWidget, SIGNAL( pressEraseButton() ), 
                      erase, SLOT( pressButton() ) );

    //-------------------------------------------------------------------------

    aggComboBox = new AggMaterialComboBox( *designerWidget );
    aggComboBox->setStatusTip( tr( "Selects a current pivot material" ) );
    addComboBox( *aggComboBox );
}

const TrussMaterial* AggTrussToolBar::getCurrentMaterial () const
{
    return aggComboBox->getCurrentMaterial();
}

void AggTrussToolBar::setMaterialLibrary ( TrussMaterialLibrary& mLib )
{
    aggComboBox->setMaterialLibrary( mLib );
}

