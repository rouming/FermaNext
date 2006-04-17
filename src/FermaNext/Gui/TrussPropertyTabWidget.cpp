
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
#include <QPushButton>
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

void LoadTableDelegate::updateEditorGeometry ( QWidget* editor,
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
    
    horizontalHeader()->resizeSection( 0, loadColumnWidth );
    horizontalHeader()->resizeSection( 1, loadColumnWidth );
}

TrussLoad LoadTable::getLoad ( int row ) const
{
    QTableWidgetItem* cellX = item( row, 0 );
    Q_ASSERT( cellX != 0 );

    QTableWidgetItem* cellY = item( row, 1 );
    Q_ASSERT( cellY != 0 );

    TrussLoad load( cellX->text().toDouble(), cellY->text().toDouble() );
    return load;
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
 * Pivot Table Delegate
 *****************************************************************************/
/*
PivotTableDelegate::PivotTableDelegate ( QWidget* parent ) : 
    QItemDelegate( parent ),
    nodesNumb( 0 )
{}

QWidget* PivotTableDelegate::createEditor ( QWidget *parent, 
                                            const QStyleOptionViewItem& ,
                                            const QModelIndex& index ) const
{
    
    if ( index.column() != 2 ) {
        // create editor for node number cells
        QComboBox *editor = new QComboBox( parent );
        editor->installEventFilter(const_cast<PivotTableDelegate*>(this));
        return editor;
    }

    // create editor for thickness cell
    QLineEdit* editor = new QLineEdit( parent );
    editor->setFrame( false );
    RangeValidator* validator = new RangeValidator( editor );
    validator->setRange( 0.0, pivotThickLimit );
    validator->setDecimals( 2 );
    editor->setValidator( validator );
    editor->setMaxLength( 6 );
    return editor;
}

void PivotTableDelegate::setEditorData ( QWidget *editor,
                                         const QModelIndex &index ) const
{
    // fill editor of the node number cell
    if ( index.column() != 2 ) {
        int currValue = index.model()->data( index, Qt::DisplayRole ).toInt();
        QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
        Q_ASSERT( comboBox != 0 );
        comboBox->addItem( QString::number(currValue) );
        comboBox->addItems( this->getComboArgList( index ) );
        comboBox->setCurrentIndex( 0 );
        return;
    }

    // fill editor of the thickness cell
    QString thickness = index.model()->data( index, Qt::DisplayRole ).toString();
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( editor != 0 );
    lineEdit->setText( thickness );
    lineEdit->setAlignment( Qt::AlignLeft );
    return;
}

void PivotTableDelegate::setModelData ( QWidget* editor, 
                                        QAbstractItemModel* model, 
                                        const QModelIndex& index ) const
{
    // set data for node number cell
    if ( index.column() != 2 ) {
        QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
        QVariant newValue( comboBox->currentText() );
        model->setData( index, newValue );
        emit const_cast<PivotTableDelegate*>(this)->
                cellWasChanged( index.row(), index.column() );
        return;
    }

    // set data for thickness cell
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( lineEdit );
    QVariant newValue( lineEdit->text() );
    model->setData( index, newValue );
    emit const_cast<PivotTableDelegate*>(this)->
            cellWasChanged( index.row(), index.column() );

    return;
}

void PivotTableDelegate::updateEditorGeometry ( QWidget* editor,
                                            const QStyleOptionViewItem& option, 
                                            const QModelIndex& ) const
{
    editor->setGeometry( option.rect );
}
*/
/*****************************************************************************
 * Pivot Table
 *****************************************************************************/
/*
PivotTable::PivotTable ( QWidget* parent ) :
    QTableWidget( parent )
{}

void PivotTable::setNodeNumber ( int row, int col, int numb )
{
    QTableWidgetItem* cell = item( row, col );
    if ( cell )
        cell->setData( Qt::EditRole, QVariant(numb) );
    else {
        cell = new QTableWidgetItem;
        cell->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        cell->setData( Qt::EditRole, QVariant(numb) );
        setItem( row, col, cell );
    }
    horizontalHeader()->resizeSection( col, nodeColumnWidth );
}

void PivotTable::setThickness ( int row, double thick )
{
    QTableWidgetItem* cell = 
        new QTableWidgetItem( QString("%1").arg( thick,0,'f',2 ) );
    cell->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    setItem( row, 2, cell );
    horizontalHeader()->resizeSection( 2, thicknessColumnWidth );
}

double PivotTable::getThickness ( int row ) const
{
    QTableWidgetItem* cell = item( row, 2 );
    Q_ASSERT( cell != 0 );
    return cell->text().toDouble();
}

void PivotTable::addPivot ( const TrussPivot& pivot, int row )
{
    if ( row == -1 )
        row = pivot.getNumber() - 1;
    insertRow( row );
    Node& first = pivot.getFirstNode();
    Node& last = pivot.getLastNode();
    setNodeNumber( row, 0, first.getNumber() );
    setNodeNumber( row, 1, last.getNumber() );
    setThickness( row, pivot.getThickness() );
    for ( int i = row; i < rowCount(); ++i )
        verticalHeader()->resizeSection( i, tableRowHeight );
}

void PivotTable::setNodesTotalNumber ( int newNodesNumb )
{
    PivotTableDelegate* delegate = 
        dynamic_cast<PivotTableDelegate*>( itemDelegate() );
    Q_ASSERT( delegate != 0 );
    delegate->setNodesTotalNumber( newNodesNumb );
}
*/
/*****************************************************************************
 * Truss Property Tab Widget
 *****************************************************************************/

TrussPropertyTabWidget::TrussPropertyTabWidget ( QWidget* p ) :
    QTabWidget( p ),
    focusWindow( 0 ),
    loadTable( 0 ),
    //pivotPropTable( 0 ),
    nodesNumbLabel( 0 ),
    loadedNodesLabel( 0 ), 
    loadCaseGroupBox( 0 )//,
    //pivotsNumbLabel( 0 )
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

    loadCaseGroupBox = new QGroupBox( tr( "Load cases" ), parentFrame );
    QLabel* loadCaseNumb = 
        new QLabel( tr( "Current: " ), loadCaseGroupBox );
    loadCaseComboBox = new QComboBox( loadCaseGroupBox );
    createLoadCaseBtn = new QPushButton( loadCaseGroupBox );
    createLoadCaseBtn->setIcon( QIcon( imagesPath() + "/plus.png" ) );
    createLoadCaseBtn->setFixedSize( QSize( 22, 22 ) );
    removeLoadCaseBtn = new QPushButton( loadCaseGroupBox );
    removeLoadCaseBtn->setIcon( QIcon( imagesPath() + "/minus.png" ) );
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
    loadSpacer = new QSpacerItem( 0, 0 );
    parentLayout->addItem( loadSpacer );
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
    vertHeader->setDefaultSectionSize( tableRowHeight );
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
    /*
    pivotsNumbLabel = new QLabel( tr( "Total pivots: " ), parentFrame );
    pivotTable = new PivotTable( parentFrame );
    PivotTableDelegate* delegate = new PivotTableDelegate;
    pivotTable->setItemDelegate( delegate );
    connect( delegate, SIGNAL(cellWasChanged(int, int)),
                       SLOT(updatePivotState(int, int)) );

    // init layout managers
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    QHBoxLayout* topLayout = new QHBoxLayout;
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( bottomLayout );
    topLayout->addWidget( pivotsNumbLabel );
    bottomLayout->addWidget( pivotTable );
    pivotsSpacer = new QSpacerItem( 0, 0 );
    parentLayout->addItem( pivotsSpacer );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    bottomLayout->setMargin( 1 );
    bottomLayout->setSpacing( 1 );
    parentLayout->setMargin( 1 );
    parentLayout->setSpacing( 1 );

    // init pivot table
    pivotTable->setColumnCount( 3 );
    pivotTable->setSelectionMode( QAbstractItemView::NoSelection );

    QHeaderView *horHeader = pivotTable->horizontalHeader(),
                *vertHeader = pivotTable->verticalHeader();
    horHeader->setClickable( false );
    horHeader->setResizeMode( QHeaderView::Custom );
    vertHeader->setClickable( false );
    vertHeader->setResizeMode( QHeaderView::Custom );
    pivotTable->setShowGrid( true );

    QStringList headerList;
    headerList << tr("Node1") << tr("Node2") << tr("Thickness");
    pivotTable->setHorizontalHeaderLabels( headerList );
    pivotTable->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    pivotTable->hide();
*/
    addTab( parentFrame, "Pivot Property" );
}


