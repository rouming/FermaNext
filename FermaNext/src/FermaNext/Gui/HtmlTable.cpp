
#include <QStringList>
#include <QWheelEvent>

#include "Geometry.h"
#include "HtmlTable.h"
#include "TrussSolutionResults.h"

/*****************************************************************************
 * HTML Table
 *****************************************************************************/

HtmlTable::HtmlTable ( const QString& title, QWidget* parent /* = 0 */ ) :
    QTextBrowser( parent )
{
    init( title );
}

HtmlTable::~HtmlTable ()
{}

void HtmlTable::init ( const QString& title )
{
    rootTable = htmlDoc.createElement( "table" );
    rootTable.setAttribute( "border", 1 );
    rootTable.setAttribute( "cellpadding", 1 );
    rootTable.setAttribute( "cellspacing", 0 );
    rootTable.setAttribute( "bordercolor", Global::htmlBorderColor );
    rootTable.setAttribute( "align", "center" );
    htmlDoc.appendChild( rootTable );
               
    QDomElement header = htmlDoc.createElement( "tr" );
    rootTable.appendChild( header );
    QDomElement headerBody = htmlDoc.createElement( "td" );
    headerBody.setAttribute( "bgcolor", Global::htmlTitleBgColor );
    headerBody.setAttribute( "align", "center" );
    headerBody.setAttribute( "style", "font-weight:600; font-size:14px" );
    header.appendChild( headerBody );
    headerBody.appendChild( htmlDoc.createTextNode( title ) );

    QDomElement data = htmlDoc.createElement( "tr" );
    rootTable.appendChild( data );

    QDomElement dataBody = htmlDoc.createElement( "td" );
    data.appendChild( dataBody );

    childTable = htmlDoc.createElement( "table" );
    childTable.setAttribute( "border", 1 );
    childTable.setAttribute( "cellpadding", 1 );
    childTable.setAttribute( "cellspacing", 0 );
    childTable.setAttribute( "bordercolor", Global::htmlBorderColor );
    childTable.setAttribute( "align", "center" );
    dataBody.appendChild( childTable );
}

const QDomDocument& HtmlTable::getDomDocument () const
{
    return htmlDoc;
}

void HtmlTable::fillCell ( QDomElement& elem, const QString& text, 
                           const QString& color, const QString& align,
                           int width, int height, int colspan )
{
    QDomElement col = htmlDoc.createElement( "td" );
    col.setAttribute( "width", width );
    col.setAttribute( "colspan", colspan );
    col.setAttribute( "height", height );
    col.setAttribute( "bgcolor", color );
    col.setAttribute( "align", align );
    col.appendChild( htmlDoc.createTextNode( text ) );
    elem.appendChild( col );
}

void HtmlTable::addRow ( const HtmlTableCellList& cells )
{
    QDomElement cellElement = htmlDoc.createElement( "tr" );
    childTable.appendChild( cellElement );
        
    HtmlTableCellList::const_iterator iter;
    for ( iter = cells.begin(); iter != cells.end(); ++iter ) {
        HtmlTableCell cell = *iter;
        fillCell( cellElement, cell.text, cell.bgColor, cell.align,
                  cell.width, cell.height, cell.colspan );
    }
}

void HtmlTable::resizeRootTable ( int newWidth, int newHeight /* = -1 */ )
{
    rootTable.setAttribute( "width", newWidth );
    if ( newHeight != -1 )
        rootTable.setAttribute( "height", newHeight );
}

void HtmlTable::clearData ()
{
    QDomNodeList rows = childTable.elementsByTagName( "tr" );
    int numb = rows.count();
    for ( int i = 0; i < numb; ++i ) {
        QDomNode elem = childTable.firstChild();
        childTable.removeChild( elem );
    }
}

QDomElement HtmlTable::getRowElement ( int row ) const
{
    QDomNodeList rows = childTable.elementsByTagName( "tr" );
    QDomNode rowNode;
    for ( int i = 0; i < rows.count(); ++i ) {
        rowNode = rows.item( i );
        if ( i == row )
            break;
    }

    return rowNode.toElement();
}

QDomElement HtmlTable::getColumnElement ( int col, const QDomElement& rowElem ) const
{
    QDomNode colNode;
    QDomNodeList cols = rowElem.elementsByTagName( "td" );
    for ( int i = 0; i < cols.count(); ++i ) {
        colNode = cols.item( i );
        if ( i == col )
            break;
    }

    return colNode.toElement();
}

