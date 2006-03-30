
#include <QCheckBox>
#include <QComboBox>
#include <QFont>
#include <QFrame>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QStringList>
#include <QStyle>

#include "GeometryTabWidget.h"
#include "TrussUnitActions.h"

/*****************************************************************************
 * Range Validator
 *****************************************************************************/

RangeValidator::RangeValidator ( QObject* parent ) :
    QDoubleValidator( parent )
{}

RangeValidator::RangeValidator ( double bottom, double top, int decimals, 
                                  QObject* parent ) :
    QDoubleValidator( bottom, top, decimals, parent )
{}   
    
QValidator::State RangeValidator::validate ( QString& input, int& pos ) const
{
    State s = QDoubleValidator::validate( input, pos );
    if ( s != Acceptable )
       return Invalid;
    return s;
}

/*****************************************************************************
 * Node Table Delegate
 *****************************************************************************/

NodeTableDelegate::NodeTableDelegate ( QObject* parent ) :
    QItemDelegate( parent ),
    areaSize( 0, 0 )
{}

QRect NodeTableDelegate::getCheckBoxDrawArea( const QStyleOptionViewItem &option, 
                                              bool forFirstCheckBox ) const
{
    int cellWidth = option.rect.bottomRight().x() - 
                    option.rect.topLeft().x();

    QPoint cbTopLeft;

    // check box top left positions
    if ( forFirstCheckBox )
        cbTopLeft = QPoint( option.rect.topLeft().x() + checkBoxIndent, 
                            option.rect.topLeft().y() + checkBoxIndent );
    else
        cbTopLeft = QPoint( option.rect.topLeft().x() + cellWidth / 2,
                            option.rect.topLeft().y() + checkBoxIndent );

    // get axis labels pixel size
    QSize xLabelSize = option.fontMetrics.size( Qt::TextSingleLine, "X" ),
          yLabelSize = option.fontMetrics.size( Qt::TextSingleLine, "Y" );

    return QRect( cbTopLeft, cbTopLeft + QPoint( checkBoxLen + 6 + 
                  xLabelSize.width(), checkBoxLen + 1 ) );
}

void NodeTableDelegate::getCheckStates ( bool& checkState1, bool& checkState2,
                                         const QModelIndex& index ) const
{
    QVariant fixation = index.model()->data( index, Qt::DisplayRole );
    Q_ASSERT( qVariantCanConvert<Node::Fixation>(fixation) );
    Node::Fixation fixType = 
              qVariantValue<Node::Fixation>( fixation );

    if ( fixType == Node::FixationByX ) {
        checkState1 = true;
        checkState2 = false;
    } else if ( fixType == Node::FixationByY ) {
        checkState1 = false;
        checkState2 = true;
    } else if ( fixType == Node::FixationByXY ) {
        checkState1 = true;
        checkState2 = true;
    } else {
        checkState1 = false;
        checkState2 = false;
    }
}

Node::Fixation NodeTableDelegate::getFixationByCheckStates( bool state1, 
                                                            bool state2 ) const
{
    Node::Fixation fixType;
    if ( state1 && ! state2 )
        fixType = Node::FixationByX;
    else if ( ! state1 && state2 )
        fixType = Node::FixationByY;
    else if ( state1 && state2 )
        fixType = Node::FixationByXY;
    else 
        fixType = Node::Unfixed;
    
    return fixType;
}

QWidget* NodeTableDelegate::createEditor ( QWidget *parent,
                                           const QStyleOptionViewItem&,
                                           const QModelIndex& index ) const
{
    // create editor only for coordinate cells
    if ( index.column() == 2 )
        return 0;

    QLineEdit* editor = new QLineEdit( parent );
    editor->setFrame( false );
    RangeValidator* validator = new RangeValidator( editor );
    if ( index.column() == 0 )
        validator->setRange( 0.0, areaSize.width() );
    else
        validator->setRange( 0.0, areaSize.height() );
    validator->setDecimals( 2 );
    editor->setValidator( validator );
    editor->setMaxLength( 6 );
    return editor;
}

