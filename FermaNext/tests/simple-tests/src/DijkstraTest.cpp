
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>

#include <QRegion> 
#include <QPointArray>
#include <QMap>

#define MIN(v1, v2)( (v1) < (v2) ? (v1) : (v2)  )

#define PI 3.14159265359

const int MAX_INT = 99999999;

struct Graph;
struct GraphNode;
struct GraphPivot;

struct Vector
{
    int x, y;
};

float vectorlength ( const Vector& v )
{
    return sqrt(v.x * v.x + v.y * v.y);
}

float angle ( const Vector& a, const Vector& b )
{
  float cosine = a.x * b.x + a.y * b.y / (vectorlength(a) * vectorlength(b));
  // rounding errors might make dotproduct out of range for cosine
  if (cosine > 1) cosine = 1;
  else if (cosine < -1) cosine = -1;

  if (a.x * b.y - a.y * b.x < 0)
    return 2*PI - acos(cosine);
  else
    return acos(cosine);
}


struct GraphNode
{
    GraphNode( int num, int x_, int y_ ) : number(num), x(x_), y(y_) {}
    QPoint point () { return QPoint(x, y); }

    int number;
    int x, y;
};

float angle ( const GraphNode* center,  const GraphNode* a  )
{
    Vector v1, v2;
    v1.x = a->x - center->x;
    v1.y = a->y - center->y;
    v2.x = 0;
    v2.y = 1;
    return angle(v1, v2);
}


struct GraphPivot
{
    GraphPivot ( GraphNode* first,
                 GraphNode* last,
                 int c ) : firstNode(first), lastNode(last), cost(c) {}   


    GraphNode* firstNode;
    GraphNode* lastNode;
    int cost;
};


struct PivotsSortAlg
{
    PivotsSortAlg ( GraphNode* n ) : nodeToSort(n) {}

    bool operator()( GraphPivot*& p1, GraphPivot*& p2 )
    {
        float angle1 = angle(nodeToSort, 
                             (p1->firstNode == nodeToSort ? 
                              p1->lastNode :
                              p1->firstNode) );
        float angle2 = angle(nodeToSort, 
                             (p2->firstNode == nodeToSort ? 
                              p2->lastNode :
                              p2->firstNode) );

        return angle1 < angle2;
    }
    
    GraphNode* nodeToSort;
};


typedef std::vector<GraphNode*> Nodes;
typedef std::vector<GraphPivot*> Pivots;

struct Segment
{
    Segment ( uint indx, GraphNode* c, GraphPivot* l, GraphPivot* r ) : 
        index(indx), centerNode(c), left(l), right(r) {}

    GraphNode* leftNode () const
    {
        return left->firstNode == centerNode ? left->lastNode : 
                                               left->firstNode;
    }

    GraphNode* rightNode () const
    {
        return right->firstNode == centerNode ? right->lastNode : 
                                                right->firstNode;
    }

    uint index;
    GraphNode* centerNode;
    GraphPivot* left;
    GraphPivot* right;
};


typedef std::vector<Segment> Segments;

struct Graph
{

    Pivots shortestPath ( GraphNode* begin, GraphNode* end )
    {
        typedef QMap<GraphNode*, int> Cost;
        typedef QMap<GraphNode*, GraphNode*> Path;

        Pivots path;
        Path shortestPath;
        shortestPath[end] = begin;

        Nodes::iterator nIter;

        Cost currentCost;
        Nodes visitedNodes;
        visitedNodes.push_back(begin);

        nIter = this->nodes.begin();
        for ( ; nIter != this->nodes.end(); ++nIter ) {
            GraphNode* n = *nIter;
            if ( n != begin )
                currentCost[n] = this->cost(begin, n);
        }


        for ( size_t i=1; i < this->nodes.size(); ++i ) {

            // find min
            GraphNode* minNode = 0;
            nIter = this->nodes.begin();
            for ( ; nIter != this->nodes.end(); ++nIter ) {
                GraphNode* n = *nIter;
                Nodes::iterator visited = std::find( visitedNodes.begin(), 
                                                     visitedNodes.end(),
                                                     n );
                if ( visited != visitedNodes.end() ) 
                    continue;

                if ( minNode == 0 )
                    minNode = n;
                else {
                    int cost = currentCost[n];
                    int prevCost = currentCost[minNode];
                    if ( cost < prevCost )
                        minNode = n;
                }
            }
            visitedNodes.push_back(minNode);
            
            
            nIter = this->nodes.begin();
            for ( ; nIter != this->nodes.end(); ++nIter ) {
                GraphNode* n = *nIter;
                if ( !shortestPath.contains(n) ) 
                    shortestPath[n] = begin;
                Nodes::iterator visited = std::find( visitedNodes.begin(), 
                                                     visitedNodes.end(),
                                                     n );
                if ( visited != visitedNodes.end() ) 
                    continue;
                
                int cost = currentCost[n];
                int prevMinCost = currentCost[minNode];
                
                currentCost[n] = MIN(cost, prevMinCost + this->cost(minNode, n));
                if ( (prevMinCost + this->cost(minNode, n)) < cost ) {
                    shortestPath[n] = minNode;
                }            
            }        
        }
        
        GraphNode* prevNode = shortestPath[end];
        GraphPivot* p = this->pivot(prevNode, end);
        if ( p )
            path.push_back(p);
        while ( prevNode != begin ) {        
            GraphNode* node = prevNode;
            prevNode = shortestPath[prevNode];        
            path.push_back(this->pivot(prevNode, node));        
        }
        // Reverse the path.
        std::reverse( path.begin (), path.end () );
        return path;
    }


