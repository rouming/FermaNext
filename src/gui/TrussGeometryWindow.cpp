
#include <QCheckBox>
#include <QComboBox>
#include <QFont>
#include <QFrame>
#include <QGroupBox>
#include <QHBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPalette> 
#include <QRect>
#include <QStringList>
#include <QStyle>

#include "TrussGeometryWindow.h"
#include "TrussUnitActions.h"

/*****************************************************************************
 * Range Validator
 *****************************************************************************/

RangeValidator::RangeValidator ( QObject* parent, const char* name ) :
    QDoubleValidator( parent, name )
{}

RangeValidator::RangeValidator ( double bottom, double top, int decimals, 
                                  QObject* parent, const char* name ) :
    QDoubleValidator( bottom, top, decimals, parent, name )
{}   
    
QValidator::State RangeValidator::validate ( QString& input, int& pos ) const
{
    State s = QDoubleValidator::validate( input, pos );
    if ( s != Acceptable )
       return Invalid;
    return s;
}

/*****************************************************************************
 * Double Check Box
 *****************************************************************************/

DoubleCheckBox::DoubleCheckBox ( const QString& label1, const QString& label2,
                                 QWidget* parent, const char* name, WFlags f ) :
    QButton( parent, name, f ),
    firstLabel( label1 ),
    secondLabel( label2 )
{
    init();
}

DoubleCheckBox::DoubleCheckBox ( QWidget* parent, const char* name, WFlags f ) :
    QButton( parent, name, f )
{
    init();
}

void DoubleCheckBox::init ()
{
    QHBoxLayout* horLayout = new QHBoxLayout( this );
    checkBox1 = new QCheckBox( firstLabel, this );
    checkBox2 = new QCheckBox( secondLabel, this );

    QSpacerItem* nodesSpacer = new QSpacerItem( 1, 0 );
    horLayout->addItem( nodesSpacer );

    horLayout->addWidget( checkBox1 );
    horLayout->addWidget( checkBox2 );
    horLayout->setMargin( 1 );
    connect( checkBox1, SIGNAL( toggled(bool) ), 
                        SIGNAL( onValueChange() ) );
    connect( checkBox2, SIGNAL( toggled(bool) ), 
                        SIGNAL( onValueChange() ) );
}

void DoubleCheckBox::setFirstChecked ( bool check )
{
    checkBox1->setChecked( check );
}

void DoubleCheckBox::setSecondChecked ( bool check )
{
    checkBox2->setChecked( check );
}

bool DoubleCheckBox::isFirstChecked () const
{
    return checkBox1->isChecked();
}

bool DoubleCheckBox::isSecondChecked () const
{
    return checkBox2->isChecked();
}

void DoubleCheckBox::setFirstLabel ( QString& label )
{
    firstLabel = label;
}

void DoubleCheckBox::setSecondLabel ( QString& label )
{
    secondLabel = label;
}

void DoubleCheckBox::mousePressEvent ( QMouseEvent* me )
{
    if ( checkBox1 ) {
        QPoint childCoord = checkBox1->mapFrom( this, me->pos() );
        if ( checkBox1->rect().contains( childCoord ) )
            checkBox1->toggle();
    }
    
    if ( checkBox2 ) {
        QPoint childCoord = checkBox2->mapFrom( this, me->pos() );
        if ( checkBox2->rect().contains( childCoord ) )
            checkBox2->toggle();
    }

}

/*****************************************************************************
 * Fixation Item
 *****************************************************************************/

FixationItem::FixationItem( QTable *table, Node::Fixation fix ) : 
    QTableItem( table, WhenCurrent ),
    fixCheckBox( 0 )
{
    setFixation( fix );
}

FixationItem::FixationItem( QTable *table ) : 
    QTableItem( table, WhenCurrent ),
    fixCheckBox( 0 ),
    fixType( Node::Unfixed ),
    xChecked( false ),
    yChecked( false )
{}

void FixationItem::setFixation ( Node::Fixation fix )
{
    if ( fix == fixType )
        return;

    if ( fix == Node::FixationByX ) {
        xChecked = true;
        yChecked = false;
    }
    else if ( fix == Node::FixationByY ) {
        xChecked = false;
        yChecked = true;
    }
    else if ( fix == Node::FixationByXY ) {
        xChecked = true;
        yChecked = true;
    }
    else {
        xChecked = false;
        yChecked = false;
    }
    fixType = fix;
}

void FixationItem::setFixation ( bool xFix, bool yFix ) 
{
    if ( xFix && ! yFix )
        fixType = Node::FixationByX;
    else if ( ! xFix && yFix )
        fixType = Node::FixationByY;
    else if ( xFix && yFix )
        fixType = Node::FixationByXY;
    else 
        fixType = Node::Unfixed;
}