void HtmlTable::setCellHighlight ( int row, int col, const QString& color )
{
    QDomElement colElem = getColumnElement( col, getRowElement( row ) );

    colElem.setAttribute( "bgcolor", color );
}

void HtmlTable::setColumnHighlight ( int col, const QString& color )
{
    QDomNodeList rows = childTable.elementsByTagName( "tr" );
    // Start with 2 to ignore table headers
    for ( int i = 2; i < rows.count(); ++i ) {
        QDomElement colElem = 
                getColumnElement( col, getRowElement( i ) );
        colElem.setAttribute( "bgcolor", color );
    }
}

void HtmlTable::removeHighlight ()
{
    QDomNodeList rows = childTable.elementsByTagName( "tr" );
    // Start with 2 to ignore table headers
    for ( int i = 2; i < rows.count(); ++i ) {
        QDomNode rowNode = rows.item( i );
        if ( ! rowNode.isElement() )
            continue;
        QDomElement rowElem = rowNode.toElement();
        QDomNodeList cols = rowElem.elementsByTagName( "td" );
        for ( int j = 0; j < cols.count(); ++j ) {
            QDomNode colNode = cols.item( j );
            if ( ! colNode.isElement() )
                continue;
            QDomElement colElem = colNode.toElement();
            colElem.setAttribute( "bgcolor", "" );
        }
    }

}

int HtmlTable::getHeight () const
{
    int height = 0;
    QDomNodeList rows = childTable.elementsByTagName( "tr" );
    for ( int i = 0; i < rows.count(); ++i ) {
        QDomNode rowNode = rows.item( i );
        if ( ! rowNode.isElement() )
            continue;
        
        QDomElement rowElem = rowNode.toElement();
        QDomElement colElem = getColumnElement( 0, rowElem );

        if ( ! colElem.hasAttribute( "height" ) )
            continue;

        height += colElem.attribute( "height" ).toInt();
    }

    // Increase by top level header width
    height += 20;
    return height; 
}

void HtmlTable::updateHtml ()
{
    setHtml( htmlDoc.toString() );
}

void HtmlTable::wheelEvent ( QWheelEvent* )
{}

/*****************************************************************************
 * HTML Property Table
 *****************************************************************************/

HtmlPropertyTable::HtmlPropertyTable ( const QString& title, 
                                       QWidget* parent /* = 0 */ ) :
    HtmlTable( title, parent )
{
    fillTable();
}

void HtmlPropertyTable::addProperty ( const QString& prop, int value )
{
    HtmlTableCellList cells;
    cells.push_back( HtmlTableCell( prop, 
                     Global::htmlSubHeaderBgColor, "left", 1, 110 ) );
    cells.push_back( HtmlTableCell( "%" + QString::number(value), 
                     "", "center", 1, 55 ) );
    addRow( cells );
}

void HtmlPropertyTable::fillTable ()
{
    uint i = 0;
    addProperty( tr("Node number"), ++i );
    addProperty( tr("Element number"), ++i );
    addProperty( tr("Load case number"), ++i );
    addProperty( tr("Material number"), ++i );
    addProperty( tr("Max compression"), ++i );
    addProperty( tr("Max tension stress"), ++i );
    addProperty( tr("Force weight"), ++i );
    addProperty( tr("Opt. force weight"), ++i );
    addProperty( tr("Force weight koeff."), ++i );
    addProperty( tr("Material volume"), ++i );
    addProperty( tr("Truss mass"), ++i );
    resizeRootTable( 176 );
}

/*****************************************************************************
 * HTML Stress Table
 *****************************************************************************/

HtmlStressTable::HtmlStressTable ( const QString& title, 
                                   QWidget* parent /* = 0 */ ) :
    HtmlTable( title, parent ),
    pluginResults( 0 )
{}

void HtmlStressTable::updateTable ( const PluginResults& res, 
                              TrussUnitCopy::TrussCopyPivotList pivotList )
{
    pluginResults = &res;
    pivots = pivotList;
    clearData();
    fillTable();
    setHtml( getDomDocument().toString() );
}

