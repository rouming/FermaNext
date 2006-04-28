
#include "TrussUnit.h"
#include "TrussUnitActions.h"
#include <algorithm>

/*****************************************************************************
 * Paintable Truss Element
 *****************************************************************************/

PaintableTrussElement::PaintableTrussElement () : 
    visible(true), 
    highlighted(false),
    enabled(true),
    renderedFlag(false)
{}

PaintableTrussElement::~PaintableTrussElement ()
{}

PaintableTrussElement::PaintableTrussElement ( bool h_, bool v_, 
                                               bool e_, bool r_ ) :
    visible(v_), 
    highlighted(h_),
    enabled(e_),
    renderedFlag(r_)
{}

void PaintableTrussElement::setVisible ( bool v_ )
{
    if ( visible == v_ ) 
        return;
    rendered(false);
    visible = v_;
    emit onVisibleChange( visible );
}

void PaintableTrussElement::setHighlighted ( bool h_ )
{
    if ( highlighted == h_ )
        return;
    rendered(false);
    highlighted = h_;
    emit onHighlightChange( highlighted );
}

void PaintableTrussElement::setEnabled ( bool e_ )
{
    if ( enabled == e_ )
        return;
    rendered(false);
    enabled = e_;    
    emit onEnableChange( enabled );
}

void PaintableTrussElement::rendered ( bool r_ ) const
{
    renderedFlag = r_;
}

bool PaintableTrussElement::isVisible () const
{
    return visible;
}

bool PaintableTrussElement::isHighlighted () const
{
    return highlighted;
}

bool PaintableTrussElement::isEnabled () const
{
    return enabled;
}

bool PaintableTrussElement::isRendered () const
{
    return renderedFlag;
}

/*****************************************************************************
 * Truss Unit
 *****************************************************************************/

TrussUnit::TrussUnit ( const QString& name, ObjectStateManager* mng ) :
    Truss<TrussNode, TrussPivot>(mng),
    trussRendered(false),
    calculated(false),
    trussName(name),
    frontNode(0),
    firstFront(0),
    lastFront(0)
{
    // We should render again when state has been changed
    QObject::connect( this, SIGNAL(onStateChange()), 
                            SLOT(trussUnitStateIsChanged()) );
    QObject::connect( this, SIGNAL(afterNodeDesist( const Node& )),
                            SLOT(clearFrontNodePointer( const Node& )) );
    QObject::connect( this, SIGNAL(afterPivotDesist( const Node&, 
                                                     const Node& )),
                            SLOT(clearFrontPivotPointer( const Node&, 
                                                         const Node& )) );
}

TrussUnit::~TrussUnit ()
{}


