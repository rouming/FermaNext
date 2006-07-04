
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFont>
#include <QFrame>
#include <QGroupBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QRect>
#include <QStringList>
#include <QStyle>
#include <QStringList>

#include "GuiSubsidiary.h"
#include "TrussPropertyTabWidget.h"
#include "TrussUnitActions.h"

/*****************************************************************************
 * Load Table Delegate
 *****************************************************************************/

LoadTableDelegate::LoadTableDelegate ( QObject* parent ) :
    QItemDelegate( parent )
{}

QWidget* LoadTableDelegate::createEditor ( QWidget *parent,
                                           const QStyleOptionViewItem&,
                                           const QModelIndex& ) const
{
    QLineEdit* editor = new QLineEdit( parent );
    editor->setFrame( false );
    RangeValidator* validator = new RangeValidator( editor );
    validator->setDecimals( 2 );
    editor->setValidator( validator );
    editor->setMaxLength( 10 );
    return editor;
}

void LoadTableDelegate::setEditorData ( QWidget* editor, 
                                        const QModelIndex& index ) const
{
    QString load = 
        index.model()->data( index, Qt::DisplayRole ).toString();
    QLineEdit* loadEditor = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( loadEditor );
    loadEditor->setText( load );
    loadEditor->setAlignment( Qt::AlignLeft );
    return;
}

void LoadTableDelegate::setModelData ( QWidget* editor, 
                                       QAbstractItemModel* model, 
                                       const QModelIndex& index ) const
{
    QLineEdit* loadEditor = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( loadEditor );
    QVariant newLoadVariant( loadEditor->text() );
    model->setData( index, newLoadVariant );
    emit const_cast<LoadTableDelegate*>(this)->
            cellWasChanged( index.row(), index.column() );
    return;
}

void LoadTableDelegate::updateEditorGeometry ( 
    QWidget* editor,
    const QStyleOptionViewItem& option, 
    const QModelIndex& ) const
{
    editor->setGeometry( option.rect );
}

/*****************************************************************************
 * Load Table
 *****************************************************************************/

LoadTable::LoadTable ( QWidget* parent ) :
    QTableWidget( parent )
{}

void LoadTable::setLoad ( int row, const TrussLoad& load )
{
    if ( row >= rowCount() )
        return;
    
    QTableWidgetItem* cellX = item( row, 0 );
    if ( cellX )
        cellX->setText( QString("%1").arg( load.getXForce(),0,'f',2 ) );
    else {
        QTableWidgetItem* cellX = new QTableWidgetItem( 
                        QString("%1").arg( load.getXForce(),0,'f',2 ) );
        cellX->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        setItem( row, 0, cellX );
    }

    QTableWidgetItem* cellY = item( row, 1 );
    if ( cellY )
        cellY->setText( QString("%1").arg( load.getYForce(),0,'f',2 ) );
    else {
        QTableWidgetItem* cellY = new QTableWidgetItem( 
                        QString("%1").arg( load.getYForce(),0,'f',2 ) );
        cellY->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        setItem( row, 1, cellY );
    }
    
    horizontalHeader()->resizeSection( 0, Global::loadColumnWidth );
    horizontalHeader()->resizeSection( 1, Global::loadColumnWidth );
}

double LoadTable::getLoadXForce ( int row ) const
{
    QTableWidgetItem* cellX = item( row, 0 );
    Q_ASSERT( cellX != 0 );
    return cellX->text().toDouble();
}

double LoadTable::getLoadYForce ( int row ) const
{
    QTableWidgetItem* cellY = item( row, 0 );
    Q_ASSERT( cellY != 0 );
    return cellY->text().toDouble();
}

int LoadTable::getLoadedNodesNumber () const
{
    int loadedNumb = 0;
    QTableWidgetItem *cellX = 0, *cellY = 0;
    for ( int i = 0; i < rowCount(); ++i ) {
        cellX = item( i, 0 );
        cellY = item( i, 1 );
        if ( cellX && cellX->text().toDouble() != 0 ||
             cellY && cellY->text().toDouble() != 0 )
            ++loadedNumb;
    }
    return loadedNumb;
}

/*****************************************************************************
 * Material Table Item
 *****************************************************************************/


MaterialTableItem::MaterialTableItem ( const TrussMaterial* m /* = 0 */, 
                                       int type /* = Type */ ) :
    QTableWidgetItem( type )
{
    setMaterial( m );
}

void MaterialTableItem::setMaterial ( const TrussMaterial* m )
{
    QVariant material;
    qVariantSetValue<const TrussMaterial*>( material, m );
    setData( Qt::EditRole, material );

    if ( m )
        connect( m, SIGNAL(onAfterNameChange(const QString&)),
                    SLOT(updateMaterialItemName(const QString&)) );
}