Node::Fixation FixationItem::getFixation () const
{
    return fixType;
}

QWidget* FixationItem::createEditor() const
{
    const_cast<FixationItem*>(this)->fixCheckBox = 
        new DoubleCheckBox( "X", "Y", table()->viewport() );
    fixCheckBox->setFirstChecked( xChecked );
    fixCheckBox->setSecondChecked( yChecked );
    fixCheckBox->
        setPaletteBackgroundColor( table()->viewport()->backgroundColor() );
    connect( fixCheckBox, SIGNAL(onValueChange()), 
                          SLOT(setValueChanged()) );
    return fixCheckBox;
}

void FixationItem::setContentFromEditor( QWidget *widget )
{
    DoubleCheckBox *editor = ::qt_cast<DoubleCheckBox*>( widget );
    if ( editor ) {
        xChecked = editor->isFirstChecked();
        yChecked = editor->isSecondChecked();
    }
}

void FixationItem::setValueChanged ()
{
    setFixation( fixCheckBox->isFirstChecked(),
	             fixCheckBox->isSecondChecked() );
    emit onTableFixValueChange( row(), col() );
}

void FixationItem::paint ( QPainter* p, const QColorGroup& cg,
				           const QRect& cr, bool selected )
{
    p->fillRect( 0, 0, cr.width(), cr.height(),
		         selected ? cg.brush( QColorGroup::Highlight ) : 
                 cg.brush( QColorGroup::Base ) );

    QSize sz = QSize( table()->style().pixelMetric( QStyle::PM_IndicatorWidth ),
		              table()->style().pixelMetric( QStyle::PM_IndicatorHeight ) );
    
    QColorGroup c( cg );
    c.setBrush( QColorGroup::Background, c.brush( QColorGroup::Base ) );
    QStyle::SFlags flags = QStyle::Style_Default;
    
    if( isEnabled() )
	    flags |= QStyle::Style_Enabled;
    if ( xChecked )
	    flags |= QStyle::Style_On;
    else
	    flags |= QStyle::Style_Off;
    if ( isEnabled() && table()->isEnabled() )
	    flags |= QStyle::Style_Enabled;

    QRect primArea = QRect( 2, ( cr.height() - sz.height() ) / 2, 
                            sz.width(), sz.height() );
    table()->style().drawPrimitive( QStyle::PE_Indicator, p, 
                                    primArea, c, flags );

    int x = sz.width() + 7;
    int w = cr.width() - x;
    int h = cr.height();

    if ( selected )
	    p->setPen( cg.highlightedText() );
    else
	    p->setPen( cg.text() );

    p->drawText( x, 0, w, h, wordWrap() ? ( alignment() | QPainter::WordBreak ) : 
                 alignment(), "X" );

    flags = QStyle::Style_Default;

    if( isEnabled() )
	    flags |= QStyle::Style_Enabled;
    if ( yChecked )
	    flags |= QStyle::Style_On;
    else
	    flags |= QStyle::Style_Off;
    if ( isEnabled() && table()->isEnabled() )
	    flags |= QStyle::Style_Enabled;

    x += 12;
    primArea = QRect( x, ( cr.height() - sz.height() ) / 2, 
                      sz.width(), sz.height() );
    table()->style().drawPrimitive( QStyle::PE_Indicator, p, primArea, c, flags );

    x += 18;
    p->drawText( x, 1, w, h, 0, "Y" );
}

/*****************************************************************************
 * Nodes Table
 *****************************************************************************/

NodeTable::NodeTable ( QWidget* parent, const char* name ) :
    QTable( parent, name ),
    areaSize( 0, 0 )
{}

void NodeTable::setCoord ( int row, int col, double coord )
{
    setText( row, col, QString("%1").arg( coord,0,'f',2 ) );
}

double NodeTable::getCoord ( int row, int col ) const
{
    return text( row, col ).toDouble();
}

void NodeTable::setFixationItem ( int row, Node::Fixation fix )
{
    int col = numCols() - 1;
    FixationItem* fixCell = getFixationItem( row );
    if ( fixCell ) {
        fixCell->setFixation( fix );
        updateCell( row, col );
    }
    else {
        FixationItem* fixCell = new FixationItem( this, fix );
        setItem( row, col, fixCell );
        QObject::connect( fixCell, SIGNAL(onTableFixValueChange(int, int)),
                                   SIGNAL(valueChanged(int, int)) );
    }
}

FixationItem* NodeTable::getFixationItem ( int row ) const
{
    return dynamic_cast<FixationItem*>( item(row, numCols() - 1) );
}