void NodeTableDelegate::paint ( QPainter* painter, 
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index ) const
{
    if ( index.column() != 2 ) {
        QStyleOptionViewItem opt = option;
        
        // since we draw the grid ourselves
        opt.rect.adjust( 0, 0, -1, -1 );
        QItemDelegate::paint( painter, opt, index );
    } 
    else 
    {
        const QAbstractItemModel* model = index.model();
        QPalette::ColorGroup cg = option.state& QStyle::State_Enabled ? 
                                  QPalette::Normal : QPalette::Disabled;
        if ( option.state & QStyle::State_Selected )
            painter->fillRect( option.rect, 
                               option.palette.color( cg, QPalette::Highlight ) );
     
        QRect checkBoxRect1 = getCheckBoxDrawArea( option );
        QRect checkBoxRect2 = getCheckBoxDrawArea( option, false );

        // define area in which axis labels will be painted
        QRect strRect1 = 
            checkBoxRect1.adjusted( checkBoxLen + 1, 0, 0, 0 );
        QRect strRect2 = 
            checkBoxRect2.adjusted( checkBoxLen + 1, 0, 0, 0 );
        
        bool checked1, checked2;
        getCheckStates( checked1, checked2, index );

        drawCheck( painter, option, checkBoxRect1, checked1 ? Qt::Checked : 
                                                              Qt::Unchecked );
        drawDisplay( painter, option, strRect1, "X" );
        drawCheck( painter, option, checkBoxRect2, checked2 ? Qt::Checked :
                                                              Qt::Unchecked );
        drawDisplay( painter, option, strRect2, "Y" );

        // since we draw the grid ourselves
        drawFocus( painter, option, option.rect.adjusted( 0, 0, -1, -1 ) ); 
    }

    QPen pen = painter->pen();
    painter->setPen( option.palette.color( QPalette::Mid ) );
    painter->drawLine( option.rect.bottomLeft(), option.rect.bottomRight() );
    painter->drawLine( option.rect.topRight(), option.rect.bottomRight() );
    painter->setPen( pen );
}

void NodeTableDelegate::setEditorData ( QWidget* editor, 
                                        const QModelIndex& index ) const
{
    // set data for coordinate cell editor
    int row = index.row(),
        col = index.column();
    
    QVariant coordVariant = index.model()->data( index, Qt::DisplayRole );
    Q_ASSERT( qVariantCanConvert<QString>(coordVariant) );
    QString coord = coordVariant.toString();
    QLineEdit* coordEditor = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( coordEditor );
    coordEditor->setText( coord );
    coordEditor->setAlignment( Qt::AlignLeft );
    return;
}

void NodeTableDelegate::setModelData ( QWidget* editor, 
                                       QAbstractItemModel* model, 
                                       const QModelIndex& index ) const
{
    // set data for coordinate cell
    QLineEdit* coordEditor = dynamic_cast<QLineEdit*>( editor );
    Q_ASSERT( coordEditor );
    QVariant newCoordValue( coordEditor->text() );
    model->setData( index, newCoordValue );
    emit const_cast<NodeTableDelegate*>(this)->
            cellWasChanged( index.row(), index.column() );

    return;
}

QSize NodeTableDelegate::sizeHint( const QStyleOptionViewItem &option,
                                   const QModelIndex &index ) const
{
    if ( index.column() != 2 )
        return QSize( 40, tableRowHeight );
    else 
        return QSize( 60, tableRowHeight );
}