const TrussMaterial* MaterialTableItem::getMaterial () const
{
    QVariant material = data( Qt::DisplayRole );
    Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
    const TrussMaterial* m = qVariantValue<const TrussMaterial*>(material);
    return m;
}

void MaterialTableItem::updateMaterialItemName( const QString& )
{
    const TrussMaterial* senderMaterial = 
        dynamic_cast<const TrussMaterial*>(sender());
    const TrussMaterial* m = getMaterial();
    if ( m == senderMaterial )
        setMaterial( senderMaterial );
}

/*****************************************************************************
 * Pivot Property Table Delegate
 *****************************************************************************/

PivotPropertyTableDelegate::PivotPropertyTableDelegate ( QWidget* parent ) : 
    QItemDelegate( parent ),
    materialLib( 0 )
{}

QWidget* PivotPropertyTableDelegate::createEditor ( 
    QWidget *parent, 
    const QStyleOptionViewItem& ,
    const QModelIndex& index ) const
{
    
    if ( index.column() != 0 ) {
        // create editor for material cells
        MaterialComboBox *editor = new MaterialComboBox( parent, materialLib );
        return editor;
    }

    // create editor for thickness cell
    QLineEdit* editor = new QLineEdit( parent );
    editor->setFrame( false );
    RangeValidator* validator = new RangeValidator( editor );
    validator->setRange( 0.0, Global::pivotThickLimit );
    validator->setDecimals( 2 );
    editor->setValidator( validator );
    editor->setMaxLength( 6 );
    return editor;
}

void PivotPropertyTableDelegate::setEditorData ( 
    QWidget *editor,
    const QModelIndex &index ) const
{
    // fill editor of the material cell
    if ( index.column() != 0 ) {
        MaterialComboBox *comboBox = 
            dynamic_cast<MaterialComboBox*>(editor);
        Q_ASSERT( comboBox != 0 );
        QVariant material = index.model()->data( index, Qt::DisplayRole );
        Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
        const TrussMaterial* currentMaterial = 
                  qVariantValue<const TrussMaterial*>(material);
        if ( currentMaterial )
            comboBox->setCurrentMaterial( *currentMaterial );
        return;
    }

    // fill editor of the thickness cell
    QString thickness = 
        index.model()->data( index, Qt::DisplayRole ).toString();
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( editor != 0 );
    lineEdit->setText( thickness );
    lineEdit->setAlignment( Qt::AlignLeft );
    return;
}

void PivotPropertyTableDelegate::setModelData ( 
    QWidget* editor, 
    QAbstractItemModel* model, 
    const QModelIndex& index ) const
{
    // set data for material cell
    if ( index.column() != 0 ) {
        MaterialComboBox *comboBox = dynamic_cast<MaterialComboBox*>(editor);
        const TrussMaterial* m = comboBox->getCurrentMaterial();
        if ( ! m )
            return;
        QVariant material;
        qVariantSetValue<const TrussMaterial*>( material, m );
        model->setData( index, material );
        emit const_cast<PivotPropertyTableDelegate*>(this)->
                cellWasChanged( index.row(), index.column() );
        return;
    }

    // set data for thickness cell
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( lineEdit );
    QVariant newValue( lineEdit->text() );
    model->setData( index, newValue );
    emit const_cast<PivotPropertyTableDelegate*>(this)->
            cellWasChanged( index.row(), index.column() );

    return;
}

void PivotPropertyTableDelegate::paint ( QPainter* painter, 
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index ) const
{
    if ( index.column() != 1 ) {
        QStyleOptionViewItem opt = option;
        QItemDelegate::paint( painter, opt, index );
    } 
    else 
    {
        QPalette::ColorGroup cg = option.state& QStyle::State_Enabled ? 
                                  QPalette::Normal : QPalette::Disabled;
        if ( option.state & QStyle::State_Selected )
            painter->fillRect( option.rect, 
                               option.palette.color( cg, 
                                                     QPalette::Highlight ) );
        QVariant material = index.model()->data( index, Qt::DisplayRole );
        Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
        const TrussMaterial* currentMaterial = 
                  qVariantValue<const TrussMaterial*>(material);
        if ( currentMaterial )
            drawDisplay( painter, option, option.rect, 
                         currentMaterial->getMaterialName() );
        // since we draw the grid ourselves
        drawFocus( painter, option, option.rect ); 
    }
}

void PivotPropertyTableDelegate::updateEditorGeometry ( 
    QWidget* editor,
    const QStyleOptionViewItem& option, 
    const QModelIndex& ) const
{
    editor->setGeometry( option.rect );
}

void PivotPropertyTableDelegate::setMaterialLibrary ( 
    const TrussMaterialLibrary& matLib )
{
    materialLib = &matLib;
}