int NodeTable::getFixedNodesNumber () const
{
    int fixedNumb = 0;
    FixationItem* item = 0;
    for ( int i = 0; i < numRows(); ++i ) {
        item = getFixationItem( i );
        if ( item && item->getFixation() != Node::Unfixed )
            ++fixedNumb;
    }
    return fixedNumb;
}

void NodeTable::addNode ( const Node& node )
{
    int row = node.getNumber() - 1;
    insertRows( row );
    setRowHeight( row, tableRowHeight );
    setCoord( row, 0, node.getX() );
    setCoord( row, 1, node.getY() );
    setFixationItem( row, node.getFixation() );
    //updateMaximumHeight();
}

void NodeTable::updateMaximumHeight ()
{
    setMaximumHeight( numRows() * tableRowHeight + lineWidth() * 2 +
                      horizontalHeader()->height() );
}

QWidget* NodeTable::createEditor ( int row, int col, bool initFromCell ) const
{
    QTableItem* cell = item( row, col );
    if ( ! cell )
        return 0;

    if ( col == numCols() - 1 )
        return item(row, col)->createEditor();

    QLineEdit* editor = new QLineEdit( viewport() );
    editor->setFrame( false );
    RangeValidator* validator = new RangeValidator( editor );
    if ( col == 0 )
        validator->setRange( 0.0, areaSize.width() );
    else
        validator->setRange( 0.0, areaSize.height() );
    validator->setDecimals( 2 );
    editor->setValidator( validator );
    editor->setMaxLength( 6 );
    if ( initFromCell )
        editor->setText( text( row, col ) );
    return editor;
}

void NodeTable::updateTrussAreaSize ( const DoubleSize& newSize )
{
    areaSize = newSize;
}

/*****************************************************************************
 * Combo Item
 *****************************************************************************/

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
 *****************************************************************************/

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
 * Truss Geometry Window
 *****************************************************************************/

TrussGeometryWindow::TrussGeometryWindow ( QWidget* p, Qt::WFlags f ) :
    QTabWidget( p, f ),
    nodeTable( 0 ),
    pivotTable( 0 ),
    focusWindow( 0 )
{
    init();
}

TrussGeometryWindow::~TrussGeometryWindow ()
{}

void TrussGeometryWindow::init ()
{
    setCaption( tr("Truss Geometry") );
    setFont( QFont( "Arial", 8 ) );
    setMinimumWidth( 160 );
    setMaximumWidth( 195 );
    setFixedHeight( 174 );
    initNodesTab();
    initPivotsTab();
    initAreaTab();
}

void TrussGeometryWindow::initNodesTab ()
{
    QFrame* parentFrame = new QFrame( this, "nodes_frame",
                                      QFrame::Panel | QFrame::Plain );
    nodesNumbLabel = new QLabel( tr( "Total nodes: " ), parentFrame );
    fixedNodesLabel = new QLabel( tr( "Nodes fixed: " ), parentFrame );
    nodeTable = new NodeTable( parentFrame );

    // init layout managers
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget( nodesNumbLabel );
    topLayout->addWidget( fixedNodesLabel );
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget( nodeTable );
    QVBoxLayout* parentLayout = new QVBoxLayout( parentFrame );
    parentLayout->addLayout( topLayout );
    parentLayout->addLayout( bottomLayout );
    nodesSpacer = new QSpacerItem( 0, 0 );
    parentLayout->addItem( nodesSpacer );
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    bottomLayout->setMargin( 3 );
    bottomLayout->setSpacing( 5 );
    
    // init nodes table
    nodeTable->setNumCols( 3 );
    nodeTable->setSelectionMode( QTable::NoSelection );
    QHeader *horHeader = nodeTable->horizontalHeader(),
            *vertHeader = nodeTable->verticalHeader();
    horHeader->setLabel( 0, tr( "X" ) );
    horHeader->setLabel( 1, tr( "Y" ) );
    horHeader->setLabel( 2, tr( "Fixations" ) );
    horHeader->setClickEnabled ( false );
    horHeader->setResizeEnabled ( false );
    vertHeader->setClickEnabled ( false );
    vertHeader->setResizeEnabled ( false );
    nodeTable->setVScrollBarMode( QScrollView::AlwaysOn );
    nodeTable->setColumnWidth( 0, 40 );
    nodeTable->setColumnWidth( 1, 40 );
    nodeTable->setColumnWidth( 2, 63 );
    nodeTable->setLeftMargin( 22 );
    nodeTable->hide();
    connect( nodeTable, SIGNAL(valueChanged(int, int)),
                        SLOT(updateNodeState(int, int)) );
    addTab( parentFrame, "Nodes" );
}