void TrussUnit::loadFromXML ( const QDomElement& elem ) 
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( elem );

    // Destroy existent truss elements
    clear();

    /**
     * Set name
     *****************/
    if ( ! elem.hasAttribute( "trussName" ) )
        throw LoadException();
    QString name = elem.attribute( "trussName" );   
    setTrussName( name );

    /**
     * Set truss area size
     ***********************/
    if ( ! elem.hasAttribute( "trussWidth" ) )
        throw LoadException();
    if ( ! elem.hasAttribute( "trussHeight" ) )
        throw LoadException();

    bool ok;
    double width = elem.attribute( "trussWidth" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    double height = elem.attribute( "trussHeight" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setTrussAreaSize( DoubleSize( width, height ) );

    /** 
     * Set dimension
     *****************/
    QDomNodeList trussDim = elem.elementsByTagName( "TrussDimension" );
    if ( trussDim.count() != 1 )
        throw LoadException();
    if ( ! trussDim.item(0).isElement() )
        throw LoadException();

    QDomElement trussDimElem = trussDim.item(0).toElement();
    if ( ! trussDimElem.hasAttribute( "lengthMeasure" ) )
        throw LoadException();
    if ( ! trussDimElem.hasAttribute( "forceMeasure" ) )
        throw LoadException();

    try { 
        TrussDimension dim( trussDimElem.attribute("lengthMeasure"),
                            trussDimElem.attribute("forceMeasure") );
        setDimension( dim );
    } catch ( TrussDimension::WrongArgsException& ) {
        throw LoadException();
    }

    // Tie UUID with truss nodes to link nodes with pivots, nodes with loads
    typedef QMap<QString, TrussNode*> TrussNodeUUIDMap;
    TrussNodeUUIDMap nodeUUIDMap;

    /** 
     * Create nodes
     *****************/
    QDomNodeList trussNodes = elem.elementsByTagName( "TrussNode" );
    for ( int nodesNum = 0; nodesNum < trussNodes.count(); ++nodesNum ) {
        QDomNode trussNode = trussNodes.item( nodesNum );
        if ( ! trussNode.isElement() )
            throw LoadException();
        QDomElement nodeElem = trussNode.toElement();
        // Default position is 0
        TrussNode& node = createNode( 0, 0 );
        node.loadFromXML( nodeElem );
        nodeUUIDMap[node.getUUID()] = &node;
    }

    /** 
     * Create load cases
     *********************/
    QDomNodeList trussLoadCases = elem.elementsByTagName( "LoadCase" );
    for ( int loadCasesNum = 0; 
          loadCasesNum < trussLoadCases.count(); 
          ++loadCasesNum ) {
        QDomNode trussLoadCase = trussLoadCases.item( loadCasesNum );
        if ( ! trussLoadCase.isElement() )
            throw LoadException();
        QDomElement loadCaseElem = trussLoadCase.toElement();
        
        LoadCase* loadCase = 0;
        if ( loadCasesNum != 0 )
            loadCase = &getLoadCases().createLoadCase();
        else
            loadCase = getLoadCases().getCurrentLoadCase();

        // Set current load case
        // Note: we don't check uniqueness of current attribute
        //       so last in the set will be applied.
        if ( loadCaseElem.hasAttribute( "current" ) && 
             loadCaseElem.attribute( "current" ) == "Yes" )
             getLoadCases().setCurrentLoadCase( *loadCase );

        // Create loads
        QDomNodeList trussLoads = 
            loadCaseElem.elementsByTagName( "Load" );
        for ( int loadsNum = 0; loadsNum < trussLoads.count(); ++loadsNum ) {
            QDomNode trussLoad = trussLoads.item( loadsNum );
            if ( ! trussLoad.isElement() )
                throw LoadException();            
            QDomElement loadElem = trussLoad.toElement();
            if ( ! loadElem.hasAttribute( "nodeId" ) )
                throw LoadException();
            if ( ! loadElem.hasAttribute( "xForce" ) )
                throw LoadException();
            if ( ! loadElem.hasAttribute( "yForce" ) )
                throw LoadException();

            QString nodeId = loadElem.attribute( "nodeId" );
            if ( ! nodeUUIDMap.contains(nodeId) )
                throw LoadException();

            bool ok;
            double xForce = loadElem.attribute( "xForce" ).toDouble( &ok );
            if ( !ok ) throw LoadException();

            double yForce = loadElem.attribute( "yForce" ).toDouble( &ok );
            if ( !ok ) throw LoadException();

            loadCase->addLoad( *nodeUUIDMap[nodeId], xForce, yForce );
        }
    }

    /** 
     * Create pivots
     *****************/
    QDomNodeList trussPivots = elem.elementsByTagName( "TrussPivot" );
    for ( int pivotsNum = 0; pivotsNum < trussPivots.count(); ++pivotsNum ) {
        QDomNode trussPivot = trussPivots.item( pivotsNum );
        if ( ! trussPivot.isElement() )
            throw LoadException();
        QDomElement pivotElem = trussPivot.toElement();
        if ( ! pivotElem.hasAttribute( "firstNodeId" ) )
            throw LoadException();
        if ( ! pivotElem.hasAttribute( "lastNodeId" ) )
            throw LoadException();

        QString firstNodeId = pivotElem.attribute( "firstNodeId" );
        QString lastNodeId = pivotElem.attribute( "lastNodeId" );
        if ( ! nodeUUIDMap.contains(firstNodeId) )
            throw LoadException();
        if ( ! nodeUUIDMap.contains(lastNodeId) )
            throw LoadException();

        TrussPivot& pivot = createPivot( *nodeUUIDMap[firstNodeId],
                                         *nodeUUIDMap[lastNodeId] );
        pivot.setMaterialUUIDMap( *materialUUIDMap );
        pivot.loadFromXML( pivotElem );
    }
}

void TrussUnit::setMaterialUUIDMap ( const QMap<QString, TrussMaterial*>& m )
{
    materialUUIDMap = &m;
}

QDomElement TrussUnit::saveToXML ( QDomDocument& doc )
{
    QDomElement trussElem = XMLSerializableObject::saveToXML( doc );
    trussElem.setTagName( "TrussUnit" );

    /**
     * Save name
     *****************/
    trussElem.setAttribute( "trussName", getTrussName() );

    /**
     * Save truss area size
     ***********************/
    const DoubleSize& size = getTrussAreaSize();
    trussElem.setAttribute( "trussWidth", size.width() );
    trussElem.setAttribute( "trussHeight", size.height() );

    /** 
     * Save dimension
     *****************/
    TrussDimension& trussDim = getDimension();
    QDomElement trussDimElem = doc.createElement( "TrussDimension" );
    trussDimElem.setAttribute( "lengthMeasure", 
                               trussDim.getLengthMeasureStr() );
    trussDimElem.setAttribute( "forceMeasure", 
                               trussDim.getForceMeasureStr() );
    trussElem.appendChild( trussDimElem );

    /** 
     * Save nodes
     *****************/
    NodeList nodes = getNodeList();
    NodeListIter nIter = nodes.begin();
    for ( ; nIter != nodes.end(); ++nIter )
        trussElem.appendChild( (*nIter)->saveToXML( doc ) );

    /** 
     * Save pivots
     *****************/
    PivotList pivots = getPivotList();
    PivotListIter pIter = pivots.begin();
    for ( ; pIter != pivots.end(); ++pIter )
        trussElem.appendChild( (*pIter)->saveToXML( doc ) );

    /** 
     * Save load cases
     *********************/
    LoadCases& loadCases = getLoadCases();    
    for ( uint indx = 1; indx <= loadCases.countLoadCases(); ++indx ) {
        LoadCase* loadCase = loadCases.findLoadCase( indx );
        QDomElement loadCaseElem = doc.createElement( "LoadCase" );
        // Save current
        if ( loadCases.getCurrentLoadCase() == loadCase )
            loadCaseElem.setAttribute( "current", "Yes" );
        else
            loadCaseElem.removeAttribute( "current" );

        LoadCase::TrussLoadMap loadMap = loadCase->getTrussLoadMap();
        LoadCase::TrussLoadMap::Iterator loadsIt = loadMap.begin();
        for ( ; loadsIt != loadMap.end(); ++loadsIt ) {
            const TrussNode* node = loadsIt.key();
            TrussLoad* load = loadsIt.value();
            QDomElement loadElem = doc.createElement( "Load" );
            loadElem.setAttribute( "nodeId", node->getUUID() );
            loadElem.setAttribute( "xForce", load->getXForce() );
            loadElem.setAttribute( "yForce", load->getYForce() );
            loadCaseElem.appendChild( loadElem );
        }
        trussElem.appendChild( loadCaseElem );        
    }

    return trussElem;
}

void TrussUnit::trussUnitStateIsChanged ()
{
    setTrussRenderedStatus(false);
}

bool TrussUnit::isCalculated () const
{
    return calculated;
}

void TrussUnit::setCalculatedStatus ( bool status )
{
    calculated = status;
}

void TrussUnit::setTrussRenderedStatus ( bool status ) const
{
    trussRendered = status;
}

const QString& TrussUnit::getTrussName () const
{
    return trussName;
}

void TrussUnit::setTrussName ( const QString& name )
{
    trussName = name;    
    emit onTrussNameChange( trussName );
    emit onStateChange();
}

void TrussUnit::setFocusOnNode ( TrussNode* selectedNode )
{
    selectedNode->setHighlighted ( true );
    nodeToFront ( *selectedNode );
    setTrussRenderedStatus(false);
}

void TrussUnit::setFocusOnPivot ( TrussPivot* selectedPivot )
{
    selectedPivot->setHighlighted ( true );
    pivotToFront ( *selectedPivot );
    setTrussRenderedStatus(false);
}

/* 
    Removes highlight from all the truss unit nodes except
    node1 and node2 if assigned.
*/
void TrussUnit::removeNodesHighlight ( const TrussNode* node1,
                                       const TrussNode* node2 )
{
    NodeList nodeList = getNodeList();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {
        TrussNode* node = *iter;
        if ( node->isHighlighted() && node != node1 && node != node2 )
        {
            node->setHighlighted( false );
            setTrussRenderedStatus(false);
        }
    }
}

void TrussUnit::removePivotsHighlight ( const TrussPivot* excPivot )
{
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        if ( pivot->isHighlighted () && pivot != excPivot )
        {
            pivot->setHighlighted ( false );
            setTrussRenderedStatus( false );
        }
    }
    if ( excPivot ) {
    // should be sure that nodes of the selected pivot are still highlighted
        excPivot->getFirstNode().setHighlighted ( true );
        excPivot->getLastNode().setHighlighted ( true );
    }
}