/*****************************************************************************
 * Pivot Table
 *****************************************************************************/

PivotPropertyTable::PivotPropertyTable ( QWidget* parent ) :
    QTableWidget( parent )
{}

MaterialTableItem* PivotPropertyTable::getMaterialCell ( int row ) const
{
    QTableWidgetItem* cell = item( row, 1 );
    if ( ! cell )
        return 0;
    return dynamic_cast<MaterialTableItem*>(cell);
}

void PivotPropertyTable::setThickness ( int row, double thick )
{
    if ( row >= rowCount() || thick > Global::pivotThickLimit )
        return;

    QTableWidgetItem* cell = item( row, 0 );
    if ( cell )
        cell->setText( QString("%1").arg( thick,0,'f',2 ) );
    else {
        cell = new QTableWidgetItem( QString("%1").arg( thick,0,'f',2 ) );
        cell->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        setItem( row, 0, cell );
    }    
    horizontalHeader()->resizeSection( 0, Global::thicknessColumnWidth );
}

double PivotPropertyTable::getThickness ( int row ) const
{
    QTableWidgetItem* cell = item( row, 0 );
    Q_ASSERT( cell != 0 );
    return cell->text().toDouble();
}

void PivotPropertyTable::setMaterial ( int row, const TrussMaterial* m )
{
    MaterialTableItem* cell = getMaterialCell( row );
    if ( cell )
        cell->setMaterial( m );
    else {
        cell = new MaterialTableItem( m );
        setItem( row, 1, cell );
    }   
    horizontalHeader()->resizeSection( 1, Global::materialColumnWidth );
}

const TrussMaterial* PivotPropertyTable::getMaterial ( int row ) const
{
    MaterialTableItem* cell = getMaterialCell( row );
    if ( ! cell )
        return 0;

    return cell->getMaterial();
}

void PivotPropertyTable::addPivot ( const TrussPivot& pivot, int row )
{
    if ( row == -1 )
        row = pivot.getNumber() - 1;
    insertRow( row );
    setThickness( row, pivot.getThickness() );
    setMaterial( row, pivot.getMaterial() );
}

/*****************************************************************************
 * Truss Property Tab Widget
 *****************************************************************************/

TrussPropertyTabWidget::TrussPropertyTabWidget ( QWidget* p ) :
    QTabWidget( p ),
    focusWindow(0),
    loadTable(0),
    pivotPropTable(0),
    nodesNumbLabel(0),
    loadedNodesLabel(0),
    pivotsNumbLabel(0),
    createLoadCaseBtn(0),
    removeLoadCaseBtn(0),
    levelButton(0),
    loadCaseComboBox(0),
    thickSpinBox(0),
    materialComboBox(0),
    isLevelButtonEnabled(true)
{
    init();
}

TrussPropertyTabWidget::~TrussPropertyTabWidget ()
{}

void TrussPropertyTabWidget::init ()
{
    setFont( QFont( "Arial", 8 ) );
    initLoadTab();
    initPivotPropertyTab();
}