    int cost ( GraphNode* begin, GraphNode* end )
    {
        Pivots::iterator iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            GraphPivot* p = *iter;
            if ( p->firstNode == begin &&
                 p->lastNode == end ||
                 p->firstNode == end &&
                 p->lastNode == begin )
                return p->cost;
        }
        return MAX_INT;
    }    

    GraphPivot* pivot ( GraphNode* begin, GraphNode* end )
    {
        Pivots::iterator iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            GraphPivot* p = *iter;
            if (  p->firstNode == begin &&
                  p->lastNode == end || 
                  p->firstNode == end && 
                  p->lastNode == begin)
                return p;
        }
        return 0;
    } 

    Pivots nodePivots ( GraphNode* node )
    {
        Pivots res;
        Pivots::iterator iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            GraphPivot* p = *iter;
            if (  p->firstNode == node || 
                  p->lastNode == node )
                res.push_back(p);
        }
        return res;        
    }

    Nodes neighborNodes ( GraphNode* node )
    {
        Nodes nodes;
        Pivots::iterator iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            GraphPivot* p = *iter;
            if (  p->firstNode == node )
                nodes.push_back(p->lastNode);
            else if ( p->lastNode == node )
                nodes.push_back(p->firstNode);
        }
        return nodes;
    }

    Pivots sortNodePivotsByAngle ( GraphNode* node )
    {
        Pivots res = nodePivots(node);
        std::sort( res.begin(), res.end(), PivotsSortAlg(node) );
        return res;
    }

    Segments clockwiseSegments ( GraphNode* node )
    {
        Segments segments;
        Pivots pivots = sortNodePivotsByAngle(node);
        Pivots::iterator iter = pivots.begin();
        size_t indx = 0;
        for ( ; iter != pivots.end(); ++iter, ++indx ) {
            GraphPivot* left = *iter;
            GraphPivot* right;
            Pivots::iterator rightIter = iter + 1;
            if ( rightIter == pivots.end() )
                right = pivots.front();
            else 
                right = *rightIter;
            segments.push_back(Segment(indx, node, left, right));
        }       
        return segments;
    }

    void removeSimilarSegments ( Segments& segments )
    {
        Segments::iterator iter = segments.begin();
        for ( ; iter != segments.end(); ) {
            GraphPivot* left  = (*iter).left;
            GraphPivot* right = (*iter).right;
            bool erased = false;
            Segments::iterator innerIter = segments.begin();
            for ( ; innerIter != segments.end(); ++innerIter ) {
                if ( (*innerIter).index == (*iter).index )
                    continue;
                if ( left  == (*innerIter).left && 
                     right == (*innerIter).right ||
                     left  == (*innerIter).right && 
                     right == (*innerIter).left) {
                    segments.erase(iter);
                    erased = true;
                    break;
                }                     
            }
            if ( !erased )
                ++iter;
        }
    }

    bool segmentPathIsCorrect ( const Segment& seg, const Pivots& path )
    {
        //TODO: path should be sorted by direction!! 
        Nodes neighbors = neighborNodes( seg.centerNode ); 

        QPointArray points( path.size() + 2 );
        points.setPoint( 0, seg.centerNode->point() );

        Pivots::const_iterator iter = path.begin();
        size_t indx = 1;
        for ( ; iter != path.end(); ++iter, ++indx ) {            
            GraphPivot* pivot = *iter;

            // Check first node of pivot
            if ( seg.leftNode() != pivot->firstNode &&
                 seg.rightNode() != pivot->firstNode ) {
                Nodes::iterator nodeOnPath = std::find( neighbors.begin(), 
                                                        neighbors.end(), 
                                                        pivot->firstNode );
            if ( nodeOnPath != neighbors.end() )
                return false;
            }
            // Check last node of pivot
            if ( seg.leftNode() != pivot->lastNode &&
                 seg.rightNode() != pivot->lastNode ) {
                Nodes::iterator nodeOnPath = std::find( neighbors.begin(), 
                                                        neighbors.end(), 
                                                        pivot->lastNode );
            if ( nodeOnPath != neighbors.end() )
                return false;
            }


            points.setPoint( indx, pivot->firstNode->point() );
            if ( iter + 1 == path.end() ) {
                points.setPoint( ++indx, pivot->lastNode->point() );
            }
        }

        // Second check
        QRegion region( points );
        Nodes::iterator i = neighbors.begin();
        for ( ; i != neighbors.end(); ++i ) {
            GraphNode* n = *i;
            if ( n == seg.leftNode() || n == seg.rightNode() )
                continue;
            if ( region.contains( n->point() ) ) {
                return false;
            }            
        }
        return true;
    }

    Pivots nearestPivots ( const Segment& seg, const Pivots& path )
    {
        //TODO: path should be sorted by direction!! 
        QPointArray points( path.size() + 2 );
        Nodes pathNodes;

        points.setPoint( 0, seg.centerNode->point() );
        pathNodes.push_back( seg.centerNode );

        Pivots::const_iterator iter = path.begin();
        size_t indx = 1;
        for ( ; iter != path.end(); ++iter, ++indx ) {            
            GraphPivot* pivot = *iter;
            points.setPoint( indx, pivot->firstNode->point() );
            pathNodes.push_back(pivot->firstNode);
            if ( iter + 1 == path.end() ) {
                points.setPoint( ++indx, pivot->lastNode->point() );
                pathNodes.push_back(pivot->lastNode);
            }
        }

        Nodes nodesInSegment;
        QRegion region( points );
        Nodes::iterator i = nodes.begin();
        for ( ; i != nodes.end(); ++i ) {
            GraphNode* n = *i;
            Nodes::iterator onPath = std::find( pathNodes.begin(),
                                                pathNodes.end(),
                                                n );
            if ( onPath != pathNodes.end() )
                continue;
            if ( region.contains( n->point() ) )
                nodesInSegment.push_back(n);            
        }

        if ( nodesInSegment.size() == 0 )
            return path;

        Pivots nearest;
        GraphNode* left = seg.leftNode();
        while ( nodesInSegment.size() != 0 ) {
            Nodes::iterator i = nodesInSegment.begin();
            for ( ; i != nodesInSegment.end(); ++i ) {
                GraphNode* n = *i;
                if ( n == left )
                    continue;
                GraphPivot* p = pivot( left, n );
                if ( p != 0 ) {
                    left = n;
                    nearest.push_back(p);
                    nodesInSegment.erase(i);
                    break;
                }                    
            }
        }
        return nearest;        
    }

    Pivots surroundingPivots ( GraphNode* node )
    {
        Segments segments = clockwiseSegments(node);
        removeSimilarSegments(segments);
        size_t holesInSurroundings = 0;
        
        Pivots pathBeforeHole;
        Pivots pathAfterHole;

        Segments::iterator iter = segments.begin();
        for ( ; iter != segments.end(); ++iter ) {
            Segment seg = *iter;        
            GraphNode* left = seg.leftNode();
            GraphNode* right = seg.rightNode();

            seg.left->cost = MAX_INT;
            seg.right->cost = MAX_INT;

            Pivots path = shortestPath( left, right );

            seg.left->cost = 1;
            seg.right->cost = 1;

            if ( ! segmentPathIsCorrect( seg, path ) ) {
                if ( holesInSurroundings > 1 )
                    throw "bad";
                ++holesInSurroundings;
                continue;
            }

            Pivots nearestPath = nearestPivots( seg, path );

            Pivots* tmpPath = 0;
            if ( holesInSurroundings )
                tmpPath = &pathAfterHole;
            else 
                tmpPath = &pathBeforeHole;

            Pivots::iterator i = nearestPath.begin();
            for ( ; i != nearestPath.end(); ++i )
                tmpPath->push_back( *i );
        }

        Pivots path( pathBeforeHole.size() + pathAfterHole.size() );
        std::merge( pathAfterHole.begin(), pathAfterHole.end(),
                    pathBeforeHole.begin(), pathBeforeHole.end(),
                    path.begin() );
        
        return path;
    }

    Nodes nodes;
    Pivots pivots;
};