void TrussGeometryWindow::initPivotsTab ()
{
    QFrame* parentFrame = new QFrame( this, "pivots_frame",
                                      QFrame::Panel | QFrame::Plain );
    pivotsNumbLabel = new QLabel( tr( "Total pivots: " ), parentFrame );
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
                          SLOT(updatePivotState(int, int)) );
    addTab( parentFrame, "Pivots" );
}

void TrussGeometryWindow::initAreaTab ()
{
    QFrame* parentFrame = new QFrame( this, "area_frame",
                                      QFrame::Panel | QFrame::Plain );
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

    addTab( parentFrame, "Area" );
}

// connect new truss unit window with geometry widget
void TrussGeometryWindow::trussUnitWindowWasCreated ( TrussUnitWindow& window )
{
    // connections for node table
    connect( &window, SIGNAL(afterNodeCreation(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );
                    
    connect( &window, SIGNAL(beforeNodeDesist(const Node&)),
                      SLOT(removeNodeFromTable(const Node&)) );

    connect( &window, SIGNAL(onAreaChange(const DoubleSize&)),
             nodeTable, SLOT(updateTrussAreaSize(const DoubleSize&)) );

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

    changeFocusWindow( &window );
}

void TrussGeometryWindow::changeFocusWindow ( TrussUnitWindow* newFocusWindow )
{
    focusWindow = newFocusWindow;
    if ( focusWindow ) {
        fillNodeTable();
        fillPivotTable();
        return;
    }
    nodeTable->hide();
    pivotTable->hide();
    nodesSpacer->changeSize( 0, nodeTable->height() );
    pivotsSpacer->changeSize( 0, nodeTable->height() );
    nodesNumbLabel->setText( "Total nodes: " );
    pivotsNumbLabel->setText( "Total pivots: " );
    fixedNodesLabel->setText( "Fixed nodes: " );
}

void TrussGeometryWindow::saveNodeStateAfterMoving ( TrussNode& node,
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

void TrussGeometryWindow::closeEvent ( QCloseEvent* )
{
    emit onGeometryWindowClose();
}

/******************************* nodes ***************************************/

void TrussGeometryWindow::fillNodeTable ()
{
    // clear table
    nodeTable->setNumRows( 0 );

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

void TrussGeometryWindow::addNodeToTable ( const Node& node )
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

    pivotTable->setNodesTotalNumber( focusWindow->getNodeList().size() );
}

void TrussGeometryWindow::showNodeTableRow ( bool visible )
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

void TrussGeometryWindow::removeNodeFromTable ( const Node& node )
{
    if ( sender() != focusWindow )
        return;

    nodeTable->removeRow( node.getNumber() - 1 );
    nodesNumbLabel->setText( "Total nodes: " + 
                      QString::number(focusWindow->getNodeList().size() - 1) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodeTable->getFixedNodesNumber()) );

    if ( ! nodeTable->numRows() ) {
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

    pivotTable->setNodesTotalNumber( focusWindow->getNodeList().size() - 1 );
}

void TrussGeometryWindow::updateNodeTableCoords ()
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        nodeTable->setCoord( row, 0, node.getX() );
        nodeTable->setCoord( row, 1, node.getY() );
    }
    catch ( ... ) { return; }
}

void TrussGeometryWindow::updateNodeTableFixation ()
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

void TrussGeometryWindow::updateNodeState ( int row, int col )
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

    if ( col != nodeTable->numCols() - 1 ) {
        // Save pos for Undo/Redo
        beforeMovingNodePos = node->getPoint();

        node->setPoint( nodeTable->text(row, 0).toDouble(),
              nodeTable->text(row, 1).toDouble() );

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

/****************************** pivots ***************************************/

void TrussGeometryWindow::fillPivotTable ()
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

void TrussGeometryWindow::addPivotToTable ( const Node& first, const Node& last )
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

void TrussGeometryWindow::removePivotFromTable ( const Node& first, 
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

void TrussGeometryWindow::updateNodesNumbers ( const Node& node )
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

void TrussGeometryWindow::showPivotTableRow ( bool visible )
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

void TrussGeometryWindow::updatePivotTableFirstNode ()
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

void TrussGeometryWindow::updatePivotTableLastNode ()
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

void TrussGeometryWindow::updatePivotTableThickness ()
{
    try { 
        const TrussPivot& pivot = dynamic_cast<const TrussPivot&>(*sender());
        pivotTable->setThickness( pivot.getNumber() - 1, 
                                   pivot.getThickness() );
    }
    catch ( ... ) { return; }
}

void TrussGeometryWindow::updatePivotState ( int row, int col )
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

void TrussGeometryWindow::changeTrussAreaSize ( const QString& newSizeStr )
{}

/****************************************************************************/