void HtmlStressTable::fillTable ()
{
    // default values
    QString col = Global::htmlHeaderBgColor;
    int w = Global::htmlColWidth;

    //******* create first level headers
    HtmlTableCellList cells;
    loadCaseNumb = pluginResults->countLoadCaseResults();
    cells.push_back( HtmlTableCell( "", col, "center", 2 ) );
    cells.push_back( HtmlTableCell( tr( "Stresses" ), col, 
                                    "center", loadCaseNumb ) );
    cells.push_back( HtmlTableCell( tr( "Margin of safety coeff." ), col, 
                                    "center", loadCaseNumb ) );
    cells.push_back( HtmlTableCell( tr( "Required thickness" ), col, 
                                    "center", loadCaseNumb ) );
    addRow( cells );
    
    //******* create second level headers
    // clear first level headers
    cells.clear();

    col = Global::htmlSubHeaderBgColor;
    cells.push_back( HtmlTableCell( tr( "Element" ), col ) );
    cells.push_back( HtmlTableCell( tr( "Thick." ), col ) );

    // load case columns 
    for ( int i = 0; i < 3; ++i )
        for ( int j = 1; j <= loadCaseNumb; ++j )
            cells.push_back( HtmlTableCell( tr( "Case" ) + " " + 
                             QString::number(j), col) );
    addRow( cells );
    
    resizeRootTable( w * loadCaseNumb * 3 + w * 2 + loadCaseNumb * 6 + 10 );
    
    //******* fill table
    uint i = 0;
    TrussUnitCopy::TrussCopyPivotListIter pIter = pivots.begin();
    for ( ; pIter < pivots.end(); ++pIter ) {
        TrussCopyPivot* p = *pIter;
        cells.clear();
        cells.push_back( HtmlTableCell( QString::number( i + 1 ) ) );
        cells.push_back( HtmlTableCell( 
                         QString::number( p->getThickness(), 'e', 2 ), 
                         "", "right") );

        //------ fill with stresses
        for ( int j = 1; j <= loadCaseNumb; ++j ) {
            const LoadCaseResults* res = pluginResults->getLoadCaseResults( j );
            if ( ! res )
                cells.push_back( HtmlTableCell() );
            else {
                bool valid;
                double stress = res->getStress( i, valid );
                if ( valid )
                    cells.push_back( HtmlTableCell( 
                        QString::number( stress, 'e', 2 ), "", "right") );
                else
                    cells.push_back( HtmlTableCell() );
            }
        }

        //------ fill with margin of safety coefficient
        // will be added soon
        for ( int j = 1; j <= loadCaseNumb; ++j ) {
            const LoadCaseResults* res = pluginResults->getLoadCaseResults( j );
            if ( ! res )
                cells.push_back( HtmlTableCell() );
            else {
                bool valid;
                double safetyMargin = res->getSafetyMargin( i, valid );
                if ( valid )
                    cells.push_back( HtmlTableCell( 
                        QString::number( safetyMargin, 'e', 2 ), "", "right") );
                else
                    cells.push_back( HtmlTableCell() );
            }
        }

        //------ fill with required thickness values
        for ( int j = 1; j <= loadCaseNumb; ++j ) {
            const LoadCaseResults* res = pluginResults->getLoadCaseResults( j );
            if ( ! res )
                cells.push_back( HtmlTableCell() );
            else {
                bool valid;
                double reqThick = res->getRequiredThickness( i, valid );
                if ( valid )
                    cells.push_back( HtmlTableCell( 
                        QString::number( reqThick, 'e', 2 ), "", "right") );
                else
                    cells.push_back( HtmlTableCell() );
            }
        }
        addRow( cells );
        ++i;
    }
}

void HtmlStressTable::changeLoadCaseHighlight ( int currLoadCase )
{
    if ( loadCaseNumb == 1 )
        return;

    QString col = Global::htmlCellHighlightColor;
    int firstLoadCaseColNum = currLoadCase + 2;
    removeHighlight();
    setColumnHighlight( firstLoadCaseColNum, col );
    setColumnHighlight( firstLoadCaseColNum + loadCaseNumb, col );
    setColumnHighlight( firstLoadCaseColNum + loadCaseNumb * 2, col );
    updateHtml();
}

/*****************************************************************************
 * HTML Displacement Table
 *****************************************************************************/

HtmlDisplacementTable::HtmlDisplacementTable ( const QString& title, 
                                               QWidget* parent /* = 0 */ ) :
    HtmlTable( title, parent ),
    pluginResults( 0 )
{}