bool NodeTableDelegate::editorEvent ( QEvent* event, 
                                      QAbstractItemModel* model,
                                      const QStyleOptionViewItem& option,
                                      const QModelIndex& index )
{
    if ( index.column() != 2 )
        return false;

    QMouseEvent *mouseEvent;

    if ( event->type() == QEvent::MouseButtonRelease ) {
        mouseEvent = static_cast<QMouseEvent*>( event );
        
        QRect checkBoxRect1 = getCheckBoxDrawArea( option );
        QRect checkBoxRect2 = getCheckBoxDrawArea( option, false );

        if ( checkBoxRect1.contains( mouseEvent->x(), mouseEvent->y() ) )
        {
            bool xCheckState, yCheckState;
            getCheckStates( xCheckState, yCheckState, index );
          
            Node::Fixation fixType = 
                getFixationByCheckStates( ! xCheckState, yCheckState );
            
            QVariant fixation;
            qVariantSetValue<Node::Fixation>( fixation, fixType );
            model->setData( index, fixation );

            emit cellWasChanged( index.row(), index.column() );
        }
        else if ( checkBoxRect2.contains( mouseEvent->x(), mouseEvent->y() ) )
        {
            bool xCheckState, yCheckState;
            getCheckStates( xCheckState, yCheckState, index );
          
            Node::Fixation fixType = 
                getFixationByCheckStates( xCheckState, ! yCheckState );
            
            QVariant fixation;
            qVariantSetValue<Node::Fixation>( fixation, fixType );
            model->setData( index, fixation );

            emit cellWasChanged( index.row(), index.column() );
        }
    }    
    return true;
}

void NodeTableDelegate::updateEditorGeometry ( QWidget* editor,
                                            const QStyleOptionViewItem& option, 
                                            const QModelIndex& ) const
{
    editor->setGeometry( option.rect );
}


void NodeTableDelegate::updateTrussAreaSize ( const DoubleSize& newSize )
{
    areaSize = newSize;
}

/*****************************************************************************
 * Fixation Item
 *****************************************************************************/

FixationItem::FixationItem ( Node::Fixation fix, int type ) : 
    QTableWidgetItem( type )
{
    setFixation( fix );
}

FixationItem::FixationItem ( int type ) : 
    QTableWidgetItem( type )
{
    setFixation( Node::Unfixed );
}

void FixationItem::setFixation ( Node::Fixation fix )
{
    QVariant fixation;
    qVariantSetValue<Node::Fixation>( fixation, fix );
    setData( Qt::EditRole, fixation );
}

Node::Fixation FixationItem::getFixation () const
{
    QVariant fixation = data( Qt::DisplayRole );
    Q_ASSERT( qVariantCanConvert<Node::Fixation>(fixation) );
    return qVariantValue<Node::Fixation>( fixation );
}

/*****************************************************************************
 * Nodes Table
 *****************************************************************************/

NodeTable::NodeTable ( QWidget* parent ) :
    QTableWidget( parent )
{}

void NodeTable::setCoord ( int row, int col, double coord )
{
    QTableWidgetItem* coordCell = 
        new QTableWidgetItem( QString("%1").arg( coord,0,'f',2 ) );
    coordCell->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    setItem( row, col, coordCell );
    resizeColumnToContents( col );
}

double NodeTable::getCoord ( int row, int col ) const
{
    QTableWidgetItem* coordCell = item( row, col );
    Q_ASSERT( coordCell != 0 );
    return coordCell->text().toDouble();
}

void NodeTable::setFixationItem ( int row, Node::Fixation fix )
{
    int col = columnCount() - 1;
    FixationItem* fixCell = getFixationItem( row );
    if ( fixCell )
        fixCell->setFixation( fix );
    else {
        FixationItem* fixCell = new FixationItem( fix );
        setItem( row, col, fixCell );
        resizeColumnToContents( col );
    }
}

FixationItem* NodeTable::getFixationItem ( int row ) const
{
    return dynamic_cast<FixationItem*>( item(row, columnCount() - 1) );
}

int NodeTable::getFixedNodesNumber () const
{
    int fixedNumb = 0;
    FixationItem* item = 0;
    for ( int i = 0; i < rowCount(); ++i ) {
        item = getFixationItem( i );
        if ( item && item->getFixation() != Node::Unfixed )
            ++fixedNumb;
    }
    return fixedNumb;
}

void NodeTable::addNode ( const Node& node )
{
    int row = node.getNumber() - 1;
    insertRow( row );
    setCoord( row, 0, node.getX() );
    setCoord( row, 1, node.getY() );
    setFixationItem( row, node.getFixation() );
    for ( int i = row; i < rowCount(); ++i )
        verticalHeader()->resizeSection( i, tableRowHeight );
    //updateMaximumHeight();
}