void TrussUnit::setTrussPosition ( const QPoint& pos )
{
    leftTopPos.setX( pos.x() - trussBufIndent );
    leftTopPos.setY( pos.y() - trussBufIndent );
}

void TrussUnit::setTrussAreaSizeInPix ( const QSize &size )
{
    pixAreaSize = size;
    setTrussRenderedStatus(false);
}

DoublePoint TrussUnit::getTrussScaleMultiplier () const
{
    const DoubleSize& areaSize = getTrussAreaSize();
    return DoublePoint( pixAreaSize.width() / areaSize.width(),
                        pixAreaSize.height() / areaSize.height() );
}

DoublePoint TrussUnit::getTrussCoordFromWidgetPos ( int x, int y ) const
{
    // TODO: flipY comparison
    DoublePoint scaleMult = getTrussScaleMultiplier ();
    double trussX = ( x - leftTopPos.x() - trussBufIndent ) / scaleMult.x();
    double trussY = ( leftTopPos.y() + pixAreaSize.height() + 
                      trussBufIndent - y ) / scaleMult.y();
    return DoublePoint( trussX, trussY );
}

DoublePoint TrussUnit::getTrussCoordFromWidgetPos ( QPoint pos ) const
{
    return getTrussCoordFromWidgetPos ( pos.x(), pos.y() );
}

QPoint TrussUnit::getWidgetPosFromTrussCoord ( double x, double y ) const
{
    DoublePoint scaleMult = getTrussScaleMultiplier ();
    int widgetX = int ( x * scaleMult.x() ) + trussBufIndent + leftTopPos.x();
    int widgetY = ( flipY ? int( ( getTrussAreaSize().height() - y ) * 
                   scaleMult.y() ) + trussBufIndent + leftTopPos.y() : 
                   int(y * scaleMult.y()) + trussBufIndent + leftTopPos.y() );
    return QPoint( widgetX, widgetY );
}

QPoint TrussUnit::getWidgetPosFromTrussCoord ( const DoublePoint& coord ) const
{
    return getWidgetPosFromTrussCoord ( coord.x(), coord.y() );
}

/*
    Returns size of the truss unit. Truss unit size are 
    equal to maximum x and y coords of its nodes.
*/
DoubleSize TrussUnit::getTrussSize () const
{
    DoubleSize trussSize( 0, 0 );
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {   
        TrussNode* node = *iter;
        if ( node->getX() > trussSize.width() )
            trussSize.setWidth( node->getX() );
        if ( node->getY() > trussSize.height() )
            trussSize.setHeight( node->getY() );
    }
    return trussSize;
}

/*
    Returns node finding precision. If inPix is TRUE, 
    returns it in pixels, otherwise returns it in truss 
    coordinates relative to the current truss area size.
*/
double TrussUnit::getNodePrecision ( bool inPix ) const
{
    if ( inPix )
        return 4.0;

    DoublePoint scaleMult = getTrussScaleMultiplier();
    double precision = ( 1.7 / scaleMult.x() ) * 
                       ( 1.7 / scaleMult.x() ) +
                       ( 1.7 / scaleMult.y() ) *
                       ( 1.7 / scaleMult.y() );
    return sqrt( precision );
}

/* 
   Returns pivot finding precision in truss coordinates
   relative to the current truss area size
*/
double TrussUnit::getPivotPrecision () const
{
    DoublePoint scaleMult = getTrussScaleMultiplier();
    double precision = ( 1.0 / scaleMult.x() ) * 
                       ( 1.0 / scaleMult.x() ) +
                       ( 1.0 / scaleMult.y() ) *
                       ( 1.0 / scaleMult.y() );
    return sqrt( precision );
}

TrussNode* TrussUnit::findNodeByWidgetPos ( const QPoint& pos, 
                                            double precision ) const
{
    QPoint nodePos;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {   
        TrussNode* node = *iter;
        nodePos = getWidgetPosFromTrussCoord ( node->getPoint() );
        if ( abs ( nodePos.x() - pos.x() ) < precision && 
             abs ( nodePos.y() - pos.y() ) < precision )
        {
            return node;
        }
    }
    return 0;
}

TrussNode* TrussUnit::findNodeByWidgetPos ( const QPoint& pos ) const
{
    return findNodeByWidgetPos ( pos, getNodePrecision() );
}

TrussNode* TrussUnit::findNodeByWidgetPos ( int x, int y, 
                                            double precision ) const
{
    return findNodeByWidgetPos ( QPoint( x, y ), precision );
}

TrussNode* TrussUnit::findNodeByWidgetPos ( int x, int y ) const
{
    return findNodeByWidgetPos ( x, y, getNodePrecision() );
}