void HtmlDisplacementTable::updateTable ( const PluginResults& res, 
                               TrussUnitCopy::TrussCopyNodeList nodeList )
{
    pluginResults = &res;
    nodes = nodeList;
    clearData();
    fillTable();
    setHtml( getDomDocument().toString() );
}

void HtmlDisplacementTable::fillTable ()
{
    // default values
    QString col = Global::htmlHeaderBgColor;
    int w = Global::htmlColWidth;

    //******* create first level headers
    HtmlTableCellList cells;
    loadCaseNumb = pluginResults->countLoadCaseResults();
    QString dispXTitle, dispYTitle;
    if ( loadCaseNumb == 1 ) {
        dispXTitle = QString( tr("X-Disp.") );
        dispYTitle = QString( tr("Y-Disp.") );
    }
    else  {
        dispXTitle = QString( tr("X-Displacement") );
        dispYTitle = QString( tr("Y-Displacement") );
    }

    cells.push_back( HtmlTableCell( "", col, "center" ) );
    cells.push_back( HtmlTableCell( tr( "Node Coords" ), col, 
                                    "center", 2 ) );
    cells.push_back( HtmlTableCell( dispXTitle, col, 
                                    "center", loadCaseNumb ) );
    cells.push_back( HtmlTableCell( dispYTitle, col, 
                                    "center", loadCaseNumb ) );
    addRow( cells );
    
    //******* create second level headers
    // clear first level headers
    cells.clear();

    col = Global::htmlSubHeaderBgColor;
    cells.push_back( HtmlTableCell( tr( "Node" ), col ) );
    cells.push_back( HtmlTableCell( "X", col ) );
    cells.push_back( HtmlTableCell( "Y", col ) );

    // load case columns 
    for ( int i = 0; i < 2; ++i )
        for ( int j = 1; j <= loadCaseNumb; ++j )
            cells.push_back( HtmlTableCell("Case " + QString::number(j), col) );
    addRow( cells );
    
    resizeRootTable( w * loadCaseNumb * 2 + w * 3 + loadCaseNumb * 4 + 12 );

    //******* fill table with stress values
    uint i = 0;
    TrussUnitCopy::TrussCopyNodeListIter nIter = nodes.begin();
    for ( ; nIter < nodes.end(); ++nIter ) {
        TrussCopyNode* n = *nIter;
        cells.clear();

        // fill table with node number
        cells.push_back( HtmlTableCell( QString::number( i + 1 ) ) );

        // fill table with coords
        cells.push_back( HtmlTableCell( QString::number( n->getBaseCoord().x(), 
                         'e', 2 ), "", "right") );
        cells.push_back( HtmlTableCell( QString::number( n->getBaseCoord().y(), 
                         'e', 2 ), "", "right") );

        // fill table with X-displacements
        for ( int j = 1; j <= loadCaseNumb; ++j ) {
            const LoadCaseResults* res = pluginResults->getLoadCaseResults( j );
            if ( ! res )
                cells.push_back( HtmlTableCell() );
            else {
                bool valid;
                DoublePoint disp = res->getDisplacement( i, valid );
                if ( valid ) 
                    cells.push_back( HtmlTableCell( 
                        QString::number( disp.x(), 'e', 2 ), "", "right") );
                else
                    cells.push_back( HtmlTableCell() );
            }
        }
        for ( int j = 1; j <= loadCaseNumb; ++j ) {
            const LoadCaseResults* res = pluginResults->getLoadCaseResults( j );
            if ( ! res )
                cells.push_back( HtmlTableCell() );
            else {
                bool valid;
                DoublePoint disp = res->getDisplacement( i, valid );
                if ( valid )
                    cells.push_back( HtmlTableCell( 
                        QString::number( disp.y(), 'e', 2 ), "", "right") );
                else
                    cells.push_back( HtmlTableCell() );
            }
        }
        addRow( cells );
        ++i;
    }
}

void HtmlDisplacementTable::changeLoadCaseHighlight ( int currLoadCase )
{
    if ( loadCaseNumb == 1 )
        return;
    
    QString col = Global::htmlCellHighlightColor;
    int firstLoadCaseColNum = currLoadCase + 3;
    removeHighlight();
    setColumnHighlight( firstLoadCaseColNum, col );
    setColumnHighlight( firstLoadCaseColNum + loadCaseNumb, col );
    updateHtml();
}