void TrussPropertyTabWidget::initLoadTab ()
{
    QFrame* parentFrame = new QFrame;
    nodesNumbLabel = new QLabel( tr( "Total nodes: " ), parentFrame );
    loadedNodesLabel = new QLabel( tr( "Nodes loaded: " ), parentFrame );
    loadTable = new LoadTable( parentFrame );
    LoadTableDelegate* delegate = new LoadTableDelegate;
    loadTable->setItemDelegate( delegate );
    
    connect( delegate, SIGNAL(cellWasChanged(int, int)),
                       SLOT(updateTrussLoad(int, int)) );

    QGroupBox* loadCaseGroupBox = 
            new QGroupBox( tr( "Load cases" ), parentFrame );
    QLabel* loadCaseNumb = 
        new QLabel( tr( "Current: " ), loadCaseGroupBox );
    loadCaseComboBox = new QComboBox( loadCaseGroupBox );
    createLoadCaseBtn = new QPushButton( loadCaseGroupBox );
    createLoadCaseBtn->setIcon( QIcon( Global::imagesPath() + "/plus.png" ) );
    createLoadCaseBtn->setFixedSize( QSize( 22, 22 ) );
    removeLoadCaseBtn = new QPushButton( loadCaseGroupBox );
    removeLoadCaseBtn->setIcon( QIcon( Global::imagesPath() + "/minus.png" ) );
    removeLoadCaseBtn->setFixedSize( QSize( 22, 22 ) );
    removeLoadCaseBtn->setEnabled( false );

    connect ( createLoadCaseBtn, SIGNAL(pressed()),
                                 SLOT(addLoadCase()) );
    connect ( removeLoadCaseBtn, SIGNAL(pressed()),
                                 SLOT(removeLoadCase()) );
    // init layout managers
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    QHBoxLayout* topLayout = new QHBoxLayout;
    QVBoxLayout* tableLayout = new QVBoxLayout;
    QHBoxLayout* loadCaseLayout = new QHBoxLayout( loadCaseGroupBox );
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( tableLayout );
    parentLayout->addStretch(1);
    topLayout->addWidget( nodesNumbLabel );
    topLayout->addWidget( loadedNodesLabel );
    tableLayout->addWidget( loadTable );
    parentLayout->addWidget( loadCaseGroupBox );
    loadCaseLayout->addWidget( loadCaseNumb );
    loadCaseLayout->addWidget( loadCaseComboBox );
    loadCaseLayout->addWidget( createLoadCaseBtn);
    loadCaseLayout->addWidget( removeLoadCaseBtn );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    tableLayout->setMargin( 1 );
    tableLayout->setSpacing( 1 );
    loadCaseLayout->setMargin( 6 );
    loadCaseLayout->setSpacing( 5 );
    parentLayout->setMargin( 1 );
    parentLayout->setSpacing( 1 );

    // init load table
    loadTable->setColumnCount( 2 );
    loadTable->setSelectionMode( QAbstractItemView::NoSelection );
    QHeaderView *horHeader = loadTable->horizontalHeader(),
                *vertHeader = loadTable->verticalHeader();
    horHeader->setClickable( false );
    horHeader->setResizeMode( QHeaderView::Custom );
    vertHeader->setDefaultSectionSize( Global::tableRowHeight );
    vertHeader->setClickable( false );
    vertHeader->setResizeMode( QHeaderView::Custom );
    loadTable->setShowGrid( true );

    QStringList headerList;
    headerList << tr("Froce by X") << tr("Force by Y");
    loadTable->setHorizontalHeaderLabels( headerList );
    loadTable->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    loadTable->hide();

    addTab( parentFrame, "Loads" );
}

void TrussPropertyTabWidget::initPivotPropertyTab ()
{
    QFrame* parentFrame = new QFrame;
    
    pivotsNumbLabel = new QLabel( tr( "Total pivots: " ), parentFrame );
    pivotPropTable = new PivotPropertyTable( parentFrame );
    PivotPropertyTableDelegate* delegate = new PivotPropertyTableDelegate;
    pivotPropTable->setItemDelegate( delegate );

    connect( delegate, SIGNAL(cellWasChanged(int, int)),
                       SLOT(updatePivotState(int, int)) );
    connect( this, 
             SIGNAL(onMaterialLibraryChanged(const TrussMaterialLibrary&)),
             delegate, 
             SLOT(setMaterialLibrary(const TrussMaterialLibrary&)) );

    QGroupBox* levelGroupBox = 
            new QGroupBox( tr( "Leveling" ), parentFrame );
    QComboBox* levelComboBox = new QComboBox( levelGroupBox );
    levelComboBox->addItem( "by Thickness" );
    levelComboBox->addItem( "by Material" );
    levelComboBox->setCurrentIndex( 0 );
    thickSpinBox = new QDoubleSpinBox( levelGroupBox );
    thickSpinBox->setMaximum( Global::pivotThickLimit );
    thickSpinBox->setSingleStep( 0.01 );
    thickSpinBox->setValue( 0.1 );
    thickSpinBox->setFixedHeight( 21 );
    materialComboBox = new MaterialComboBox( levelGroupBox );
    materialComboBox->setVisible( false );

    connect( levelComboBox, SIGNAL(currentIndexChanged(int)),
                            SLOT(changeLevelEditor(int)) );
   
    connect( materialComboBox, SIGNAL(comboBoxIsEmpty(bool)),
                               SLOT(updateLevelButtonState(bool)) );
    
    levelButton = new QPushButton( levelGroupBox );
    levelButton->setIcon( QIcon( Global::imagesPath() + "/tick.png" ) );
    levelButton->setFixedSize( QSize( 21, 21 ) );

    connect( levelButton, SIGNAL(pressed()),
                          SLOT(levelPivotState()) );

    // init layout managers
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    QHBoxLayout* topLayout = new QHBoxLayout;
    QVBoxLayout* tableLayout = new QVBoxLayout;
    QGridLayout* bottomLayout = new QGridLayout( levelGroupBox );
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( tableLayout );
    topLayout->addWidget( pivotsNumbLabel );
    tableLayout->addWidget( pivotPropTable );
    parentLayout->addStretch( 1 );
    parentLayout->addWidget( levelGroupBox );
    bottomLayout->addWidget( levelComboBox, 0, 0 );
    bottomLayout->addWidget( thickSpinBox, 0, 1 );
    bottomLayout->addWidget( materialComboBox, 0, 2 );
    bottomLayout->addWidget( levelButton, 0, 3 );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    tableLayout->setMargin( 1 );
    tableLayout->setSpacing( 1 );
    bottomLayout->setMargin( 6 );
    bottomLayout->setSpacing( 5 );
    parentLayout->setMargin( 1 );
    parentLayout->setSpacing( 1 );

    // init pivot table
    pivotPropTable->setColumnCount( 2 );
    pivotPropTable->setSelectionMode( QAbstractItemView::NoSelection );
    QHeaderView *horHeader = pivotPropTable->horizontalHeader(),
                *vertHeader = pivotPropTable->verticalHeader();
    horHeader->setClickable( false );
    horHeader->setResizeMode( QHeaderView::Custom );
    vertHeader->setDefaultSectionSize( Global::tableRowHeight );
    vertHeader->setClickable( false );
    vertHeader->setResizeMode( QHeaderView::Custom );
    pivotPropTable->setShowGrid( true );

    QStringList headerList;
    headerList << tr("Thickness") << tr("Material");
    pivotPropTable->setHorizontalHeaderLabels( headerList );
    pivotPropTable->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    pivotPropTable->hide();

    addTab( parentFrame, "Pivot Property" );
}