void NodeTable::updateMaximumHeight ()
{
//    setMaximumHeight( rowCount() * tableRowHeight + lineWidth() * 2 +
//                      horizontalHeader()->height() );
}

/*****************************************************************************
 * Combo Item
 *****************************************************************************

ComboItem::ComboItem( QTable* table, int currentNumb, int adjNumb ) : 
    QTableItem( table, WhenCurrent, QString::number( currentNumb ) ), 
    comboBox( 0 ),
    currentValue( currentNumb ),
    adjNodeValue( adjNumb )
{
    setReplaceable( false );
}

void ComboItem::setAdjoiningNodeNumber ( int numb )
{   
    adjNodeValue = numb;
}

QWidget* ComboItem::createEditor() const
{
    const_cast<ComboItem*>(this)->comboBox = 
        new QComboBox( table()->viewport() );
    QObject::connect( comboBox, SIGNAL( activated( int ) ), 
                      table(), SLOT( doValueChanged() ) );
    comboBox->insertStringList( this->getComboArgList() );
    comboBox->setCurrentText( QString::number(currentValue) );
    return comboBox;
}

QStringList ComboItem::getComboArgList () const
{
    QStringList badArgList;
    QTableItem *cell = 0, *adjCell = 0;

    int i = 0;
    
    for ( i = 0; i < table()->numRows(); ++i ) {
        cell = table()->item( i, col() );
        adjCell = table()->item( i, ! col() );
        if ( cell && adjCell && cell->row() != row() ) {
            QString val = adjCell->text();
            if ( cell->text() == QString::number(adjNodeValue) )
                badArgList.append( adjCell->text() );
            else if ( adjCell->text() == QString::number(adjNodeValue) )
                badArgList.append( cell->text() );
        }
    }
    badArgList.append( QString::number(adjNodeValue) );
    
    PivotTable* pTable = dynamic_cast<PivotTable*>(table());
    Q_ASSERT( pTable != 0 );
    int nodesNumb = pTable->getNodesNumber();
    QStringList argList;
    for ( i = 0; i < nodesNumb; ++i ) {
        QString arg = QString::number(i + 1);
        if ( ! badArgList.contains( arg ) )
            argList.append( arg );
    }
    return argList;
}

void ComboItem::setContentFromEditor( QWidget *w )
{
    // the user changed the value of the combobox, so synchronize the
    // value of the item (its text), with the value of the combobox
    if ( w->inherits( "QComboBox" ) )
	    setText( dynamic_cast<QComboBox*>(w)->currentText() );
    else
	    QTableItem::setContentFromEditor( w );
}

void ComboItem::setText( const QString &s )
{
    if ( comboBox )
	    // initialize the combobox from the text
        comboBox->setCurrentText( s );
    QTableItem::setText( s );
    currentValue = s.toInt();
}

/*****************************************************************************
 * Pivot Table
 *****************************************************************************

PivotTable::PivotTable ( QWidget* parent, const char* name ) :
    QTable( parent, name ),
    nodesNumb( 0 )
{}

ComboItem* PivotTable::getComboItem ( int row, int col ) const
{
    return dynamic_cast<ComboItem*>( item(row, col) );
}

void PivotTable::setNodeNumber ( int row, int col, int numb, int adjNumb )
{
    setItem( row, col, new ComboItem(this, numb, adjNumb) );
    //ComboItem* cell = getComboItem( row, !col );
    //if ( cell )
    //    cell->setAdjoiningNodeNumber( adjNumb );
    setItem( row, !col, new ComboItem(this, adjNumb, numb) );
}

void PivotTable::setThickness ( int row, double thick )
{
    setText( row, numCols() - 1, QString("%1").arg( thick,0,'f',2 ) );
}

double PivotTable::getThickness ( int row ) const
{
    return text( row, numCols() - 1 ).toDouble();
}

void PivotTable::setNodesTotalNumber ( int newNodesNumb )
{
    nodesNumb = newNodesNumb;
}

int PivotTable::getNodesNumber () const
{
    return nodesNumb;
}

void PivotTable::addPivot ( const TrussPivot& pivot, int row )
{
    if ( row == -1 )
        row = pivot.getNumber() - 1;
    insertRows( row );
    setRowHeight( row, tableRowHeight );
    Node& first = pivot.getFirstNode();
    Node& last = pivot.getLastNode();
    setItem( row, 0, 
        new ComboItem(this, first.getNumber(), last.getNumber()) );
    setItem( row, 1, 
        new ComboItem(this, last.getNumber(), first.getNumber()) );
    setThickness( row, pivot.getThickness() );
}

QWidget* PivotTable::createEditor ( int row, int col, bool initFromCell ) const
{
    QTableItem* cell = item( row, col );
    if ( ! cell )
        return 0;

    if ( col != numCols() - 1 )
        return cell->createEditor();

    QLineEdit* editor = new QLineEdit( viewport() );
    editor->setFrame( false );
    editor->setValidator( new RangeValidator( 0.0, pivotThickLimit, 
                                                2, editor ) );
    editor->setMaxLength( 6 );
    if ( initFromCell )
        editor->setText( text( row, col ) );
    return editor;
}

/*****************************************************************************
 * Truss Geometry Tab Widget
 *****************************************************************************/