TrussPivot* TrussUnit::findPivotByCoord ( const DoublePoint& coord, 
                                          double precision ) const
{
    DoublePoint firstNodeCoord, lastNodeCoord;
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        firstNodeCoord = pivot->getFirstNode().getPoint();
        lastNodeCoord = pivot->getLastNode().getPoint();
        double dist = pointToSegmentDist ( coord, firstNodeCoord, 
                                                  lastNodeCoord );
        if ( dist < precision  && 
            ( firstNodeCoord.x() - coord.x() ) * 
            ( firstNodeCoord.x() - coord.x() ) + 
            ( firstNodeCoord.y() - coord.y() ) *
            ( firstNodeCoord.y() - coord.y() ) > dist * dist &&
            ( lastNodeCoord.x() - coord.x() ) * 
            ( lastNodeCoord.x() - coord.x() ) + 
            ( lastNodeCoord.y() - coord.y() ) *
            ( lastNodeCoord.y() - coord.y() ) > dist * dist )
            return pivot;
    }
    return 0;
}

TrussPivot* TrussUnit::findPivotByCoord ( const DoublePoint& coord ) const
{
    return findPivotByCoord ( coord, getPivotPrecision() );
}

TrussPivot* TrussUnit::findPivotByWidgetPos ( const QPoint& pos, 
                                                    double precision ) const
{
    DoublePoint coord = getTrussCoordFromWidgetPos ( pos );
    return findPivotByCoord ( coord, precision );
}

TrussPivot* TrussUnit::findPivotByWidgetPos ( const QPoint& pos ) const
{
    return findPivotByWidgetPos ( pos, getPivotPrecision() );
}

TrussPivot* TrussUnit::findPivotByWidgetPos ( int x, int y, 
                                              double precision ) const
{
    return findPivotByWidgetPos ( QPoint( x, y ), precision );
}

TrussPivot* TrussUnit::findPivotByWidgetPos ( int x, int y ) const
{
    return findPivotByWidgetPos ( x, y, getPivotPrecision() );
}

void TrussUnit::moveTrussNode ( int x, int y, TrussNode* node )
{
    DoublePoint newCoord = getTrussCoordFromWidgetPos ( x, y );
    const DoubleSize& size = getTrussAreaSize();

    double areaWidth = size.width();
    double areaHeight = size.height();
    if ( newCoord.x() < 0 )
        newCoord.setX( 0 );
    if ( newCoord.x() > areaWidth )
        newCoord.setX( areaWidth );
    if ( newCoord.y() < 0 )
        newCoord.setY( 0 );
    if ( newCoord.y() > areaHeight )
        newCoord.setY( areaHeight );

    node->setPoint( newCoord.x(), newCoord.y() );

    // remove highlight from nodes except moved one
    removeNodesHighlight ( node );

    removePivotsHighlight ();

    // highlight the merging node
    TrussNode* mergeNode = 0;
    if ( ! node->isVisible() )
        mergeNode = nodesMergingComparison( *node, 2 * getNodePrecision(), 
                                            false );
    else
        mergeNode = nodesMergingComparison( *node, 2 * getNodePrecision(), 
                                            true );

    TrussPivot* divPivot = findDividualPivot( *node, getPivotPrecision() );

    if ( mergeNode )
        mergeNode->setHighlighted( true );

    else if ( divPivot )
        divPivot->setHighlighted( true );


    setTrussRenderedStatus( false );
}

void TrussUnit::moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                 QPoint firstNodeClickDist, 
                                 QPoint lastNodeClickDist )
{
    DoublePoint newCoord = getTrussCoordFromWidgetPos ( x, y );
    double newXFirst = newCoord.x()  + firstNodeClickDist.x();
    double newYFirst = newCoord.y() + firstNodeClickDist.y();
    double newXLast = newCoord.x() + lastNodeClickDist.x();
    double newYLast = newCoord.y() + lastNodeClickDist.y();
    
    TrussNode& first = pivot->getFirstNode();
    TrussNode& last = pivot->getLastNode();

    const DoublePoint& oldFirstCoord = first.getPoint();
    const DoublePoint& oldLastCoord  = last.getPoint();
    const DoubleSize& size = getTrussAreaSize();

    double oldXFirst = oldFirstCoord.x();
    double oldYFirst = oldFirstCoord.y();
    double oldXLast  = oldLastCoord.x();
    double oldYLast  = oldLastCoord.y();
    double areaWidth = size.width();
    double areaHeight = size.height();

    if ( newXFirst < 0 )
    {
        newXFirst = 0;
        newXLast = oldXLast - oldXFirst;
    }
    if ( newXLast < 0 )
    {
        newXLast = 0;
        newXFirst = oldXFirst - oldXLast;
    }
    if ( newYFirst < 0 )
    {
        newYFirst = 0;
        newYLast = oldYLast - oldYFirst;
    }
    if ( newYLast < 0 )
    {
        newYLast = 0;
        newYFirst = oldYFirst - oldYLast;
    }
    if ( newXFirst > areaWidth )
    {
        newXFirst = areaWidth;
        newXLast = oldXLast + ( areaWidth - oldXFirst );
    }
    if ( newXLast > areaWidth )
    {
        newXLast = areaWidth;
        newXFirst = oldXFirst + ( areaWidth - oldXLast );
    }
    if ( newYFirst > areaHeight )
    {
        newYFirst = areaHeight;
        newYLast = oldYLast + ( areaHeight - oldYFirst);
    }
    if ( newYLast > areaHeight )
    {
        newYLast = areaHeight;
        newYFirst = oldYFirst + ( areaHeight - oldYLast);
    }

    first.setPoint( newXFirst, newYFirst );
    last.setPoint( newXLast, newYLast );

    // remove highlight from nodes except moved one
    removeNodesHighlight ( &first, &last );

    removePivotsHighlight ( pivot );

    // highlight the merging node
    TrussNode* mergeNode = nodesMergingComparison( first, 
                                                   2 * getNodePrecision(), 
                                                   true );
    TrussPivot* divPivot = findDividualPivot( first, getPivotPrecision() );

    if ( mergeNode )
        mergeNode->setHighlighted( true );
    else if ( divPivot )
        divPivot->setHighlighted( true );

    mergeNode = nodesMergingComparison( last, 2 * getNodePrecision(), true );
    divPivot = findDividualPivot( last, getPivotPrecision() );

    if ( mergeNode )
        mergeNode->setHighlighted( true );
    else if ( divPivot )
        divPivot->setHighlighted( true );

    setTrussRenderedStatus(false);
}