// connect new truss unit window with geometry widget
void TrussPropertyTabWidget::trussUnitWindowWasCreated ( TrussUnitWindow& window )
{
    changeFocusWindow( &window );
    
    // connections for load tab
    connect( &window, SIGNAL(afterNodeCreation(const Node&)),
                      SLOT(addLoadTableRow(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(addLoadTableRow(const Node&)) );
                    
    connect( &window, SIGNAL(beforeNodeDesist(const Node&)),
                      SLOT(removeLoadTableRow(const Node&)) );

    connect( &window, SIGNAL(afterLoadCaseCreation(int)),
                      SLOT(fillLoadCaseComboBox()) );

    connect( &window, SIGNAL(afterLoadCaseRemoval()),
                      SLOT(fillLoadCaseComboBox()) );

    connect( &window, SIGNAL(currentLoadCaseChanged(int)),
                      SLOT(changeTabCurrentLoadCase(int)) );

    connect( &window, SIGNAL(onTrussLoadChange(const Node&) ),
                      SLOT(updateTableLoad(const Node&)) );

    connect( &window, SIGNAL(onTrussLoadCreate(const Node&)),
                      SLOT(updateTableLoad(const Node&)) );

    connect( &window, SIGNAL(onTrussLoadRemove(const Node&) ),
                      SLOT(updateTableLoad(const Node&)) );

    connect( &window, SIGNAL(loadCaseCanBeRemoved(bool)),
             removeLoadCaseBtn, SLOT(setEnabled(bool)) );

    connect( loadCaseComboBox, SIGNAL(currentIndexChanged(int)),
                               SLOT(setCurrentLoadCase(int)) );

/*
    // connections for pivot table
    connect( &window, SIGNAL(afterPivotCreation(const Node&, const Node&)),
                      SLOT(addPivotToTable(const Node&, const Node&)) );

    connect( &window, SIGNAL(afterPivotRevive(const Node&, const Node&)),
                      SLOT(addPivotToTable(const Node&, const Node&)) );
                    
    connect( &window, SIGNAL(beforePivotDesist(const Node&, const Node&)),
                      SLOT(removePivotFromTable(const Node&, const Node&)) );

    connect( &window, SIGNAL(afterNodeDesist(const Node&)),
                      SLOT(updateNodesNumbers(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(updateNodesNumbers(const Node&)) );*/
}

void TrussPropertyTabWidget::changeFocusWindow ( TrussUnitWindow* newFocusWindow )
{
    focusWindow = newFocusWindow;
    if ( focusWindow ) {
        fillLoadTab();
        //fillPivotPropertyTable();
        return;
    }
    loadTable->hide();
    createLoadCaseBtn->setEnabled( false );
    removeLoadCaseBtn->setEnabled( false );
    //pivotPropTable->hide();
    loadSpacer->changeSize( 0, loadTable->height() + 1 );
    //pivotPropSpacer->changeSize( 0, loadTable->height() + 1 );
    nodesNumbLabel->setText( "Total nodes: " );
    //pivotPropNumbLabel->setText( "Total pivots: " );
    loadedNodesLabel->setText( "Loaded nodes: " );
    loadCaseComboBox->clear();
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
        loadSpacer->changeSize( 0, loadTable->height() );
        nodesNumbLabel->setText( "Total nodes: 0" );
        loadedNodesLabel->setText( "Loaded nodes: 0" );
        return;
    }
    loadSpacer->changeSize( 0, 0 );
    loadTable->show();

    fillLoadTable( loadCases.getCurrentLoadCase() );

    nodesNumbLabel->setText( "Total nodes: " + 
                             QString::number(nodeList.size()) );
}

void TrussPropertyTabWidget::fillLoadTable ( const TrussUnit::LoadCase* loadCase )
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
    uint i;
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

    if ( loadTable->isHidden() ) {
        loadTable->show();
        loadSpacer->changeSize( 0, 0 );
    }

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

    if ( ! loadTable->rowCount() ) {
        loadSpacer->changeSize( 0, loadTable->height() );
        loadTable->hide();
    }
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

    focusWindow->createLoadCase();
}