GeometryTabWidget::GeometryTabWidget ( QWidget* p ) :
    QTabWidget( p ),
    nodeTable( 0 ),
//    pivotTable( 0 ),
    focusWindow( 0 )
{
    init();
}

GeometryTabWidget::~GeometryTabWidget ()
{}

void GeometryTabWidget::init ()
{
    setFont( QFont( "Arial", 8 ) );
    initNodesTab();
    initPivotsTab();
    initAreaTab();
}

void GeometryTabWidget::initNodesTab ()
{
    QFrame* parentFrame = new QFrame;
    nodesNumbLabel = new QLabel( tr( "Total nodes: " ), parentFrame );
    fixedNodesLabel = new QLabel( tr( "Nodes fixed: " ), parentFrame );
    nodeTable = new NodeTable( parentFrame );
    NodeTableDelegate* delegate = new NodeTableDelegate;
    nodeTable->setItemDelegate( delegate );
    connect( delegate, SIGNAL(cellWasChanged(int, int)),
                       SLOT(updateNodeState(int, int)) );

    // init layout managers
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    QHBoxLayout* topLayout = new QHBoxLayout;
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( bottomLayout );
    topLayout->addWidget( nodesNumbLabel );
    topLayout->addWidget( fixedNodesLabel );
    bottomLayout->addWidget( nodeTable );
    nodesSpacer = new QSpacerItem( 0, 0 );
    parentLayout->addItem( nodesSpacer );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    bottomLayout->setMargin( 1 );
    bottomLayout->setSpacing( 1 );
    parentLayout->setMargin( 1 );
    parentLayout->setSpacing( 1 );
    
    // init nodes table
    nodeTable->setColumnCount( 3 );
    nodeTable->setSelectionMode( QAbstractItemView::NoSelection );
    
    QHeaderView *horHeader = nodeTable->horizontalHeader(),
                *vertHeader = nodeTable->verticalHeader();
    horHeader->setClickable( false );
    horHeader->setResizeMode( QHeaderView::Custom );
    vertHeader->setClickable( false );
    vertHeader->setResizeMode( QHeaderView::Custom );
    nodeTable->setShowGrid( false );

    QStringList headerList;
    headerList << "X" << "Y" << tr("Fixations");
    nodeTable->setHorizontalHeaderLabels( headerList );
    nodeTable->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    nodeTable->hide();

    addTab( parentFrame, "Nodes" );
}