TrussNode* TrussUnit::nodesMergingComparison ( TrussNode& comparableNode, 
                                               double precision, 
                                               bool fixationCheck )
{
    QPoint nodePos = getWidgetPosFromTrussCoord ( comparableNode.getPoint() );
    TrussNode* node = 0;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {   
        TrussNode* n = *iter;
        QPoint pos = getWidgetPosFromTrussCoord ( n->getPoint() );
        if ( abs ( nodePos.x() - pos.x() ) < precision && 
             abs ( nodePos.y() - pos.y() ) < precision &&
             n != &comparableNode )
        {
            node = n;
        }
    }
    if ( node != 0 )
    {
        if ( fixationCheck )
        {
            if ( node->getFixation() == comparableNode.getFixation() )
            {
                return node;
            }
        }
        else
            return node;
    }
    setTrussRenderedStatus(false);
    return 0;
}

void TrussUnit::mergeNodes ( TrussNode* mergingNode, TrussNode* node )
{
    ObjectStateManager* mng = mergingNode->getStateManager();
    mng->startStateBlock();

    TrussPivot* fakePivot = findPivotByNodes ( *mergingNode, *node );
    if ( fakePivot ) {
        // Save remove pivot action
        ObjectState& state = fakePivot->createState( "remove pivot" );
        state.addAction( new TrussPivotRemoveAction( *fakePivot ) );
        state.save();
        fakePivot->desist();
    }

    // Reduce search area: find only adjoining pivots
    PivotList pivotList = findAdjoiningPivots( *mergingNode );
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        TrussNode* firstNode = &pivot->getFirstNode();
        TrussNode* lastNode = &pivot->getLastNode();
        if ( firstNode == mergingNode )
        {
            if ( findPivotByNodes ( *node, *lastNode ) ) 
            // remove fake pivot
            {
                // Save remove pivot action
                ObjectState& state = pivot->createState( "remove pivot" );
                state.addAction( new TrussPivotRemoveAction( *pivot ) );
                state.save();
                pivot->desist();
            }
            else
            {
                // Save action of node changing
                ObjectState& state = pivot->createState( "change node" );
                state.addAction( new TrussPivotChangeNodeAction( *pivot, 
                                 *firstNode, *node, true ) );
                state.save();
                pivot->setFirstNode( node );
            }
        }
        else if ( lastNode == mergingNode )
        {
            if ( findPivotByNodes ( *node, *firstNode ) ) 
            // remove fake pivot
            {
                // Save remove pivot action
                ObjectState& state = pivot->createState( "remove pivot" );
                state.addAction( new TrussPivotRemoveAction( *pivot ) );
                state.save();
                pivot->desist();
            }
            else
            {
                // Save action of node changing
                ObjectState& state = pivot->createState( "change node" );
                state.addAction( new TrussPivotChangeNodeAction( *pivot, 
                                 *lastNode, *node, false ) );
                state.save();
                pivot->setLastNode( node );
            }
        }
    }

    // Save remove node action
    ObjectState& state = mergingNode->createState( "remove node" );
    state.addAction( new TrussNodeRemoveAction( *mergingNode ) );
    state.save();

    mergingNode->desist();

    mng->endStateBlock();

    setTrussRenderedStatus(false);
}

void TrussUnit::dividePivot ( TrussPivot& dividualPivot, 
                                    TrussNode& dividingNode )
{
    TrussNode& first = dividualPivot.getFirstNode();
    TrussNode& last = dividualPivot.getLastNode();

    ObjectStateManager* mng = dividualPivot.getStateManager();
    mng->startStateBlock();

    dividualPivot.setHighlighted ( false );

    // Save remove pivot action
    ObjectState& state = dividualPivot.createState( "remove pivot" );
    state.addAction( new TrussPivotRemoveAction( dividualPivot ) );
    state.save();
    dividualPivot.desist();

    TrussPivot* pivot1 = findPivotByNodes ( first, dividingNode );
    TrussPivot* pivot2 = findPivotByNodes ( last, dividingNode );
    if ( pivot1 )
    {
        // Save remove pivot action
        ObjectState& state = pivot1->createState( "remove pivot" );
        state.addAction( new TrussPivotRemoveAction( *pivot1 ) );
        state.save();
        pivot1->desist();
    }
    if ( pivot2 )
    {
        // Save remove pivot action
        ObjectState& state = pivot2->createState( "remove pivot" );
        state.addAction( new TrussPivotRemoveAction( *pivot2 ) );
        state.save();
        pivot2->desist();
    }
    TrussPivot* newlyCreated = 0;

    if ( first.getNumber() < dividingNode.getNumber() )
        newlyCreated = &createPivot( first, dividingNode );
    else
        newlyCreated = &createPivot( dividingNode, first );

    // Save create pivot action
    ObjectState& stateFirst = newlyCreated->createState( "create pivot" );
    stateFirst.addAction( new TrussPivotCreateAction( *newlyCreated ) );
    stateFirst.save();

    if ( last.getNumber() < dividingNode.getNumber() )
        newlyCreated = &createPivot( last, dividingNode );
    else
        newlyCreated = &createPivot( dividingNode, last );

    // Save create pivot action
    ObjectState& stateSecond = newlyCreated->createState( "create pivot" );
    stateSecond.addAction( new TrussPivotCreateAction( *newlyCreated ) );
    stateSecond.save();
    mng->endStateBlock();

    setTrussRenderedStatus(false);
}

