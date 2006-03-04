
#include "TrussGeometryWindow.h"

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfont.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qpalette.h> 
#include <qrect.h>
#include <qstyle.h>

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
    ((FixationItem*)this)->fixCheckBox = 
        new DoubleCheckBox( "X", "Y", table()->viewport() );
    fixCheckBox->setFirstChecked( xChecked );
    fixCheckBox->setSecondChecked( yChecked );
    fixCheckBox->setPaletteBackgroundColor( table()->viewport()->backgroundColor() );
    // connect( fixCheckBox, SIGNAL(onValueChange()), 
    //         table(), SLOT(doValueChanged()) );
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
    table()->style().drawPrimitive( QStyle::PE_Indicator, p, primArea, c, flags );

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

NodesTable::NodesTable ( QWidget* parent, const char* name ) :
    QTable( parent, name ),
    areaSize( 0, 0 )
{}

void NodesTable::setCoord ( int row, int col, double coord )
{
    setText( row, col, QString("%1").arg( coord,0,'f',2 ) );
}

double NodesTable::getCoord ( int row, int col ) const
{
    return text( row, col ).toDouble();
}

void NodesTable::setFixationItem ( int row, int col, Node::Fixation fix )
{
    FixationItem* fixCell = getFixationItem( row, col );
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

FixationItem* NodesTable::getFixationItem ( int row, int col ) const
{
    try {
        return dynamic_cast<FixationItem*>( item(row, col) );
    } catch ( ... ) { return 0; }
}

int NodesTable::getFixedNodesNumber () const
{
    int fixedNumb = 0;
    FixationItem* item = 0;
    for ( uint i = 0; i < numRows(); ++i ) {
        item = getFixationItem( i, numCols() - 1 );
        if ( item && item->getFixation() != Node::Unfixed )
            ++fixedNumb;
    }
    return fixedNumb;
}

void NodesTable::addNode ( const Node& node )
{
    int row = node.getNumber() - 1;
    insertRows( row );
    setRowHeight( row, tableRowHeight );
    setCoord( row, 0, node.getX() );
    setCoord( row, 1, node.getY() );
    setFixationItem( row, numCols() - 1, node.getFixation() );
    //updateMaximumHeight();
}

void NodesTable::updateMaximumHeight ()
{
    setMaximumHeight( numRows() * tableRowHeight + lineWidth() * 2 +
                      horizontalHeader()->height() );
}

QWidget* NodesTable::createEditor ( int row, int col, bool initFromCell ) const
{
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

void NodesTable::updateTrussAreaSize ( const DoubleSize& newSize )
{
    areaSize = newSize;
}

/*****************************************************************************
 * Combo Item
 *****************************************************************************/

ComboItem::ComboItem( QTable* table, int currentNumb ) : 
    QTableItem( table, WhenCurrent, QString::number( currentNumb ) ), 
    comboBox( 0 ),
    currentValue( currentNumb )
{
    setReplaceable( false );
}

QWidget* ComboItem::createEditor() const
{
    ( (ComboItem*)this )->comboBox = new QComboBox( table()->viewport() );
    QObject::connect( comboBox, SIGNAL( activated( int ) ), 
                      table(), SLOT( doValueChanged() ) );
    try {
        uint nodesNumb =  dynamic_cast<PivotsTable*>(table())->getNodesNumber();
        for ( uint i = 0; i < nodesNumb; ++i )
            comboBox->insertItem( QString::number(i + 1) );
        comboBox->setCurrentItem( currentValue - 1 );
        return comboBox;
    } catch ( ... ) { return 0; }
}

void ComboItem::setContentFromEditor( QWidget *w )
{
    // the user changed the value of the combobox, so synchronize the
    // value of the item (its text), with the value of the combobox
    if ( w->inherits( "QComboBox" ) )
	    setText( ( (QComboBox*)w )->currentText() );
    else
	    QTableItem::setContentFromEditor( w );
}

void ComboItem::setText( const QString &s )
{
    if ( comboBox )
	    // initialize the combobox from the text
        comboBox->setCurrentItem( s.toInt() - 1 );
    QTableItem::setText( s );
}

/*****************************************************************************
 * Pivots Table
 *****************************************************************************/

PivotsTable::PivotsTable ( QWidget* parent, const char* name ) :
    QTable( parent, name ),
    nodesNumb( 0 )
{}

void PivotsTable::setThickness ( int row, int col, double thick )
{
    setText( row, col, QString("%1").arg( thick,0,'f',2 ) );
}

double PivotsTable::getThickness ( int row, int col ) const
{
    return text( row, col ).toDouble();
}

void PivotsTable::setNodesNumber ( int newNodesNumb )
{
    nodesNumb = newNodesNumb;
}

int PivotsTable::getNodesNumber () const
{
    return nodesNumb;
}

void PivotsTable::addPivot ( const TrussPivot& pivot )
{
    int row = pivot.getNumber() - 1;
    insertRows( row );
    setRowHeight( row, tableRowHeight );
    Node& first = pivot.getFirstNode();
    Node& last = pivot.getLastNode();
	setItem( row, 0, new ComboItem(this, first.getNumber()) );
	setItem( row, 1, new ComboItem(this, last.getNumber()) );
    setThickness( row, numCols() - 1, pivot.getThickness() );
}

QWidget* PivotsTable::createEditor ( int row, int col, bool initFromCell ) const
{
    if ( col != numCols() - 1 )
        return item(row, col)->createEditor();

    QLineEdit* editor = new QLineEdit( viewport() );
    editor->setFrame( false );
    editor->setValidator( new RangeValidator( 0.0, pivotThickLimit, 2, editor ) );
    editor->setMaxLength( 6 );
    if ( initFromCell )
        editor->setText( text( row, col ) );
    return editor;
}

/*****************************************************************************
 * Truss Geometry Window
 *****************************************************************************/

TrussGeometryWindow::TrussGeometryWindow ( QWidget* p, const char* n, WFlags f ) :
    QTabWidget( p, n, f ),
    nodesTable( 0 ),
    pivotsTable( 0 ),
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
    setMinimumSize( 160, 138 );
    setMaximumWidth( 195 );
    setMaximumHeight( 250 );
    initNodesTab();
    initPivotsTab();
    addTab( new QLabel( tr( "" ), this ), "Area" );
}

void TrussGeometryWindow::initNodesTab ()
{
    QFrame* parentFrame = new QFrame( this, "nodes_frame",
                                      QFrame::Panel | QFrame::Plain );
    nodesNumbLabel = new QLabel( tr( "Total nodes: " ), parentFrame );
    fixedNodesLabel = new QLabel( tr( "Nodes fixed: " ), parentFrame );
    nodesTable = new NodesTable( parentFrame );

    // init layout managers
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget( nodesNumbLabel );
    topLayout->addWidget( fixedNodesLabel );
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget( nodesTable );
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
    nodesTable->setNumCols( 3 );
    nodesTable->setSelectionMode( QTable::NoSelection );
    QHeader *horHeader = nodesTable->horizontalHeader(),
            *vertHeader = nodesTable->verticalHeader();
    horHeader->setLabel( 0, tr( "X" ) );
    horHeader->setLabel( 1, tr( "Y" ) );
    horHeader->setLabel( 2, tr( "Fixations" ) );
    horHeader->setClickEnabled ( false );
    horHeader->setResizeEnabled ( false );
    vertHeader->setClickEnabled ( false );
    vertHeader->setResizeEnabled ( false );
    nodesTable->setVScrollBarMode( QScrollView::AlwaysOn );
    nodesTable->setColumnWidth( 0, 40 );
    nodesTable->setColumnWidth( 1, 40 );
    nodesTable->setColumnWidth( 2, 63 );
    nodesTable->setLeftMargin( 22 );
    nodesTable->hide();
    connect( nodesTable, SIGNAL(valueChanged(int, int)),
                        SLOT(updateNodeState(int, int)) );

    addTab( parentFrame, "Nodes" );
}

void TrussGeometryWindow::initPivotsTab ()
{
    QFrame* parentFrame = new QFrame( this, "pivots_frame",
                                      QFrame::Panel | QFrame::Plain );
    pivotsNumbLabel = new QLabel( tr( "Total pivots: " ), parentFrame );
    pivotsTable = new PivotsTable( parentFrame );

    // init layout managers
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget( pivotsNumbLabel );
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget( pivotsTable );
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
    pivotsTable->setNumRows( 3 );
    pivotsTable->setNumCols( 3 );
    pivotsTable->setSelectionMode( QTable::NoSelection );
    QHeader *horHeader = pivotsTable->horizontalHeader(),
            *vertHeader = pivotsTable->verticalHeader();
    horHeader->setLabel( 0, tr( "Node1" ) );
    horHeader->setLabel( 1, tr( "Node2" ) );
    horHeader->setLabel( 2, tr( "Thickness" ) );
    horHeader->setClickEnabled ( false );
    horHeader->setResizeEnabled ( false );
    vertHeader->setClickEnabled ( false );
    vertHeader->setResizeEnabled ( false );
    pivotsTable->setVScrollBarMode( QScrollView::AlwaysOn );
    pivotsTable->setColumnWidth( 0, 40 );
    pivotsTable->setColumnWidth( 1, 40 );
    pivotsTable->setColumnWidth( 2, 63 );
    pivotsTable->setLeftMargin( 22 );
    pivotsTable->hide();

    addTab( parentFrame, "Pivots" );
}

void TrussGeometryWindow::changeFocusWindow ( TrussUnitWindow* newFocusWindow )
{
    focusWindow = newFocusWindow;
    if ( focusWindow ) {
        fillNodesTable();
        fillPivotsTable();
        return;
    }
    nodesTable->hide();
    pivotsTable->hide();
    nodesSpacer->changeSize( 0, nodesTable->height() );
    pivotsSpacer->changeSize( 0, nodesTable->height() );
    nodesNumbLabel->setText( "Total nodes: " );
    pivotsNumbLabel->setText( "Total pivots: " );
    fixedNodesLabel->setText( "Fixed nodes: " );
}

void TrussGeometryWindow::fillNodesTable ()
{
    // clear table
    nodesTable->setNumRows( 0 );

    TrussUnit::NodeList nodeList = focusWindow->getNodeList();
    if ( nodeList.empty() ) {
        nodesTable->hide();
        nodesSpacer->changeSize( 0, nodesTable->height() );
        nodesNumbLabel->setText( "Total nodes: 0" );
        fixedNodesLabel->setText( "Fixed nodes: 0" );
        return;
    }
    nodesSpacer->changeSize( 0, 0 );
    nodesTable->show();

    TrussUnit::NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
        nodesTable->addNode( (const Node&)**iter );

    nodesNumbLabel->setText( "Total nodes: " + 
                             QString::number(nodeList.size()) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodesTable->getFixedNodesNumber()) );
}

