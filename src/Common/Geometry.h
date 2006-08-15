
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>
#include <QPointF>

class DoublePoint;
class DoubleSize;

typedef DoublePoint Vector;

/*****************************************************************************
 * Subsidiary 2D functions
 *****************************************************************************/

inline bool comparePoints ( const DoublePoint&, const DoublePoint&,
                            double precision );

inline double getPointDistance ( const DoublePoint&, const DoublePoint& );

inline DoublePoint getLineSegmentsCrossPoint ( const DoublePoint&, 
                                               const DoublePoint&,
                                               const DoublePoint&, 
                                               const DoublePoint& );

// returns dot product which allows vector operations in arguments
inline double dotProduct ( const Vector&, const Vector& );

// returns length of vector
inline double normalize ( const Vector& );

// returns distance = norm of difference of two vectors
inline double normDifference ( const Vector&, const Vector& );

// returns the shortest distance from point p to line segment (p1,p2)
inline double pointToSegmentDist ( const DoublePoint& p, 
                                   const DoublePoint& p1,
                                   const DoublePoint& p2 );

/*****************************************************************************
 * Double Point
 *****************************************************************************/

class DoublePoint
{
public:
    DoublePoint ();
    DoublePoint ( double x, double y );
    DoublePoint ( const DoublePoint& );
    DoublePoint ( const QPointF& );

    double x () const;
    double y () const;
    double getX () const;
    double getY () const;

    const double& rx () const;
    const double& ry () const;
    double& rx ();
    double& ry ();

    void x ( double );
    void y ( double );
    void setX ( double );
    void setY ( double );

    DoublePoint& operator+= ( const DoublePoint& );
    DoublePoint& operator-= ( const DoublePoint& );
    DoublePoint& operator*= ( double );
    DoublePoint& operator/= ( double );

    friend inline bool operator== ( const DoublePoint&, const DoublePoint& );
    friend inline bool operator!= (  const DoublePoint&, const DoublePoint& );
    friend inline const DoublePoint operator+ ( const DoublePoint&, 
                                                const DoublePoint&  );
    friend inline const DoublePoint operator- (  const DoublePoint&, 
                                                 const DoublePoint& );
    friend inline const DoublePoint operator* ( const DoublePoint&, double );
    friend inline const DoublePoint operator* ( double, const DoublePoint& );
    friend inline const DoublePoint operator- ( const DoublePoint& );
    friend inline const DoublePoint operator/ ( const DoublePoint&, double );

private:
    double xp;
    double yp;
};

/*****************************************************************************
 * Double Point inline functions
 *****************************************************************************/

inline DoublePoint::DoublePoint () :
    xp(0), yp(0)
{}

inline DoublePoint::DoublePoint ( double x, double y ) :
    xp(x), yp(y)
{}

inline DoublePoint::DoublePoint ( const DoublePoint& p ) :
    xp(p.xp), yp(p.yp)
{}

inline DoublePoint::DoublePoint ( const QPointF& p ) :
    xp(p.x()), yp(p.y())
{}

inline double DoublePoint::x () const
{ return xp; }

inline double DoublePoint::y () const
{ return yp; }

inline double DoublePoint::getX () const
{ return xp; }

inline double DoublePoint::getY () const
{ return yp; }

inline const double& DoublePoint::rx () const
{ return xp; }

inline const double& DoublePoint::ry () const
{ return yp; }

inline double& DoublePoint::rx ()
{ return xp; }

inline double& DoublePoint::ry ()
{ return yp; }

inline void DoublePoint::x ( double x )
{ xp = x; }

inline void DoublePoint::y ( double y )
{ yp = y; }

inline void DoublePoint::setX ( double x )
{ xp = x; }

inline void DoublePoint::setY ( double y )
{ yp = y; }

inline DoublePoint& DoublePoint::operator+= ( const DoublePoint& p )
{ xp += p.xp; yp += p.yp; return *this; }

inline DoublePoint& DoublePoint::operator-= ( const DoublePoint& p )
{ xp -= p.xp; yp -= p.yp; return *this; }

inline DoublePoint& DoublePoint::operator*= ( double c )
{ xp = xp*c; yp = yp*c; return *this; }

inline DoublePoint& DoublePoint::operator/= ( double c )
{ xp = xp/c; yp = yp/c; return *this; }

inline bool operator== ( const DoublePoint& p1, const DoublePoint& p2 )
{ return p1.xp == p2.xp && p1.yp == p2.yp; }