/*
    Finds pivot which is intersected by incoming node, but 
    doesn't divided as yet.
*/
TrussPivot* TrussUnit::findDividualPivot ( TrussNode& dividingNode,
                                           double precision ) const
{
    TrussPivot* dividualPivot = findPivotByCoord ( dividingNode.getPoint(), 
                                                   precision );
    if ( ! dividualPivot )
        return 0;

    if ( &dividingNode == &dividualPivot->getFirstNode() || 
         &dividingNode == &dividualPivot->getLastNode() )
        return 0;

    return dividualPivot;
}

/*
    Finds pivot which is intersected by incoming node, but 
    doesn't divided as yet. Half precision is used by default.
*/
TrussPivot* TrussUnit::findDividualPivot ( TrussNode& dividingNode ) const
{
    return findDividualPivot( dividingNode, 2 * getPivotPrecision() );
}


DoublePointArray TrussUnit::getPivotCrossPoints ( 
                                     const PivotList& nonCrossingPivots ) const
{
    DoublePointArray crossPoints;

    if ( nonCrossingPivots.empty() )
        return crossPoints;

    // find points at which pivots from incoming list intersect with other pivots
    PivotList pivotList = getPivotList ();
    PivotList::reverse_iterator rev_iter = pivotList.rbegin();
    for ( ; rev_iter != pivotList.rend(); ++rev_iter )
    {
        TrussPivot* pivot = *rev_iter;
        const DoublePoint& p11 = pivot->getFirstNode().getPoint();
        const DoublePoint& p12 = pivot->getLastNode().getPoint();

        PivotListConstIter iter = nonCrossingPivots.begin();
        for ( ; iter != nonCrossingPivots.end(); ++iter )
        {   
            TrussPivot* adjPivot = *iter;
            if ( adjPivot == pivot )
                break;  // taken pivot is one of the noncrossing pivots

            const DoublePoint& p21 = adjPivot->getFirstNode().getPoint();
            const DoublePoint& p22 = adjPivot->getLastNode().getPoint();

            DoublePoint crossPoint = getLineSegmentsCrossPoint( p11, p12, 
                                                                p21, p22 );

            // we are interested in cross points between pivot nodes only
            if ( comparePoints( p11, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p12, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p21, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p22, crossPoint, getNodePrecision(false) ) )
                continue;

            if ( crossPoint.x() == -1 )
                continue;   // there are no intersections between these pivots

            if ( crossPoints.contains( crossPoint ) == 0 ) {
                crossPoints.append( crossPoint );
            }
        }
    }

    // find points at which pivots from incoming list intersect with nodes
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {
        TrussNode* node = *iter;
        TrussPivot* pivot = findDividualPivot ( *node );
        // check if node belongs to one of the incoming pivots
        if ( pivot && 
             std::find( nonCrossingPivots.begin(), 
                        nonCrossingPivots.end(), pivot ) != 
             nonCrossingPivots.end() )
        {
            if ( crossPoints.contains( node->getPoint() ) == 0 ) {
                crossPoints.append( node->getPoint() );
            }
        }
    }
    return crossPoints;
}

void TrussUnit::createPivotCrossNodes ( const DoublePointArray& crossPoints )
{
    if ( crossPoints.isEmpty() )
        return;

    TrussNode *crossNode, *node;
    // create nodes at found points
    NodeList crossNodes;
    DoublePointArray::ConstIterator iter = crossPoints.begin();
    for ( ; iter != crossPoints.end(); ++iter )
    {
        crossNode = &createCrossNode ( *iter );
        if ( crossNode )
            crossNodes.push_back ( crossNode );
    }
    // check created nodes for merging
    NodeListIter nodeIter = crossNodes.begin();
    for ( ; nodeIter != crossNodes.end(); ++nodeIter )
    {
        crossNode = *nodeIter;
        // can not allow unfixed node to "eat" a fixed one
        if ( crossNode->getFixation() == Node::Unfixed )
            node = nodesMergingComparison( *crossNode, 
                                           int(1.5 * getNodePrecision()),
                                           false );
        else
            node = nodesMergingComparison( *crossNode, 
                                           int(1.5 * getNodePrecision()),
                                           true );
        if ( node )
            mergeNodes ( crossNode, node );
    }
}

TrussNode& TrussUnit::createCrossNode ( const DoublePoint& crossPoint )
{
    TrussNode* crossNode = findNodeByCoord ( crossPoint, 0 );

    if ( !crossNode )
    {
        crossNode = &createNode ( crossPoint.x(), crossPoint.y() );
        // Save create node action
        ObjectState& state = crossNode->createState( "create node" );
        TrussNodeCreateAction* action = 
            new TrussNodeCreateAction( *crossNode );
        state.addAction( action );
        state.save();
    }

    TrussPivot* pivot = 0;

    PivotList pivotList = getPivotList ();
    uint i;
    for ( i = 0; i < pivotList.size(); i++ )
    {
        pivot = findDividualPivot ( *crossNode );
        if ( pivot )
            dividePivot ( *pivot, *crossNode );
    }
    return *crossNode;
}

/*
    Updates new position of selected node: merge with other node and(or) 
    divide pivot if necessary.
*/
void TrussUnit::updateNodePosition ( TrussNode* selectedNode, 
                                           bool fixationCheck )
{
    TrussNode* node = nodesMergingComparison( *selectedNode, 
                                              2 * getNodePrecision(), 
                                              fixationCheck );
    if ( node )
        mergeNodes ( selectedNode, node );
    else
    {
        TrussPivot* pivot = findDividualPivot ( *selectedNode );
        if ( pivot )
            dividePivot ( *pivot, *selectedNode );
    }    
}

/* 
    Analyses positions of the selected node and its 
    adjoining pivots relative to other truss elements.
*/
void TrussUnit::updateAfterNodeManipulation ( TrussNode* selectedNode, 
                                              bool fixationCheck )
{
    int numb = selectedNode->getNumber();

    PivotList adjPivotList = findAdjoiningPivots ( *selectedNode );
    DoublePointArray crossPoints = getPivotCrossPoints ( adjPivotList );
    createPivotCrossNodes ( crossPoints );

    selectedNode = findNodeByNumber( numb );
    if ( !selectedNode )
        return;

    updateNodePosition ( selectedNode, fixationCheck );
}