void TrussGeometryWindow::fillPivotsTable ()
{
    // clear table
    pivotsTable->setNumRows( 0 );

    TrussUnit::PivotList pivotList = focusWindow->getPivotList ();
    if ( pivotList.empty() ) {
        pivotsTable->hide();
        pivotsSpacer->changeSize( 0, pivotsTable->height() );
        pivotsNumbLabel->setText( "Total pivots: 0" );
        return;
    }
    pivotsSpacer->changeSize( 0, 0 );
    pivotsTable->show();

    TrussUnit::PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
        pivotsTable->addPivot( (const TrussPivot&)**iter );

    pivotsNumbLabel->setText( "Total pivots: " + 
                              QString::number(pivotList.size()) );
}

void TrussGeometryWindow::closeEvent ( QCloseEvent *event )
{
    emit onGeometryWindowClose();
}

void TrussGeometryWindow::trussUnitWindowWasCreated ( TrussUnitWindow& window )
{

    connect( &window, SIGNAL(afterNodeCreation(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );

    connect( &window, SIGNAL(afterNodeRevive(const Node&)),
                      SLOT(addNodeToTable(const Node&)) );
                    
    connect( &window, SIGNAL(beforeNodeDesist(Node&)),
                      SLOT(removeNodeFromTable(Node&)) );

    connect( &window, SIGNAL(onAreaChange(const DoubleSize&)),
             nodesTable, SLOT(updateTrussAreaSize(const DoubleSize&)) );

    connect( &window, SIGNAL(afterPivotCreation(const Node&, const Node&)),
                      SLOT(addPivotToTable(const Node&, const Node&)) );
/*
    connect( &window, SIGNAL(afterPivotRevive(Node&, Node&)),
                      SLOT(addPivotToTable(Node&, Node&)) );
                    
    connect( &window, SIGNAL(beforePivotDesist(Node&, Node&)),
                      SLOT(removePivotFromTable(Node&, Node&)) ); */

    changeFocusWindow( &window );
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

void TrussGeometryWindow::addNodeToTable ( const Node& node )
{
    if ( sender() != focusWindow )
        return;

    try { 
        const TrussNode& n = dynamic_cast<const TrussNode&>(node);
        connect( &n, SIGNAL(onVisibleChange(bool)),
                     SLOT(setNodeTableRowVisible(bool)) );
    }
    catch ( ... ) { return; }

    if ( nodesTable->isHidden() ) {
        nodesTable->show();
        nodesSpacer->changeSize( 0, 0 );
    }

    nodesTable->addNode( node );
    nodesNumbLabel->setText( "Total nodes: " + 
                          QString::number(focusWindow->getNodeList().size()) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                          QString::number(nodesTable->getFixedNodesNumber()) );

    connect( &node, SIGNAL(onPositionChange(double, double)),
                    SLOT(updateNodeTableCoords()) );
    connect( &node, SIGNAL(onFixationChange(Fixation)),
                    SLOT(updateNodeTableFixation()) );

    pivotsTable->setNodesNumber( focusWindow->getNodeList().size() );
}

void TrussGeometryWindow::addPivotToTable ( const Node& first, const Node& last )
{
    if ( sender() != focusWindow )
        return;

    if ( pivotsTable->isHidden() ) {
        pivotsTable->show();
        pivotsSpacer->changeSize( 0, 0 );
    }
    try { 
        const TrussNode& node1 = dynamic_cast<const TrussNode&>(first);
        const TrussNode& node2 = dynamic_cast<const TrussNode&>(last);
        pivotsTable->addPivot( *focusWindow->findPivotByNodes( node1, node2) );
        pivotsTable->setNodesNumber( focusWindow->getNodeList().size() );
        pivotsNumbLabel->setText( "Total pivots: " + 
                         QString::number(focusWindow->getPivotList().size()) );
    } catch ( ... ) { return; }
}

void TrussGeometryWindow::setNodeTableRowVisible ( bool visible )
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        if ( visible ) {
            nodesTable->showRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size()) );
            //nodesTable->updateMaximumHeight();
        }
        else {
            nodesTable->hideRow( row );
            nodesNumbLabel->setText( "Total nodes: " + 
                QString::number(focusWindow->getNodeList().size() - 1) );
            QHeader* horHeader = nodesTable->horizontalHeader();
            /*nodesTable->setMaximumHeight( ( nodesTable->numRows() - 1 ) * 
                                         tableRowHeight + 
                                         horHeader->height() + 
                                         nodesTable->lineWidth() * 2 );*/
        }
    }
    catch ( ... ) { return; }
}