inline bool operator!= (  const DoublePoint& p1, const DoublePoint& p2 )
{ return p1.xp != p2.xp || p1.yp != p2.yp; }

inline const DoublePoint operator+ ( const DoublePoint& p1, 
                                     const DoublePoint& p2 )
{ return DoublePoint(p1.xp+p2.xp, p1.yp+p2.yp); }

inline const DoublePoint operator- (  const DoublePoint& p1,
                                      const DoublePoint& p2 )
{ return DoublePoint(p1.xp-p2.xp, p1.yp-p2.yp); }

inline const DoublePoint operator* ( const DoublePoint& p, double c )
{ return DoublePoint(p.xp*c, p.yp*c); }

inline const DoublePoint operator* ( double c, const DoublePoint& p )
{ return DoublePoint(p.xp*c, p.yp*c); }

inline const DoublePoint operator- ( const DoublePoint& p )
{ return DoublePoint(-p.xp, -p.yp); }

inline const DoublePoint operator/ ( const DoublePoint& p, double c )
{ return DoublePoint(p.xp/c, p.yp/c); }

/*****************************************************************************
 * Double Size
 *****************************************************************************/

class DoubleSize
{
public:
    DoubleSize ();
    DoubleSize ( double w, double h );
    DoubleSize ( const DoubleSize& );

    double width () const;
    double height () const;
    double getWidth () const;
    double getHeight () const;

    const double& rwidth () const;
    const double& rheight () const;
    double& rwidth ();
    double& rheight ();

    void width ( double );
    void height ( double );
    void setWidth ( double );
    void setHeight ( double );

    DoubleSize& operator+= ( const DoubleSize& );
    DoubleSize& operator-= ( const DoubleSize& );
    DoubleSize& operator*= ( double );
    DoubleSize& operator/= ( double );

    friend inline bool operator== ( const DoubleSize&, const DoubleSize& );
    friend inline bool operator!= (  const DoubleSize&, const DoubleSize& );
    friend inline const DoubleSize operator+ ( const DoubleSize&, 
                                               const DoubleSize&  );
    friend inline const DoubleSize operator- (  const DoubleSize&, 
                                                const DoubleSize& );
    friend inline const DoubleSize operator* ( const DoubleSize&, double );
    friend inline const DoubleSize operator* ( double, const DoubleSize& );
    friend inline const DoubleSize operator- ( const DoubleSize& );
    friend inline const DoubleSize operator/ ( const DoubleSize&, double );

private:
    double wd;
    double ht;
};

/*****************************************************************************
 * Double Size inline functions
 *****************************************************************************/

inline DoubleSize::DoubleSize () :
    wd(0), ht(0)
{}

inline DoubleSize::DoubleSize ( double w, double h ) :
    wd(w), ht(h)
{}

inline DoubleSize::DoubleSize ( const DoubleSize& s ) :
    wd(s.wd), ht(s.ht)
{}

inline double DoubleSize::width () const
{ return wd; }

inline double DoubleSize::height () const
{ return ht; }

inline double DoubleSize::getWidth () const
{ return wd; }

inline double DoubleSize::getHeight () const
{ return ht; }

inline const double& DoubleSize::rwidth () const
{ return wd; }

inline const double& DoubleSize::rheight () const
{ return ht; }

inline double& DoubleSize::rwidth ()
{ return wd; }

inline double& DoubleSize::rheight ()
{ return ht; }

inline void DoubleSize::width ( double w )
{ wd = w; }

inline void DoubleSize::height ( double h )
{ ht = h; }

inline void DoubleSize::setWidth ( double w )
{ wd = w; }

inline void DoubleSize::setHeight ( double h )
{ ht = h; }

inline DoubleSize& DoubleSize::operator+= ( const DoubleSize& s )
{ wd += s.wd; ht += s.ht; return *this; }

inline DoubleSize& DoubleSize::operator-= ( const DoubleSize& s )
{ wd -= s.wd; ht -= s.ht; return *this; }

inline DoubleSize& DoubleSize::operator*= ( double c )
{ wd = wd*c; ht = ht*c; return *this; }

inline DoubleSize& DoubleSize::operator/= ( double c )
{ wd = wd/c; ht = ht/c; return *this; }

inline bool operator== ( const DoubleSize& s1, const DoubleSize& s2 )
{ return s1.wd == s2.wd && s1.ht == s2.ht; }