/* 
    Analyses positions of the selected pivot with adjoining 
    pivots of its nodes relative to other truss elements.
*/
void TrussUnit::updateAfterPivotManipulation ( TrussPivot* selectedPivot,
                                               bool fixationCheck )
{
    TrussNode& first = selectedPivot->getFirstNode();
    TrussNode& last = selectedPivot->getLastNode();
    DoublePointArray crossPoints;

    PivotList firstAdjPivots = findAdjoiningPivots ( first );
    PivotList lastAdjPivots = findAdjoiningPivots ( last );

    PivotListIter pivotIter = std::find( lastAdjPivots.begin(), 
                                         lastAdjPivots.end(), 
                                         selectedPivot );
    if ( pivotIter != lastAdjPivots.end() )
    {
        lastAdjPivots.erase ( pivotIter );
        crossPoints = getPivotCrossPoints ( lastAdjPivots );
        createPivotCrossNodes ( crossPoints );
    }

    crossPoints = getPivotCrossPoints ( firstAdjPivots );
    createPivotCrossNodes ( crossPoints );

    updateNodePosition ( &first, fixationCheck );
    updateNodePosition ( &last, fixationCheck );
}

void TrussUnit::nodeToFront ( TrussNode& node )
{
    firstFront = 0;
    lastFront = 0;
    frontNode = &node;
}

void TrussUnit::pivotToFront ( TrussPivot& pivot )
{
    frontNode = 0;
    firstFront = &pivot.getFirstNode();
    lastFront = &pivot.getLastNode();
}

void TrussUnit::clearFrontNodePointer ( const Node& node )
{
    if ( &node == frontNode )
        frontNode = 0;
    if ( &node == firstFront )
        firstFront = 0;
    else if ( &node == lastFront )
        lastFront = 0;
}

void TrussUnit::clearFrontPivotPointer ( const Node& first, const Node& last )
{
    if ( &first == firstFront &&
         &last == lastFront )
    {
        firstFront = 0;
        lastFront = 0;
    }
}

void TrussUnit::desistAdjoiningPivots ( const TrussNode& node )
{
    // Find all pivots
    PivotList pivotList = findAdjoiningPivots( node, false );
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        if ( ! pivot->isAlive() )
            continue;
        // Save remove pivot action
        ObjectState& state = pivot->createState( "remove pivot" );
        state.addAction( new TrussPivotRemoveAction( *pivot ) );
        state.save();
        pivot->desist();
    }
}

void TrussUnit::drawLoad ( ren_dynarow& baseRend, const TrussLoad& load, 
                           const QPoint& tailPos ) const
{
    double x = load.getXForce (),
           y = load.getYForce (), 
           x1 = 1, y1 = 1, sinA;

    if ( x == 0 && y == 0 )
        return;

    if ( x != 0 && y != 0 )
    {
        sinA = fabs ( y / sqrt( x * x + y * y ) );
        y1 = sinA * 1;
        x1 = sqrt( 1 - y1 * y1 );
    }

    QPoint forceDirection;

    if ( x < 0 )
        forceDirection.setX ( - 1 );
    else if ( x > 0 )
        forceDirection.setX ( 1 );

    if ( y < 0 )
        forceDirection.setY ( - 1 );
    else if ( y > 0 )
        forceDirection.setY ( 1 );

    QPoint headPos( int( tailPos.x() + 20 * x1 * forceDirection.x() ), 
                    int( tailPos.y() - 20 * y1 * forceDirection.y() ) );
    
    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    color_type color = agg::rgba( 0, 0, 25 );

    drawArrow ( ras, solidRend, sl, tailPos, headPos, color, 4, 4, 4, 6 ); 
}