void GeometryTabWidget::initPivotsTab ()
{
    QFrame* parentFrame = new QFrame;
    pivotsNumbLabel = new QLabel( tr( "Total pivots: " ), parentFrame );
/*
    pivotTable = new PivotTable( parentFrame );

    // init layout managers
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget( pivotsNumbLabel );
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget( pivotTable );
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( bottomLayout );
    pivotsSpacer = new QSpacerItem( 0, 0 );
    parentLayout->addItem( pivotsSpacer );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    bottomLayout->setMargin( 3 );
    bottomLayout->setSpacing( 5 );

    // init pivots table
    pivotTable->setNumRows( 3 );
    pivotTable->setNumCols( 3 );
    pivotTable->setSelectionMode( QTable::NoSelection );
    QHeader *horHeader = pivotTable->horizontalHeader(),
            *vertHeader = pivotTable->verticalHeader();
    horHeader->setLabel( 0, tr( "Node1" ) );
    horHeader->setLabel( 1, tr( "Node2" ) );
    horHeader->setLabel( 2, tr( "Thickness" ) );
    horHeader->setClickEnabled ( false );
    horHeader->setResizeEnabled ( false );
    vertHeader->setClickEnabled ( false );
    vertHeader->setResizeEnabled ( false );
    pivotTable->setVScrollBarMode( QScrollView::AlwaysOn );
    pivotTable->setColumnWidth( 0, 40 );
    pivotTable->setColumnWidth( 1, 40 );
    pivotTable->setColumnWidth( 2, 63 );
    pivotTable->setLeftMargin( 22 );
    pivotTable->hide();
    connect( pivotTable, SIGNAL(valueChanged(int, int)),
                          SLOT(updatePivotState(int, int)) );*/
    addTab( parentFrame, "Pivots" );
}

void GeometryTabWidget::initAreaTab ()
{
    QFrame* parentFrame = new QFrame;
/*
    QGroupBox* sizeGroupBox = new QGroupBox( 2, Qt::Vertical, 
                                             tr( "Area size" ), parentFrame );
    QHBox* xSizeLayout = new QHBox( sizeGroupBox );
    QLabel* xSizeLabel = new QLabel( tr( "Size by X:" ), xSizeLayout );
    QLineEdit* xSizeEdit = new QLineEdit( xSizeLayout );
    xSizeLayout->setSpacing( 5 );

    QHBox* ySizeLayout = new QHBox( sizeGroupBox );
    QLabel* ySizeLabel = new QLabel( tr( "Size by Y:" ), ySizeLayout );
    QLineEdit* ySizeEdit = new QLineEdit( ySizeLayout );
    ySizeLayout->setSpacing( 5 );

    QVBoxLayout* areaSizeLayout = new QVBoxLayout( parentFrame );
    areaSizeLayout->addWidget( sizeGroupBox );
    areaSizeLayout->setMargin( 5 );
    areaSizeLayout->setSpacing( 11 );
*/
    addTab( parentFrame, "Area" );
}

// connect new truss unit window with geometry widget
void GeometryTabWidget::trussUnitWindowWasCreated ( TrussUnitWindow& window )
{
    // connections for node table
    connect( &window, SIGNAL(afterNodeCreation(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );
                    
    connect( &window, SIGNAL(beforeNodeDesist(const Node&)),
                      SLOT(removeNodeFromTable(const Node&)) );

    NodeTableDelegate* delegate = 
        dynamic_cast<NodeTableDelegate*>( nodeTable->itemDelegate() );
    Q_ASSERT( delegate );

    connect( &window, SIGNAL(onAreaChange(const DoubleSize&)),
             delegate, SLOT(updateTrussAreaSize(const DoubleSize&)) );
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
                      SLOT(updateNodesNumbers(const Node&)) );
*/
    changeFocusWindow( &window );
}

void GeometryTabWidget::changeFocusWindow ( TrussUnitWindow* newFocusWindow )
{
    focusWindow = newFocusWindow;
    if ( focusWindow ) {
        fillNodeTable();
        //fillPivotTable();
        return;
    }
    nodeTable->hide();
    //pivotTable->hide();
    nodesSpacer->changeSize( 0, nodeTable->height() );
    //pivotsSpacer->changeSize( 0, nodeTable->height() );
    nodesNumbLabel->setText( "Total nodes: " );
    pivotsNumbLabel->setText( "Total pivots: " );
    fixedNodesLabel->setText( "Fixed nodes: " );
}

void GeometryTabWidget::saveNodeStateAfterMoving ( TrussNode& node,
                                                     const DoublePoint& pos )
{
    // Similar points. Nothing to save.
    if ( pos == beforeMovingNodePos )
        return;

    // Save move node action
    ObjectState& state = node.createState( "move node" );
    state.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
                                                     node, 
                                                     &TrussNode::setPoint,
                                                     &TrussNode::setPoint,
                                                     pos,
                                                     beforeMovingNodePos) );
    state.save();    
}

