
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ( ObjectStateManager* mng ) :
    Node(mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

TrussNode::TrussNode ( double x, double y, ObjectStateManager* mng ) :
    Node(x, y, mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

TrussNode::TrussNode ( double x, double y, Fixation fix, 
                       ObjectStateManager* mng ) :
    Node(x, y, fix, mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

void TrussNode::loadFromXML ( const QDomElement& nodeElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( nodeElem );

    /** 
     * Set position
     *****************/
    if ( ! nodeElem.hasAttribute( "x" ) )
        throw LoadException();
    if ( ! nodeElem.hasAttribute( "y" ) )
        throw LoadException();

    bool ok;
    double x = nodeElem.attribute( "x" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    double y = nodeElem.attribute( "y" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setPoint( x, y );

    /** 
     * Set fixation
     *****************/
    Fixation fixation = Unfixed;
    if ( nodeElem.hasAttribute( "fixation" ) ) {
        QString fixStr = nodeElem.attribute( "fixation" );
        if ( fixStr == "x" )
            fixation = FixationByX;
        else if ( fixStr == "y" )
            fixation = FixationByY;
        else if ( fixStr == "xy" || fixStr == "yx" )
            fixation = FixationByXY;
        else
            throw LoadException();
    }

    setFixation( fixation );
}

QDomElement TrussNode::saveToXML ( QDomDocument& doc )
{
    QDomElement nodeElem = XMLSerializableObject::saveToXML( doc );
    nodeElem.setTagName( "TrussNode" );

    /** 
     * Save position
     *****************/
    const DoublePoint& pos = getPoint();
    nodeElem.setAttribute( "x", pos.x() );
    nodeElem.setAttribute( "y", pos.y() );

    /** 
     * Save fixation
     *****************/
    Fixation fix = getFixation();
    if ( fix != Unfixed ) {
        QString fixStr = "xy";
        if ( fix == FixationByX )
            fixStr = "x";
        else if ( fix == FixationByY )
            fixStr = "y";
        nodeElem.setAttribute( "fixation", fixStr );
    }
    else 
        nodeElem.removeAttribute( "fixation" );

    return nodeElem;
}


void TrussNode::removeNodeHighlight ()
{
    setHighlighted ( false );
}

void TrussNode::drawFixation( scanline_rasterizer& ras, 
                              solidRenderer& solidRend, 
                              agg::scanline_p8& sl, 
                              const DoublePoint& nodePos,
                              double lineWidth, 
                              color_type color ) const
{
    DoublePoint leftPnt, rightPnt;

    if ( getFixation() == FixationByX )
    {
        leftPnt.setX( nodePos.x() - 4 );
        leftPnt.setY( nodePos.y() - 8 );
        rightPnt.setX( nodePos.x() - 4 );
        rightPnt.setY( nodePos.y() + 8 );
        drawLine( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX( leftPnt.x() - 4 );
        leftPnt.setY( leftPnt.y() - 4 );
        rightPnt.setX( rightPnt.x() - 1 );
        rightPnt.setY( leftPnt.y() + 3 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setY( rightPnt.y() + 4 );
            leftPnt.setY( leftPnt.y() + 4 );
            drawLine( ras, solidRend, sl, leftPnt, rightPnt, 
                      lineWidth - 1, color );
        }
    }

    if ( getFixation() == FixationByY )
    {
        leftPnt.setX( nodePos.x() - 8 );
        leftPnt.setY( nodePos.y() + 4 );
        rightPnt.setX( nodePos.x() + 9 );
        rightPnt.setY( nodePos.y() + 4 );
        drawLine( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX( nodePos.x() - 11 );
        leftPnt.setY( nodePos.y() + 8 );
        rightPnt.setX( nodePos.x() - 8 );
        rightPnt.setY( nodePos.y() + 5 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX( rightPnt.x() + 4 );
            leftPnt.setX( leftPnt.x() + 4 );
            drawLine( ras, solidRend, sl, leftPnt, 
                      rightPnt, lineWidth - 1, color );
        }
    }

    if ( getFixation() == FixationByXY )
    {
        leftPnt.setX( nodePos.x() - 4 );
        leftPnt.setY( nodePos.y() + 9 );
        rightPnt.setX( nodePos.x() + 4 );
        rightPnt.setY( nodePos.y() + 9 );
        drawLine( ras, solidRend, sl, nodePos, leftPnt, lineWidth, color );
        drawLine( ras, solidRend, sl, nodePos, rightPnt, lineWidth, color );
    
        leftPnt.setX( leftPnt.x() - 4 );
        rightPnt.setX( rightPnt.x() + 5 );
        drawLine( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );

        rightPnt.setX( leftPnt.x() );
        rightPnt.setY( leftPnt.y() );
        leftPnt.setX( leftPnt.x() - 4 );
        leftPnt.setY( leftPnt.y() + 4 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX( rightPnt.x() + 4 );
            leftPnt.setX( leftPnt.x() + 4 );
            drawLine( ras, solidRend, sl, leftPnt, 
                      rightPnt, lineWidth - 1, color );
        }
    }
}

void TrussNode::paint ( ren_dynarow& baseRend, const DoublePoint& scaleMult,
                        double trussAreaHeight ) const
{
    if ( !isVisible() )
        return;

    const DoublePoint& nodeCoord = getPoint();
    DoublePoint nodePos;
    nodePos.setX( nodeCoord.x() * scaleMult.x() + Global::trussBufIndent );
    nodePos.setY( Global::flipY ? ( trussAreaHeight - nodeCoord.y() ) * 
                  scaleMult.y() + Global::trussBufIndent : nodeCoord.y() * 
                  scaleMult.y() + Global::trussBufIndent );

    solidRenderer solidRend ( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::ellipse ell;

    double highlightKoeff = 0;
    if ( isHighlighted () )
    {
        highlightKoeff = 1;
    }

    //  draw node' outline
    solidRend.color ( agg::rgba(10, 10, 10) );
    ell.init( nodePos.x(), nodePos.y(), 
              Global::nodesRadius + highlightKoeff + 1.0, 
              Global::nodesRadius + highlightKoeff + 1.0, 16 );
    ras.add_path( ell );
    agg::render_scanlines( ras, sl, solidRend );

    //  draw fixation
    if ( getFixation () )
        drawFixation ( ras, solidRend, sl, nodePos, 2, agg::rgba(0, 0, 35) ); 

    if ( isHighlighted () )
    {
        // draw translucent highlight
        solidRend.color( agg::rgba(200, 135, 15, 0.6) );
        ell.init( nodePos.x(), nodePos.y(), Global::nodesRadius + 5, 
                                            Global::nodesRadius + 5, 16 );
        ras.add_path( ell );
        agg::render_scanlines( ras, sl, solidRend );
    }

    //  draw node
    agg::trans_affine mtx;
    mtx *= agg::trans_affine_scaling( 1 / 2.0 );
    mtx *= agg::trans_affine_translation( nodePos.x(), nodePos.y() );
    mtx.invert();
    color_array_type gradColors;
    radial_gradient gradFunc;

    if ( getFixation() == Unfixed )
    {
        color_type begin( 255, 255, 255 ); 
        color_type middle( 230, 200, 195 ); 
        color_type end( 130 - highlightKoeff * 100, 
                        90 - highlightKoeff * 60, 
                        70 - highlightKoeff * 50 );
        fillColorArray( gradColors, begin, middle, end );
        drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, 
                             mtx, nodePos.x(), nodePos.y(), 
                             Global::nodesRadius + highlightKoeff, 0, 10 ); 
    }
    else 
    {
        color_type begin( 255, 255, 255 ); 
        color_type middle( 80, 100, 195 ); 
        color_type end( 0, 0, 80 );
        fillColorArray( gradColors, begin, middle, end );
        drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, 
                             mtx, nodePos.x(), nodePos.y(), 
                             Global::nodesRadius + highlightKoeff, 0, 10 ); 
    }
}

/****************************************************************************/