void TrussUnit::drawNodeNumber( TrussNode* node, 
                                ren_dynarow& baseRend, 
                                solidRenderer& solidRend, 
                                scanline_rasterizer& ras, 
                                agg::scanline_p8& sl ) const
{
    if ( ! node->isVisible() )
        return;

    QPoint nodePos = getWidgetPosFromTrussCoord ( node->getPoint() );
    int rad;
    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    QPoint textPos, backLeftTopPos, backRightBottomPos;

    if ( ! node->isHighlighted() )
    {
        backColor = agg::rgba(1, 1, 1, 0.7);
        numbFont = agg::gse5x9;
        textColor = agg::rgba(0, 100, 0);
        if ( node->getNumber() < 10 )
        {
            textPos.setX ( nodePos.x() + 1 - leftTopPos.x() );
            textPos.setY ( nodePos.y() - 5 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 6 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        else
        {
            textPos.setX ( nodePos.x() - 1 - leftTopPos.x() );
            textPos.setY ( nodePos.y() - 5 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 1 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 11 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        rad = 3;
    }
    else
    {
        backColor = agg::rgba(1, 1, 1, 0.8);
        numbFont = agg::gse6x12;
        textColor = agg::rgba(0, 100, 0);
        if ( node->getNumber() < 10 )
        {
            textPos.setX ( nodePos.x() + 1 - leftTopPos.x() ) ;
            textPos.setY ( nodePos.y() - 6 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() - 1 );
            backRightBottomPos.setX ( textPos.x() + 7 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        else
        {
            textPos.setX ( nodePos.x() - 1 - leftTopPos.x() ) ;
            textPos.setY ( nodePos.y() - 6 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 13 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        rad = 5;
    }

    agg::rounded_rect backRect ( backLeftTopPos.x(), 
                                 backLeftTopPos.y(), 
                                 backRightBottomPos.x(), 
                                 backRightBottomPos.y(), rad );
    ras.add_path ( backRect );
    solidRend.color ( backColor );
    agg::render_scanlines ( ras, sl, solidRend );
    glyph.font ( numbFont );
    textRenderer textRend ( baseRend, glyph );
    QString str;
    str = QString("%1").arg( node->getNumber() );
    drawText ( textRend, str, textColor, textPos );
}

void TrussUnit::drawPivotNumber( TrussPivot* pivot, 
                                 ren_dynarow& baseRend, 
                                 solidRenderer& solidRend, 
                                 scanline_rasterizer& ras, 
                                 agg::scanline_p8& sl ) const
{
    if ( ! pivot->isVisible() || ! pivot->getDrawingStatus() )
        return;

    QPoint p1 = getWidgetPosFromTrussCoord ( pivot->getFirstNode().getPoint() );
    QPoint p2 = getWidgetPosFromTrussCoord ( pivot->getLastNode().getPoint() );

    int rad;
    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    QPoint textPos, backLeftTopPos, backRightBottomPos;

    if ( ! pivot->isHighlighted() )
    {
        backColor = agg::rgba(1, 1, 1, 0.9);
        numbFont = agg::gse5x9;
        textColor = agg::rgba(30, 0, 0);
        if ( pivot->getNumber() < 10 )
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 2 - leftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 4 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 6 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        else
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 4 - leftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 4 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 1 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 11 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        rad = 3;
    }
    else
    {
        backColor = agg::rgba(1, 1, 1, 0.9);
        numbFont = agg::gse6x12;
        textColor = agg::rgba(30, 0, 0);
        if ( pivot->getNumber() < 10 )
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 3 - leftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 5 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 3 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 7 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        else
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 5 - leftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 5 - leftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 13 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        rad = 5;
    }

    agg::rounded_rect backRect ( backLeftTopPos.x(), backLeftTopPos.y(), 
                                 backRightBottomPos.x(), 
                                 backRightBottomPos.y(), rad );
    ras.add_path ( backRect );
    solidRend.color ( backColor );
    agg::render_scanlines ( ras, sl, solidRend );
    glyph.font ( numbFont );
    textRenderer textRend ( baseRend, glyph );
    QString str;
    str = QString("%1").arg( pivot->getNumber() );
    drawText ( textRend, str, textColor, textPos );
}

void TrussUnit::drawTrussElementsNumbers ( ren_dynarow& baseRend, 
                                           solidRenderer& solidRend, 
                                           scanline_rasterizer& ras, 
                                           agg::scanline_p8& sl ) const
{
    PivotList pivots = getPivotList ();
    PivotListIter pivotIter = pivots.begin ();
    for ( ; pivotIter != pivots.end(); ++pivotIter )
        drawPivotNumber ( *pivotIter, baseRend, solidRend, ras, sl );

    NodeList nodes = getNodeList ();
    NodeListIter nodeIter = nodes.begin ();
    for ( ; nodeIter != nodes.end(); ++nodeIter )
        drawNodeNumber ( *nodeIter, baseRend, solidRend, ras, sl );
}

void TrussUnit::paint ( ren_dynarow& baseRend ) const
{
    bool numbersDrawing = true;

    if ( ! trussRendered )
    {
        baseRend.clear ( agg::rgba( 20, 20, 20, 0 ) );
        DoublePoint scaleMult = getTrussScaleMultiplier ();
        scanline_rasterizer ras;
        agg::scanline_p8 sl;
        solidRenderer solidRend ( baseRend ); 
        agg::ellipse ell;
        QPoint pos;
        double trussAreaHeight = getTrussAreaSize().height();
        PivotList pivotList = getPivotList ();
        PivotList::const_iterator pivotsIter = pivotList.begin();
        for ( ; pivotsIter != pivotList.end(); ++pivotsIter )
            (*pivotsIter)->paint( baseRend, scaleMult, trussAreaHeight );

        LoadCase* loadCase = getLoadCases().getCurrentLoadCase();
        
        NodeList nodeList = getNodeList ();
        NodeList::const_iterator nodesIter = nodeList.begin();
        for ( ; nodesIter != nodeList.end(); ++nodesIter )
            if ( *nodesIter != frontNode &&
                 *nodesIter != firstFront &&
                 *nodesIter != lastFront )
            {
                TrussNode* node = *nodesIter;
                QPoint nodePos = getWidgetPosFromTrussCoord( node->getPoint() ) - 
                                 leftTopPos;
                if ( loadCase ) 
                {
                    TrussLoad* load = loadCase->findLoad( *node );
                    if ( load ) drawLoad( baseRend, *load, nodePos );
                }
                node->paint( baseRend, scaleMult, trussAreaHeight );
            }
           
        if ( frontNode ) 
        {
            QPoint nodePos = getWidgetPosFromTrussCoord( frontNode->getPoint() ) - 
                             leftTopPos;
            if ( loadCase ) 
            {
                pos = getWidgetPosFromTrussCoord( frontNode->getPoint() );
                TrussLoad* load = loadCase->findLoad( *frontNode );
                if ( load ) drawLoad( baseRend, *load, nodePos );
            }
            frontNode->paint( baseRend, scaleMult, trussAreaHeight );
        }

        if ( firstFront && firstFront != frontNode ) 
        {
            QPoint nodePos = getWidgetPosFromTrussCoord( firstFront->getPoint() ) - 
                             leftTopPos;
            if ( loadCase ) 
            {
                pos = getWidgetPosFromTrussCoord( firstFront->getPoint() );
                TrussLoad* load = loadCase->findLoad( *firstFront );
                if ( load ) drawLoad( baseRend, *load, nodePos );
            }
            firstFront->paint( baseRend, scaleMult, trussAreaHeight );
        }

        if ( lastFront && lastFront != frontNode ) 
        {
            QPoint nodePos = getWidgetPosFromTrussCoord( lastFront->getPoint() ) - 
                             leftTopPos;
            if ( loadCase ) 
            {
                pos = getWidgetPosFromTrussCoord( lastFront->getPoint() );
                TrussLoad* load = loadCase->findLoad( *lastFront );
                if ( load ) drawLoad( baseRend, *load, nodePos );
            }
            lastFront->paint( baseRend, scaleMult, trussAreaHeight );
        }

        /*------draw truss elements numbers------*/
        if ( numbersDrawing )
            drawTrussElementsNumbers ( baseRend, solidRend, ras, sl );
        
        setTrussRenderedStatus(true);
    }
}

/****************************************************************************/