/******************************* nodes ***************************************/

void GeometryTabWidget::fillNodeTable ()
{
    // clear table
    nodeTable->setRowCount( 0 );
    
    TrussUnit::NodeList nodeList = focusWindow->getNodeList();
    if ( nodeList.empty() ) {
        nodeTable->hide();
        nodesSpacer->changeSize( 0, nodeTable->height() );
        nodesNumbLabel->setText( "Total nodes: 0" );
        fixedNodesLabel->setText( "Fixed nodes: 0" );
        return;
    }
    nodesSpacer->changeSize( 0, 0 );
    nodeTable->show();

    TrussUnit::NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
        nodeTable->addNode( (const Node&)**iter );

    nodesNumbLabel->setText( "Total nodes: " + 
                             QString::number(nodeList.size()) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodeTable->getFixedNodesNumber()) );    
}

void GeometryTabWidget::addNodeToTable ( const Node& node )
{
    if ( sender() != focusWindow )
        return;

    try { 
        const TrussNode& n = dynamic_cast<const TrussNode&>(node);
        connect( &n, SIGNAL(onVisibleChange(bool)),
                     SLOT(showNodeTableRow(bool)) );
    }
    catch ( ... ) { return; }

    if ( nodeTable->isHidden() ) {
        nodeTable->show();
        nodesSpacer->changeSize( 0, 0 );
    }

    nodeTable->addNode( node );
    nodesNumbLabel->setText( "Total nodes: " + 
                          QString::number(focusWindow->getNodeList().size()) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                          QString::number(nodeTable->getFixedNodesNumber()) );

    connect( &node, SIGNAL(onPositionChange(double, double)),
                    SLOT(updateNodeTableCoords()) );
    connect( &node, SIGNAL(onFixationChange(Fixation)),
                    SLOT(updateNodeTableFixation()) );

    //pivotTable->setNodesTotalNumber( focusWindow->getNodeList().size() );
}

void GeometryTabWidget::showNodeTableRow ( bool visible )
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        if ( visible ) {
            nodeTable->showRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size()) );
            //nodeTable->updateMaximumHeight();
        }
        else {
            nodeTable->hideRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size() - 1) );
            /*
            QHeader* horHeader = nodeTable->horizontalHeader();
            nodeTable->setMaximumHeight( ( nodeTable->numRows() - 1 ) * 
                                         tableRowHeight + 
                                         horHeader->height() + 
                                         nodeTable->lineWidth() * 2 );*/
        }
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::removeNodeFromTable ( const Node& node )
{
    if ( sender() != focusWindow )
        return;

    nodeTable->removeRow( node.getNumber() - 1 );
    nodesNumbLabel->setText( "Total nodes: " + 
                      QString::number(focusWindow->getNodeList().size() - 1) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodeTable->getFixedNodesNumber()) );

    if ( ! nodeTable->rowCount() ) {
        nodesSpacer->changeSize( 0, nodeTable->height() );
        nodeTable->hide();
    }
    //nodeTable->updateMaximumHeight();

    // prevent double connection after revive
    disconnect( &node, SIGNAL(onPositionChange(double, double)),
                this,  SLOT(updateNodeTableCoords()) );
    disconnect( &node, SIGNAL(onFixationChange(Fixation)),
                this,  SLOT(updateNodeTableFixation()) );
    disconnect( &node, SIGNAL(onVisibleChange(bool)),
                this, SLOT(showNodeTableRow(bool)) );

    //pivotTable->setNodesTotalNumber( focusWindow->getNodeList().size() - 1 );
}