inline bool operator!= (  const DoubleSize& s1, const DoubleSize& s2 )
{ return s1.wd != s2.wd || s1.ht != s2.ht; }

inline const DoubleSize operator+ ( const DoubleSize& s1, 
                                     const DoubleSize& s2 )
{ return DoubleSize(s1.wd+s2.wd, s1.ht+s2.ht); }

inline const DoubleSize operator- (  const DoubleSize& s1,
                                      const DoubleSize& s2 )
{ return DoubleSize(s1.wd-s2.wd, s1.ht-s2.ht); }

inline const DoubleSize operator* ( const DoubleSize& s, double c )
{ return DoubleSize(s.wd*c, s.ht*c); }

inline const DoubleSize operator* ( double c, const DoubleSize& s )
{ return DoubleSize(s.wd*c, s.ht*c); }

inline const DoubleSize operator- ( const DoubleSize& s )
{ return DoubleSize(-s.wd, -s.ht); }

inline const DoubleSize operator/ ( const DoubleSize& s, double c )
{ return DoubleSize(s.wd/c, s.ht/c); }

/*****************************************************************************
 * Subsidiary 2D inline functions
 *****************************************************************************/

inline bool comparePoints ( const DoublePoint& p1, const DoublePoint& p2, 
                            double precision )
{
    if ( fabs( p1.x() - p2.x() ) < precision &&
         fabs( p1.y() - p2.y() ) < precision )
         return true;
    return false;
}

inline double getPointDistance ( const DoublePoint& p1, const DoublePoint& p2 )
{
    double dx = p2.x() - p1.x(),
           dy = p2.y() - p1.y();
    return sqrt( dx * dx + dy * dy );
}

inline DoublePoint getLineSegmentsCrossPoint ( const DoublePoint& p11, 
                                               const DoublePoint& p12,
                                               const DoublePoint& p21, 
                                               const DoublePoint& p22)
{
    // denominator
    double div  = ( p12.y() - p11.y() ) * ( p21.x() - p22.x() ) - 
                  ( p21.y() - p22.y() ) * ( p12.x() - p11.x() );
    
    // numerator1
    double mul1 = ( p12.y() - p11.y() ) * ( p21.x() - p11.x() ) - 
                  ( p21.y() - p11.y() ) * ( p12.x() - p11.x() );
    
    // numerator2
    double mul2 = ( p21.y() - p11.y() ) * ( p21.x() - p22.x() ) - 
                  ( p21.y() - p22.y() ) * ( p21.x() - p11.x() ); 

    if ( div == 0 && mul1 == 0 && mul2 == 0 )
        // line sgments are the same or they are parallel
        return DoublePoint( -1.0, -1.0 );

    // intersection indicators
    double Ua1 = mul1 / div;
    double Ua2 = mul2 / div;

    if ( ( 0 < Ua1 ) && ( Ua1 < 1.0 ) && 
         ( 0 < Ua2 ) && ( Ua2 < 1.0 ) ) 
    {
        // intersection point is within both line segments
        double x = p11.x() + ( p12.x() - p11.x() ) * Ua2;
        double y = p11.y() + ( p12.y() - p11.y() ) * Ua2;
        return DoublePoint( x, y );
    }
    else
        // intersection point is outside both line segments
        return DoublePoint( -1.0, -1.0 );    
}

inline double dotProduct ( const Vector& v1, const Vector& v2 ) 
{
    return ( v1.x() * v2.x() + v1.y() * v2.y() );
}

inline double normalize ( const Vector& v )
{
    return sqrt( dotProduct( v, v ) );
}

inline double normDifference ( const Vector& v1, const Vector& v2 )
{
    return normalize( v1 - v2 );
}

inline double pointToSegmentDist ( const DoublePoint& p, const DoublePoint& p1,
                                      const DoublePoint& p2 )
{
    Vector v1 = p2 - p1;
    Vector v2 = p - p1;

    double dotProd1 = dotProduct( v2, v1 );
    if ( dotProd1 <= 0 )
        return normDifference( p, p1 );

    double dotProd2 = dotProduct( v1, v1 );
    if ( dotProd2 <= dotProd1 )
        return normDifference( p, p2 );

    double koeff = dotProd1 / dotProd2;
    DoublePoint segmPnt = p1 + koeff * v1;
    return normDifference( p, segmPnt );
}

/*****************************************************************************/

#endif //GEOMETRY_H