// connect new truss unit window with geometry widget
void TrussPropertyTabWidget::trussUnitWindowWasCreated ( 
    TrussUnitWindow& window )
{
    changeFocusWindow( &window );

    TrussUnit::LoadCases& loadCases = window.getLoadCases();
    
    // connections for load tab
    connect( &window, SIGNAL(afterNodeCreation(const Node&)),
                      SLOT(addLoadTableRow(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(addLoadTableRow(const Node&)) );
                    
    connect( &window, SIGNAL(beforeNodeDesist(const Node&)),
                      SLOT(removeLoadTableRow(const Node&)) );

    connect( &loadCases, SIGNAL(afterLoadCaseCreation(int)),
                         SLOT(fillLoadCaseComboBox()) );

    connect( &loadCases, SIGNAL(afterLoadCaseRemoval()),
                         SLOT(fillLoadCaseComboBox()) );

    connect( &loadCases, SIGNAL(currentLoadCaseChanged(int)),
                         SLOT(changeTabCurrentLoadCase(int)) );

    connect( &loadCases, SIGNAL(onTrussLoadChange(const Node&) ),
                         SLOT(updateTableLoad(const Node&)) );

    connect( &loadCases, SIGNAL(onTrussLoadCreate(const Node&)),
                         SLOT(updateTableLoad(const Node&)) );

    connect( &loadCases, SIGNAL(onTrussLoadRemove(const Node&) ),
                         SLOT(updateTableLoad(const Node&)) );

    connect( &loadCases, SIGNAL(loadCaseCanBeRemoved(bool)),
             removeLoadCaseBtn, SLOT(setEnabled(bool)) );

    connect( loadCaseComboBox, SIGNAL(currentIndexChanged(int)),
                               SLOT(setCurrentLoadCase(int)) );

    // connections for pivot table
    connect( &window, SIGNAL(afterPivotCreation(const Node&, const Node&)),
                      SLOT(addPivotToTable(const Node&, const Node&)) );

    connect( &window, SIGNAL(afterPivotRevive(const Node&, const Node&)),
                      SLOT(addPivotToTable(const Node&, const Node&)) );
                    
    connect( &window, SIGNAL(beforePivotDesist(const Node&, const Node&)),
                      SLOT(removePivotFromTable(const Node&, const Node&)) );
}

void TrussPropertyTabWidget::changeFocusWindow ( 
    TrussUnitWindow* newFocusWindow )
{
    focusWindow = newFocusWindow;
    if ( focusWindow ) {
        fillLoadTab();
        fillPivotPropertyTab();
        return;
    }
    loadTable->hide();
    createLoadCaseBtn->setEnabled( false );
    removeLoadCaseBtn->setEnabled( false );
    levelButton->setEnabled( false );
    pivotPropTable->hide();
    nodesNumbLabel->setText( "Total nodes: " );
    pivotsNumbLabel->setText( "Total pivots: " );
    loadedNodesLabel->setText( "Loaded nodes: " );
    loadCaseComboBox->clear();
}

void TrussPropertyTabWidget::changeMaterialLibrary ( 
    const TrussMaterialLibrary& lib )
{
    materialComboBox->setMaterialLibrary( lib );
    emit onMaterialLibraryChanged( lib );
}

void TrussPropertyTabWidget::clearMaterialComboBox ()
{
    materialComboBox->clearMaterialLibrary();
}

/******************************* loads ***************************************/

void TrussPropertyTabWidget::fillLoadTab ()
{
    TrussUnit::LoadCases& loadCases = focusWindow->getLoadCases();
    if ( loadCases.countLoadCases() == 1 )
        removeLoadCaseBtn->setEnabled( false );
    else
        removeLoadCaseBtn->setEnabled( true );
    createLoadCaseBtn->setEnabled( true );

    fillLoadCaseComboBox();

    // clear table
    loadTable->setRowCount( 0 );
    
    TrussUnit::NodeList nodeList = focusWindow->getNodeList();
    if ( nodeList.empty() ) {
        loadTable->hide();
        nodesNumbLabel->setText( "Total nodes: 0" );
        loadedNodesLabel->setText( "Loaded nodes: 0" );
        return;
    }
    loadTable->show();

    fillLoadTable( loadCases.getCurrentLoadCase() );

    nodesNumbLabel->setText( "Total nodes: " + 
                             QString::number(nodeList.size()) );
}

void TrussPropertyTabWidget::fillLoadTable ( 
    const TrussUnit::LoadCase* loadCase )
{
    if ( ! focusWindow || ! loadCase )
        return;
    
    TrussUnit::NodeList nodeList = focusWindow->getNodeList();
    TrussUnit::NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter ) {
        TrussNode& node = **iter;
        int row = node.getNumber() - 1;
        if ( row >= loadTable->rowCount() )
            loadTable->insertRow( row );
        TrussLoad* load = loadCase->findLoad( node );
        if ( load )
            loadTable->setLoad( row, *load );
        else
            loadTable->setLoad( row, TrussLoad( 0, 0 ) );
    }
    
    loadedNodesLabel->setText( "Loaded nodes: " + 
                      QString::number(loadTable->getLoadedNodesNumber()) );   
}

void TrussPropertyTabWidget::fillLoadCaseComboBox ()
{
    if ( ! focusWindow )
        return;

    loadCaseComboBox->blockSignals( true );
    loadCaseComboBox->clear();

    TrussUnit::LoadCases& loadCases = focusWindow->getLoadCases();
    QStringList argList;
    int i;
    for ( i = 0; i < loadCases.countLoadCases(); ++i )
        argList.push_back( QString::number( i + 1 ) );
    loadCaseComboBox->addItems( argList );

    TrussUnit::LoadCase* currentCase = loadCases.getCurrentLoadCase();
    if ( ! currentCase )
        return;
    int currentIndx = loadCases.getLoadCaseIndex( *currentCase );
    loadCaseComboBox->setCurrentIndex( currentIndx - 1 );
    loadCaseComboBox->blockSignals( false );
}

void TrussPropertyTabWidget::addLoadTableRow ( const Node& node )
{
    if ( sender() != focusWindow )
        return;

    const TrussNode* trussNode = dynamic_cast<const TrussNode*>(&node);
    Q_ASSERT( trussNode );
    connect( trussNode, SIGNAL(onVisibleChange(bool)),
                 SLOT(showLoadTableRow(bool)) );

    if ( loadTable->isHidden() )
        loadTable->show();

    TrussUnit::LoadCase* currentCase = 
        focusWindow->getLoadCases().getCurrentLoadCase();
    if ( ! currentCase )
        return;
    int row = trussNode->getNumber() - 1;
    loadTable->insertRow( row );
    TrussLoad* load = currentCase->findLoad( *trussNode );
    if ( load )
        loadTable->setLoad( row, *load );
    else
        loadTable->setLoad( row, TrussLoad( 0, 0 ) );

    nodesNumbLabel->setText( "Total nodes: " + 
                          QString::number(focusWindow->getNodeList().size()) );
}

void TrussPropertyTabWidget::showLoadTableRow ( bool visible )
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        if ( visible ) {
            loadTable->showRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size()) );
        }
        else {
            loadTable->hideRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size() - 1) );
        }
    }
    catch ( ... ) { return; }
}

