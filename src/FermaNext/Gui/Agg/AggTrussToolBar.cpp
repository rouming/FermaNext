
#include "AggTrussToolBar.h"
#include "TrussDesignerWidget.h"
#include "TrussMaterial.h"

/*****************************************************************************
 * Agg Material ComboBox
 *****************************************************************************/

AggMaterialComboBox::AggMaterialComboBox ( QWidget& parentWidget,
                                           const TrussMaterialLibrary* mLib ) :
    AggComboBox( parentWidget ),
    materialLib( mLib )
{
    if ( mLib )
        setMaterialLibrary( *mLib );
}

const TrussMaterialLibrary& AggMaterialComboBox::getMaterialLibrary () const
{
    return *materialLib;
}

void AggMaterialComboBox::setMaterialLibrary ( const TrussMaterialLibrary& mLib )
{
    if ( materialLib ) {
        disconnect( materialLib, 
                    SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                    this, SLOT(addMaterialItem(const TrussMaterial&)) );    
        disconnect( materialLib, 
                    SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                    this, SLOT(removeMaterialItem(const TrussMaterial&)) );  
    }
  
    materialLib = &mLib;
    connect( materialLib, SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                          SLOT(addMaterialItem(const TrussMaterial&)) );    
    connect( materialLib, SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                          SLOT(removeMaterialItem(const TrussMaterial&)) );
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

void AggMaterialComboBox::setCurrentMaterial ( const TrussMaterial& m )
{
    int idx = getMaterialIndex( m );
    if ( idx >= 0 )
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

    if ( sender() && materialLib != lib )
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

void AggMaterialComboBox::updateMaterialItemName( const QString& name )
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

    //-------------------------------------------------------------------------

    aggComboBox = new AggMaterialComboBox( *designerWidget );

    addComboBox( *aggComboBox );
}


void AggTrussToolBar::setMaterialLibrary ( const TrussMaterialLibrary& mLib )
{
    aggComboBox->setMaterialLibrary( mLib );
}