int main ()
{
    Graph graph;

    /*
    // r.pen_5.frm
    graph.nodes.push_back( new GraphNode(1, 0, 75) );
    graph.nodes.push_back( new GraphNode(2, 200, 225) );
    graph.nodes.push_back( new GraphNode(3, 300, 225) );
    graph.nodes.push_back( new GraphNode(4, 100, 150) );
    graph.nodes.push_back( new GraphNode(5, 300, 150) );
    graph.nodes.push_back( new GraphNode(6, 63,  208) );
    graph.nodes.push_back( new GraphNode(7, 148, 25) );
    graph.nodes.push_back( new GraphNode(8, 88, 25) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[0],
                                            graph.nodes[5],
                                            1 ) );
    graph.pivots.push_back( new GraphPivot( graph.nodes[0],
                                            graph.nodes[3],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[1],
                                            graph.nodes[2],
                                            1 ) );
    graph.pivots.push_back( new GraphPivot( graph.nodes[1],
                                            graph.nodes[4],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[2],
                                            graph.nodes[4],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[3],
                                            graph.nodes[1],
                                            1 ) );
    graph.pivots.push_back( new GraphPivot( graph.nodes[3],
                                            graph.nodes[6],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[4],
                                            graph.nodes[6],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[5],
                                            graph.nodes[1],
                                            1 ) );
    graph.pivots.push_back( new GraphPivot( graph.nodes[5],
                                            graph.nodes[3],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[6],
                                            graph.nodes[7],
                                            1 ) );


    graph.pivots.push_back( new GraphPivot( graph.nodes[7],
                                            graph.nodes[3],
                                            1 ) );
    graph.pivots.push_back( new GraphPivot( graph.nodes[7],
                                            graph.nodes[0],
                                            1 ) );
                                            */


    // r.pen_5.frm
    graph.nodes.push_back( new GraphNode(1, 0, 75) );
    graph.nodes.push_back( new GraphNode(2, 200, 225) );
    graph.nodes.push_back( new GraphNode(3, 300, 225) );
    graph.nodes.push_back( new GraphNode(4, 100, 150) );
    graph.nodes.push_back( new GraphNode(5, 300, 150) );
    graph.nodes.push_back( new GraphNode(6, 0,  272) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[5],
                                            graph.nodes[1],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[1],
                                            graph.nodes[2],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[2],
                                            graph.nodes[4],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[0],
                                            graph.nodes[4],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[5],
                                            graph.nodes[0],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[5],
                                            graph.nodes[3],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[3],
                                            graph.nodes[1],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[3],
                                            graph.nodes[0],
                                            1 ) );

    graph.pivots.push_back( new GraphPivot( graph.nodes[1],
                                            graph.nodes[4],
                                            1 ) );




    /*
    Segments segments = graph.clockwiseSegments( graph.nodes[3] );
    Segments::iterator iter = segments.begin();
    for ( ; iter != segments.end(); ++iter ) {
        Segment seg = *iter;        
        std::cout << seg.leftNode()->number << " , " <<
                     seg.rightNode()->number << "\n";
    }
    */


    /*
    Pivots pivotsOfNode = graph.sortNodePivotsByAngle( graph.nodes[3] );
    Pivots::iterator iter = pivotsOfNode.begin();
    for ( ; iter != pivotsOfNode.end(); ++iter ) {
        GraphPivot* pivot = *iter;        
        std::cout << pivot->firstNode->number << " ( " << pivot->cost <<
            " ) " << pivot->lastNode->number << "\n";
    }
    */

    /*
    Pivots path = graph.shortestPath( graph.nodes[1], graph.nodes[6] );
    Pivots::iterator iter = path.begin();
    for ( ; iter != path.end(); ++iter ) {
        GraphPivot* pivot = *iter;        
        std::cout << pivot->firstNode->number << " ( " << pivot->cost <<
            " ) " << pivot->lastNode->number << "\n";
    }
    */

    Pivots path = graph.surroundingPivots( graph.nodes[0] );
    Pivots::iterator iter = path.begin();
    for ( ; iter != path.end(); ++iter ) {
        GraphPivot* pivot = *iter;        
        std::cout << pivot->firstNode->number << " ( " << pivot->cost <<
            " ) " << pivot->lastNode->number << "\n";
    }

    return 0;
}