void TrussPropertyTabWidget::removeLoadTableRow ( const Node& node )
{
    if ( sender() != focusWindow )
        return;
    
    if ( ! focusWindow->getLoadCases().countLoadCases() )
        return;

    loadTable->removeRow( node.getNumber() - 1 );
    nodesNumbLabel->setText( "Total nodes: " + 
                      QString::number(focusWindow->getNodeList().size() - 1) );
    loadedNodesLabel->setText( "Loaded nodes: " + 
                      QString::number(loadTable->getLoadedNodesNumber()) );

    if ( ! loadTable->rowCount() )
        loadTable->hide();
}

void TrussPropertyTabWidget::changeTabCurrentLoadCase ( int currentIndx )
{
    if ( ! focusWindow )
        return;

    fillLoadTable( focusWindow->getLoadCases().getCurrentLoadCase() );

    // Load cases index starts with 1, not 0
    loadCaseComboBox->setCurrentIndex( currentIndx - 1 );
}

void TrussPropertyTabWidget::addLoadCase ()
{
    if ( ! focusWindow )
        return;

    TrussUnit::LoadCases& loadCases = focusWindow->getLoadCases();
    loadCases.createLoadCase();
}

void TrussPropertyTabWidget::removeLoadCase ()
{
    if ( ! focusWindow )
        return;

    // Load cases index starts with 1, not 0
    int indx = loadCaseComboBox->currentIndex() + 1;
    TrussUnit::LoadCases& loadCases = focusWindow->getLoadCases();
    TrussUnit::LoadCase* loadCase = loadCases.findLoadCase( indx );
    if ( loadCase )
        loadCases.removeLoadCase( *loadCase );
}