void TrussPropertyTabWidget::removeLoadCase ()
{
    if ( ! focusWindow )
        return;

    // Load cases index starts with 1, not 0
    int indx = loadCaseComboBox->currentIndex() + 1;
    TrussUnit::LoadCase* loadCase = 
        focusWindow->getLoadCases().findLoadCase( indx );
    if ( loadCase )
        focusWindow->removeLoadCase( *loadCase );
}

void TrussPropertyTabWidget::setCurrentLoadCase ( int indx )
{
    if ( ! focusWindow )
        return;
    
    // Load cases index starts with 1, not 0
    TrussUnit::LoadCase* loadCase = 
        focusWindow->getLoadCases().findLoadCase( indx + 1 );
    if ( loadCase )
        focusWindow->setCurrentLoadCase( *loadCase );
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

        TrussLoad newLoad = loadTable->getLoad( row );
        TrussLoad* load = loadCase->findLoad( *node );
        if ( ! load )
            loadCase->addLoad( *node, newLoad.getXForce(),
                                      newLoad.getYForce() );        
        else if ( col == 0 )
            load->setXForce( newLoad.getXForce() );
        else
            load->setYForce( newLoad.getYForce() );
    }
}

/****************************** pivots ***************************************/
/*
void GeometryTabWidget::fillPivotTable ()
{
    // clear table
    pivotTable->setRowCount( 0 );

    TrussUnit::PivotList pivotList = focusWindow->getPivotList ();
    if ( pivotList.empty() ) {
        pivotTable->hide();
        pivotsSpacer->changeSize( 0, pivotTable->height() );
        pivotsNumbLabel->setText( "Total pivots: 0" );
        return;
    }
    pivotsSpacer->changeSize( 0, 0 );
    pivotTable->show();

    TrussUnit::PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
        pivotTable->addPivot( (const TrussPivot&)**iter );

    pivotsNumbLabel->setText( "Total pivots: " + 
                              QString::number(pivotList.size()) );
}

void GeometryTabWidget::addPivotToTable ( const Node& first, const Node& last )
{
    if ( sender() != focusWindow )
        return;

    if ( pivotTable->isHidden() ) {
        pivotTable->show();
        pivotsSpacer->changeSize( 0, 0 );
    }
    try { 
        const TrussNode& node1 = dynamic_cast<const TrussNode&>(first);
        const TrussNode& node2 = dynamic_cast<const TrussNode&>(last);
        TrussPivot* pivot = focusWindow->findPivotByNodes( node1, node2);
        Q_ASSERT( pivot != 0 );
        pivotTable->addPivot( *pivot );
        connect( pivot, SIGNAL(onDrawingStatusChange(bool)),
                        SLOT(showPivotTableRow(bool)) );
        connect( pivot, SIGNAL(onThicknessChange(double)),
                        SLOT(updatePivotTableThickness()) );
        connect( pivot, SIGNAL(onFirstNodeChange()),
                        SLOT(updatePivotTableFirstNode()) );
        connect( pivot, SIGNAL(onLastNodeChange()),
                        SLOT(updatePivotTableLastNode()) );
    } catch ( ... ) { return; }

    pivotTable->setNodesTotalNumber( focusWindow->getNodeList().size() );
    pivotsNumbLabel->setText( "Total pivots: " + 
                     QString::number(focusWindow->getPivotList().size()) );
}

void GeometryTabWidget::removePivotFromTable ( const Node& first, 
                                                 const Node& last )
{
    if ( sender() != focusWindow )
        return;

    try { 
        const TrussNode& node1 = dynamic_cast<const TrussNode&>(first);
        const TrussNode& node2 = dynamic_cast<const TrussNode&>(last);
        TrussPivot* pivot = focusWindow->findPivotByNodes( node1, node2);
        Q_ASSERT( pivot != 0 );
        pivotTable->removeRow( pivot->getNumber() - 1 );
        disconnect( pivot, SIGNAL(onDrawingStatusChange(bool)),
                     this, SLOT(showPivotTableRow(bool)) );
        disconnect( pivot, SIGNAL(onThicknessChange(double)),
                     this, SLOT(updatePivotTableThickness()) );
        disconnect( pivot, SIGNAL(onFirstNodeChange()),
                     this, SLOT(updatePivotTableFirstNode()) );
        disconnect( pivot, SIGNAL(onLastNodeChange()),
                     this, SLOT(updatePivotTableLastNode()) );
    } catch ( ... ) { return; }
    
    pivotsNumbLabel->setText( "Total pivots: " + 
                     QString::number(focusWindow->getPivotList().size() - 1) );

    if ( ! pivotTable->rowCount() ) {
        pivotsSpacer->changeSize( 0, loadTable->height() );
        pivotTable->hide();
    }
}

void GeometryTabWidget::updateNodesNumbers ( const Node& node )
{
    int numb = node.getNumber();
    QTableWidgetItem *item1 = 0, *item2 = 0;
    for ( int i = 0; i < pivotTable->rowCount(); ++i ) {
        item1 = pivotTable->item( i, 0 );
        if ( item1 && item1->data( Qt::DisplayRole ).toInt() >= numb ) {
            pivotTable->removeRow( i );
            TrussPivot* pivot = focusWindow->findPivotByNumber( i + 1 );
            if ( pivot ) {
                pivotTable->addPivot( *pivot, i );
                continue;
            }
        }
        item2 = pivotTable->item( i, 1 );
        if ( item2 && item2->data( Qt::DisplayRole ).toInt() >= numb ) {
            pivotTable->removeRow( i );
            TrussPivot* pivot = focusWindow->findPivotByNumber( i + 1 );
            if ( pivot ) {
                pivotTable->addPivot( *pivot, i );
                continue;
            }
        }
    }
}

void GeometryTabWidget::showPivotTableRow ( bool visible )
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        int row = pivot.getNumber() - 1;
        if ( visible ) {
            pivotTable->showRow( row );
            pivotsNumbLabel->setText( "Total pivots: " + 
                QString::number(focusWindow->getPivotList().size()) );
        }
        else {
            pivotTable->hideRow( row );
            pivotsNumbLabel->setText( "Total pivots: " + 
                QString::number(focusWindow->getPivotList().size() - 1) );
        }
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updatePivotTableFirstNode ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        int row = pivot.getNumber() - 1;
        int number = pivot.getFirstNode().getNumber();
        pivotTable->setNodeNumber( row, 0, number );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updatePivotTableLastNode ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        int row = pivot.getNumber() - 1;
        int number = pivot.getLastNode().getNumber();
        pivotTable->setNodeNumber( row, 1, number );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updatePivotTableThickness ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        pivotTable->setThickness( pivot.getNumber() - 1, 
                                  pivot.getThickness() );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updatePivotState ( int row, int col )
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

    if ( col == pivotTable->columnCount() - 1 ) {
        pivot->setThickness( pivotTable->getThickness( row ) );
        return;
    }

    QTableWidgetItem* item = pivotTable->item( row, col );
    Q_ASSERT( item != 0 );
    TrussNode* node = 
        focusWindow->findNodeByNumber( item->data( Qt::DisplayRole ).toInt() );
    if ( ! node )
        return;

    TrussNode &first = pivot->getFirstNode(),
              &last = pivot->getLastNode();

    if ( &first == node || &last == node )
        return;

    ObjectStateManager* mng = focusWindow->getStateManager();
    mng->startStateBlock();

    // Save remove node action
    ObjectState& state = pivot->createState( "change node" );
    if ( ! col ) {
        pivot->setFirstNode( node );
        state.addAction( new TrussPivotChangeNodeAction( *pivot, first,
                         pivot->getFirstNode(), true ) );
    }
    else {
        pivot->setLastNode( node );
        state.addAction( new TrussPivotChangeNodeAction( *pivot, last,
                         pivot->getLastNode(), false ) );
    }
    state.save();

    focusWindow->updateAfterPivotManipulation( pivot, true );

    mng->endStateBlock();
}
*/
/***************************************************************************/