void GeometryTabWidget::updateNodeTableCoords ()
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        nodeTable->setCoord( row, 0, node.getX() );
        nodeTable->setCoord( row, 1, node.getY() );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updateNodeTableFixation ()
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        nodeTable->setFixationItem( node.getNumber() - 1, 
                                    node.getFixation() );
        fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodeTable->getFixedNodesNumber()) );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updateNodeState ( int row, int col )
{
    TrussUnit::NodeList nodeList = focusWindow->getNodeList();
    if ( nodeList.empty() )
        return;

    TrussUnit::NodeListIter iter = nodeList.begin();
    TrussNode* node = 0;
    for ( ; iter != nodeList.end(); ++iter ) {
        node = *iter;
        if ( node->getNumber() - 1 == row )
            break;
    }

    if ( ! node )
        return;

    if ( col != nodeTable->columnCount() - 1 ) {
        // Save pos for Undo/Redo
        beforeMovingNodePos = node->getPoint();

        if ( col == 0 )
            node->setPoint( nodeTable->getCoord( row, 0 ),
                            beforeMovingNodePos.y() );
        else if ( col == 1 )
            node->setPoint( beforeMovingNodePos.x(),
                            nodeTable->getCoord( row, 1 ) );

        ObjectStateManager* mng = focusWindow->getStateManager();
        mng->startStateBlock();
        // Save state in Undo/Redo stack
        saveNodeStateAfterMoving( *node, node->getPoint() );
        focusWindow->updateAfterNodeManipulation ( node, true );
        mng->endStateBlock();
    }
    else {
        FixationItem* item = nodeTable->getFixationItem( row );
        if ( item ) {
            node->setFixation( item->getFixation() );
            fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodeTable->getFixedNodesNumber()) );
        }
    }
}

/****************************** pivots ***************************************

void GeometryTabWidget::fillPivotTable ()
{
    // clear table
    pivotTable->setNumRows( 0 );

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

    if ( ! pivotTable->numRows() ) {
        pivotsSpacer->changeSize( 0, nodeTable->height() );
        pivotTable->hide();
    }
}

void GeometryTabWidget::updateNodesNumbers ( const Node& node )
{
    int numb = node.getNumber();
    QTableItem *item1 = 0, *item2 = 0;
    for ( int i = 0; i < pivotTable->numRows(); ++i ) {
        item1 = pivotTable->item( i, 0 );
        if ( item1 && item1->text().toInt() >= numb ) {
            pivotTable->removeRow( i );
            TrussPivot* pivot = focusWindow->findPivotByNumber( i + 1 );
            if ( pivot ) {
                pivotTable->addPivot( *pivot, i );
                continue;
            }
        }
        item2 = pivotTable->item( i, 1 );
        if ( item2 && item2->text().toInt() >= numb ) {
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
        int number = pivot.getFirstNode().getNumber(),
            adjNumb = pivot.getLastNode().getNumber();
        pivotTable->setNodeNumber( row, 0, number, adjNumb );
    }
    catch ( ... ) { return; }
}

void GeometryTabWidget::updatePivotTableLastNode ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        int row = pivot.getNumber() - 1;
        int number = pivot.getLastNode().getNumber(),
            adjNumb = pivot.getFirstNode().getNumber();
        pivotTable->setNodeNumber( row, 1, number, adjNumb );
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

    if ( col == pivotTable->numCols() - 1 ) {
        pivot->setThickness( pivotTable->getThickness( row ) );
        return;
    }

    QTableItem* item = pivotTable->item( row, col );
    Q_ASSERT( item != 0 );
    TrussNode* node = focusWindow->findNodeByNumber( item->text().toInt() );
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
void GeometryTabWidget::changeTrussAreaSize ( const QString& newSizeStr )
{}

/****************************************************************************/