void TrussGeometryWindow::removeNodeFromTable ( Node& node )
{
    if ( sender() != focusWindow )
        return;

    nodesTable->removeRow( node.getNumber() - 1 );
    nodesNumbLabel->setText( "Total nodes: " + 
                      QString::number(focusWindow->getNodeList().size() - 1) );
    fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodesTable->getFixedNodesNumber()) );

    if ( ! nodesTable->numRows() ) {
        nodesSpacer->changeSize( 0, nodesTable->height() );
        nodesTable->hide();
    }
    //nodesTable->updateMaximumHeight();

    // prevent double connection after revive
    disconnect( &node, SIGNAL(onPositionChange(double, double)),
                this,  SLOT(updateNodeTableCoords()) );
    disconnect( &node, SIGNAL(onFixationChange(Fixation)),
                this,  SLOT(updateNodeTableFixation()) );

    pivotsTable->setNodesNumber( focusWindow->getNodeList().size() );
}

void TrussGeometryWindow::updateNodeTableCoords ()
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1;
        nodesTable->setCoord( row, 0, node.getX() );
        nodesTable->setCoord( row, 1, node.getY() );
    }
    catch ( ... ) { return; }
}

void TrussGeometryWindow::updateNodeTableFixation ()
{
    try { 
        const Node& node = dynamic_cast<const Node&>(*sender());
        int row = node.getNumber() - 1,
            col = nodesTable->numCols() - 1;
        nodesTable->setFixationItem( row, col, node.getFixation() );
        fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodesTable->getFixedNodesNumber()) );
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

    if ( col != nodesTable->numCols() - 1 ) {
        // Save pos for Undo/Redo
        beforeMovingNodePos = node->getPoint();

        node->setPoint( nodesTable->text(row, 0).toDouble(),
              nodesTable->text(row, 1).toDouble() );

        ObjectStateManager* mng = focusWindow->getStateManager();
        mng->startStateBlock();
        // Save state in Undo/Redo stack
        saveNodeStateAfterMoving( *node, node->getPoint() );
        focusWindow->updateAfterNodeManipulation ( node, true );
        mng->endStateBlock();
    }
    else {
        FixationItem* item = nodesTable->getFixationItem( row, col );
        if ( item ) {
            node->setFixation( item->getFixation() );
            fixedNodesLabel->setText( "Fixed nodes: " + 
                      QString::number(nodesTable->getFixedNodesNumber()) );
        }
    }
}

/****************************************************************************/