void TrussPropertyTabWidget::setCurrentLoadCase ( int indx )
{
    if ( ! focusWindow )
        return;
    
    // Load cases index starts with 1, not 0
    TrussUnit::LoadCases& loadCases = focusWindow->getLoadCases();
    TrussUnit::LoadCase* loadCase = loadCases.findLoadCase( indx + 1 );
    if ( loadCase )
        loadCases.setCurrentLoadCase( *loadCase );
}

void TrussPropertyTabWidget::updateTableLoad ( const Node& node )
{
    if ( ! focusWindow )
        return; 

    int row = node.getNumber() - 1;
    TrussUnit::LoadCase* currentCase = 
        focusWindow->getLoadCases().getCurrentLoadCase();
    try { 
        const TrussNode& trussNode = 
            dynamic_cast<const TrussNode&>(node);
        TrussLoad* load = currentCase->findLoad( trussNode );
        if ( load )
            loadTable->setLoad( row, *load );
        else
            loadTable->setLoad( row, TrussLoad( 0, 0 ) );
    }
    catch ( ... ) { return; }

    loadedNodesLabel->setText( "Loaded nodes: " + 
                      QString::number(loadTable->getLoadedNodesNumber()) ); 
}

void TrussPropertyTabWidget::updateTrussLoad ( int row, int col )
{
    if ( ! focusWindow )
        return; 

    TrussUnit::LoadCase* loadCase = 
            focusWindow->getLoadCases().getCurrentLoadCase();
    
    if ( loadCase ) {
        TrussNode *node = focusWindow->findNodeByNumber( row + 1 );
        if ( ! node )
            return;

        double newXForce = loadTable->getLoadXForce( row );
        double newYForce = loadTable->getLoadYForce( row );

        TrussLoad* load = loadCase->findLoad( *node );
        if ( ! load )
            loadCase->addLoad( *node, newXForce,newYForce );        
        else if ( col == 0 )
            load->setXForce( newXForce );
        else
            load->setYForce( newYForce );
    }
}

/****************************** pivots ***************************************/

void TrussPropertyTabWidget::fillPivotPropertyTab ()
{
    // clear table
    pivotPropTable->setRowCount( 0 );

    TrussUnit::PivotList pivotList = focusWindow->getPivotList ();
    if ( pivotList.empty() ) {
        pivotPropTable->hide();
        levelButton->setEnabled( false );
        pivotsNumbLabel->setText( "Total pivots: 0" );
        return;
    }
    pivotPropTable->show();
    if ( thickSpinBox->isVisible() )
        levelButton->setEnabled( true );
    else
        levelButton->setEnabled( isLevelButtonEnabled );

    TrussUnit::PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
        pivotPropTable->addPivot( (const TrussPivot&)**iter );

    pivotsNumbLabel->setText( "Total pivots: " + 
                              QString::number(pivotList.size()) );
}

void TrussPropertyTabWidget::addPivotToTable ( 
    const Node& first, const Node& last )
{
    if ( sender() != focusWindow )
        return;

    if ( pivotPropTable->isHidden() ) {
        pivotPropTable->show();
        if ( thickSpinBox->isVisible() )
            levelButton->setEnabled( true );
        else
            levelButton->setEnabled( isLevelButtonEnabled );
    }

    try { 
        const TrussNode& node1 = dynamic_cast<const TrussNode&>(first);
        const TrussNode& node2 = dynamic_cast<const TrussNode&>(last);
        TrussPivot* pivot = focusWindow->findPivotByNodes( node1, node2);
        Q_ASSERT( pivot != 0 );
        pivotPropTable->addPivot( *pivot );
        connect( pivot, SIGNAL(onDrawingStatusChange(bool)),
                        SLOT(showPivotPropertyTableRow(bool)) );
        connect( pivot, SIGNAL(onThicknessChange(double)),
                        SLOT(updateTableThickness()) );
        connect( pivot, SIGNAL(onMaterialChange()),
                        SLOT(updateTableMaterial()) );
    } catch ( ... ) { return; }

    pivotsNumbLabel->setText( "Total pivots: " + 
                     QString::number(focusWindow->getPivotList().size()) );
}

