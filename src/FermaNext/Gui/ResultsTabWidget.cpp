
#include <QFrame>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWheelEvent>

#include "ResultsTabWidget.h"
#include "HtmlTable.h"
#include "Geometry.h"
#include "TrussDeformPicture.h"

/*****************************************************************************
 * Results Tab
 *****************************************************************************/

ResultsTab::ResultsTab ( QWidget* parent /* = 0 */, Qt::WFlags f /* = 0 */ ) :
    QWidget( parent, f ),
    parentWidget( 0 ),
    deformWidget( 0 ),
    propertyTable( 0 ),
    stressTable( 0 ),
    dispTable( 0 )
{
    init();    
}

void ResultsTab::init ()
{
    ctrlWidget.setFixedHeight( 140 );
    colorCtrl.setFixedWidth( 70 );

    deformWidget = new TrussDeformPicture( colorCtrl );

    propertyTable = new HtmlPropertyTable( tr("Truss Properties") );
    propertyTable->setFixedSize( 190, 250 );
    propertyTable->setFrameStyle( QFrame::NoFrame );

    stressTable = new HtmlStressTable( tr("Element Stresses") );
    stressTable->setFrameStyle( QFrame::NoFrame );

    dispTable = new HtmlDisplacementTable( tr("Node Displacements") );
    dispTable->setFrameStyle( QFrame::NoFrame );

    QVBoxLayout* parentLayout = new QVBoxLayout;
    QHBoxLayout* topLayout = new QHBoxLayout;
    QVBoxLayout* topLeftLayout = new QVBoxLayout;
    QVBoxLayout* topRightLayout = new QVBoxLayout;
    QHBoxLayout* propertyLayout = new QHBoxLayout;
    QVBoxLayout* ctrlLayout = new QVBoxLayout;
    QVBoxLayout* bottomLayout = new QVBoxLayout;
    topLayout->addLayout( topLeftLayout );
    topLayout->addLayout( topRightLayout );
    topRightLayout->addSpacing( 20 );
    topRightLayout->addLayout( propertyLayout );
    topRightLayout->addSpacing( 10 );
    topRightLayout->addLayout( ctrlLayout );
    topRightLayout->setSpacing( 0 );
    parentLayout->addLayout( topLayout );
    parentLayout->addSpacing( 10 );
    parentLayout->addLayout( bottomLayout );
    parentLayout->setMargin( 15 );

    topLeftLayout->addWidget( deformWidget );
    propertyLayout->addWidget( &colorCtrl );
    propertyLayout->addSpacing( 10 );
    propertyLayout->addWidget( propertyTable );
    ctrlLayout->addWidget( &ctrlWidget );
    topRightLayout->setSizeConstraint( QLayout::SetMaximumSize );
    bottomLayout->addWidget( stressTable );
    bottomLayout->addWidget( dispTable );
    setLayout( parentLayout );

    deformWidget->setMinimumSize( Global::defaultCanvasWidth,
                                  Global::defaultCanvasHeight );

    connect( &ctrlWidget, SIGNAL( onScaleSliderValueChanged(double) ), 
             deformWidget, SLOT( changeScale(double) ) );
    connect( &ctrlWidget, SIGNAL( onDeformSliderValueChanged(double) ), 
             deformWidget, SLOT( changeDeform(double) ) );
    connect( &ctrlWidget, SIGNAL( onNumberCheckBoxValueChanged(bool) ), 
             deformWidget, SLOT( showNumbers(bool) ) );
    connect( &ctrlWidget, SIGNAL( onDeformCheckBoxValueChanged(bool, double) ), 
             deformWidget, SLOT( showDeform(bool, double) ) );
    connect( &ctrlWidget, SIGNAL(onSwitchLoadCase(int)),
              deformWidget, SLOT(switchLoadCaseResults(int)) );
    connect( deformWidget, SIGNAL(displacementLoaded(bool)),
                &ctrlWidget, SLOT(setDeformSliderEnabled(bool)) );
}

void ResultsTab::fillTab( const PluginResults& pluginResults,
                          TrussUnitCopy& trussCopy )
{
    propertyTable->setHtml( propertyTable->getDomDocument().toString().
                             arg( trussCopy.countNodes() ).
                             arg( trussCopy.countPivots() ).
                             arg( trussCopy.countLoadCases() ).
                             arg( trussCopy.countMaterials() ).
                             arg( "unknown" ).
                             arg( "unknown" ).
                             arg( "unknown" ).
                             arg( "unknown" ).
                             arg( "unknown" ).
                             arg( "unknown" ).
                             arg( "unknown" ) );
    stressTable->clear();
    stressTable->updateTable( pluginResults, trussCopy.getPivotList() );
    dispTable->updateTable( pluginResults, trussCopy.getNodeList() );
    deformWidget->fill( trussCopy, pluginResults );
    ctrlWidget.fillLoadCaseComboBox( trussCopy.countLoadCases() );
    ctrlWidget.initControlsState();
}
    
/*****************************************************************************
 * Results Tab Widget
 *****************************************************************************/

ResultsTabWidget::ResultsTabWidget ( QWidget* parent /* = 0 */ ) :
    QTabWidget( parent )
{}

ResultsTabWidget::~ResultsTabWidget ()
{
    delete scrollArea;
}

ResultsTab* ResultsTabWidget::getPluginTab ( int indx )
{
    QScrollArea* scrollArea = dynamic_cast<QScrollArea*>( widget( indx ) );
    if ( scrollArea )
        return dynamic_cast<ResultsTab*>( scrollArea->widget() );
    return 0;
}

void ResultsTabWidget::addPluginTab ( const PluginResults& results,
                                      TrussUnitCopy& trussCopy )
{
    ResultsTab* newPage = new ResultsTab;
    scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidget( newPage );
    scrollArea->viewport()->setBackgroundRole( QPalette::Base );
    scrollArea->viewport()->setAutoFillBackground( true );
    addTab( scrollArea, results.getPluginName() );
    newPage->fillTab( results, trussCopy );
}

void ResultsTabWidget::removePluginTab ( int indx )
{
    ResultsTab* resultsPage = dynamic_cast<ResultsTab*>( widget( indx ) );
    if ( ! resultsPage )
        return;
    delete resultsPage;
    removeTab( indx );
}

void ResultsTabWidget::setTrussSolutionResults( 
                               const TrussSolutionResults& trussResults )
{
    // remove unnecessary plugin tabs
    if ( trussResults.countResults() < count() )
        for ( int i = trussResults.countResults(); i < count(); ++i )
            removePluginTab( i );
    
    // update other plugin tabs
    for ( int i = 0; i < trussResults.countResults(); ++i ) {
        const PluginResults* res = trussResults.getPluginResults( i );
        if ( ! res )
            continue;
        TrussUnitCopy& trussCopy = trussResults.getTrussUnitCopy();
        ResultsTab* tab = getPluginTab( i );
        if ( tab ) {
            setTabText( i, res->getPluginName() );
            tab->fillTab( *res, trussCopy );
        }
        else
            addPluginTab( *res, trussCopy );
    }
}

QSize ResultsTabWidget::sizeHint () const
{
    QSize size = QTabWidget::sizeHint();
    size.setHeight( 700 ); 
    size.setWidth( 600 ); 
    return size;
}

/*****************************************************************************/