void TrussPropertyTabWidget::removePivotFromTable ( const Node& first, 
                                                    const Node& last )
{
    if ( sender() != focusWindow )
        return;

    try { 
        const TrussNode& node1 = dynamic_cast<const TrussNode&>(first);
        const TrussNode& node2 = dynamic_cast<const TrussNode&>(last);
        TrussPivot* pivot = focusWindow->findPivotByNodes( node1, node2);
        Q_ASSERT( pivot != 0 );
        pivotPropTable->removeRow( pivot->getNumber() - 1 );
        disconnect( pivot, SIGNAL(onDrawingStatusChange(bool)),
                     this, SLOT(showPivotPropertyTableRow(bool)) );
        disconnect( pivot, SIGNAL(onThicknessChange(double)),
                     this, SLOT(updateTableThickness()) );
        disconnect( pivot, SIGNAL(onMaterialChange()),
                     this, SLOT(updateTableMaterial()) );
    } catch ( ... ) { return; }
    
    pivotsNumbLabel->setText( "Total pivots: " + 
                     QString::number(focusWindow->getPivotList().size() - 1) );

    if ( ! pivotPropTable->rowCount() ) {
        levelButton->setEnabled( false );
        pivotPropTable->hide();
    }
}

void TrussPropertyTabWidget::showPivotPropertyTableRow ( bool visible )
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        int row = pivot.getNumber() - 1;
        if ( visible ) {
            pivotPropTable->showRow( row );
            pivotsNumbLabel->setText( "Total pivots: " + 
                QString::number(focusWindow->getPivotList().size()) );
        }
        else {
            pivotPropTable->hideRow( row );
            pivotsNumbLabel->setText( "Total pivots: " + 
                QString::number(focusWindow->getPivotList().size() - 1) );
        }
    }
    catch ( ... ) { return; }
}

void TrussPropertyTabWidget::updateTableMaterial ()
{
    if ( ! focusWindow )
        return;
    
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        if ( focusWindow->findPivotByNumber( pivot.getNumber() ) == &pivot )
            pivotPropTable->setMaterial( pivot.getNumber() - 1, 
                                         pivot.getMaterial() );
    }
    catch ( ... ) { return; }
}

void TrussPropertyTabWidget::updateTableThickness ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        pivotPropTable->setThickness( pivot.getNumber() - 1, 
                                      pivot.getThickness() );
    }
    catch ( ... ) { return; }
}

void TrussPropertyTabWidget::updatePivotState ( int row, int col )
{
    TrussUnit::PivotList pivotList = focusWindow->getPivotList();
    TrussUnit::PivotListIter pivotIter = pivotList.begin();
    TrussPivot* pivot = 0;
    for ( ; pivotIter != pivotList.end(); ++pivotIter ) {
        pivot = *pivotIter;
        if ( pivot->getNumber() - 1 == row )
            break;
    }
    if ( ! pivot )
        return;

    if ( col == 0 ) {
        pivot->setThickness( pivotPropTable->getThickness( row ) );
        return;
    }
    else
        pivot->setMaterial( pivotPropTable->getMaterial( row ) );
}

void TrussPropertyTabWidget::levelPivotState ()
{
    TrussUnit::PivotList pivotList = focusWindow->getPivotList();
    TrussUnit::PivotListIter pivotIter = pivotList.begin();
    if ( thickSpinBox->isVisible() ) {
        for ( ; pivotIter != pivotList.end(); ++pivotIter )
            (*pivotIter)->setThickness( thickSpinBox->value() );
    } else if ( materialComboBox->isVisible() ) {
        for ( ; pivotIter != pivotList.end(); ++pivotIter ) {
            const TrussMaterial* m = materialComboBox->getCurrentMaterial();
            (*pivotIter)->setMaterial( m );
        }
    }
}

void TrussPropertyTabWidget::changeLevelEditor ( int indx )
{
    if ( indx == 0 ) {
        thickSpinBox->setVisible( true );
        materialComboBox->setVisible( false );
        if ( focusWindow )
            levelButton->setEnabled( true );
    } else {
        thickSpinBox->setVisible( false );
        materialComboBox->setVisible( true );
        if ( focusWindow )
            levelButton->setEnabled( isLevelButtonEnabled );
    }
}

void TrussPropertyTabWidget::updateLevelButtonState ( bool noMaterials )
{
    if ( focusWindow && materialComboBox->isVisible() )
        levelButton->setEnabled( ! noMaterials );
    isLevelButtonEnabled = ! noMaterials;
}

/***************************************************************************/
