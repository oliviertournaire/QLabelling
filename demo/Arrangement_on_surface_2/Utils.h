// Copyright (c) 2012  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: $
// $Id: $
//
// Author(s)     : Alex Tsui <alextsui05@gmail.com>

#ifndef CGAL_ARRANGEMENTS_DEMO_UTILS_H
#define CGAL_ARRANGEMENTS_DEMO_UTILS_H

#include <CGAL/iterator.h>
#include <CGAL/Qt/Converter.h>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arr_walk_along_line_point_location.h>

#include "ArrangementTypes.h"

class QGraphicsScene;

class QGraphicsSceneMixin
{
public:
    /*! Costructor */
    QGraphicsSceneMixin( ) : scene( 0 ) { }

    /*! Destructor (virtual) */
    virtual ~QGraphicsSceneMixin() {}

    virtual void setScene( QGraphicsScene* scene_ ) { this->scene = scene_; }

    virtual QGraphicsScene* getScene( ) const { return this->scene; }

    virtual QRectF viewportRect( ) const
    {
        QRectF res;
        if ( this->scene == NULL )
        {
            return res;
        }

        QList< QGraphicsView* > views = this->scene->views( );
        if ( views.size( ) == 0 )
        {
            return res;
        }
        // assumes the first view is the right one
        QGraphicsView* viewport = views.first( );
        QPointF p1 = viewport->mapToScene( 0, 0 );
        QPointF p2 = viewport->mapToScene(viewport->width(), viewport->height());

        double xmin = std::min( p1.x( ), p2.x( ) );
        double xmax = std::max( p1.x( ), p2.x( ) );
        double ymin = std::min( p1.y( ), p2.y( ) );
        double ymax = std::max( p1.y( ), p2.y( ) );


        res = QRectF( QPointF( xmin, ymin ), QPointF( xmax, ymax ) );

        return res;
    }

    QPoint fromScene( QPointF p, bool* ok = 0 )
    {
        QPoint res;
        if ( this->scene == NULL )
        {
            if ( ok ) { *ok = false; }
            return res;
        }
        QList< QGraphicsView* > views = this->scene->views( );
        if ( views.size( ) == 0 )
        {
            if ( ok ) { *ok = false; }
            return res;
        }

        // assumes the first view is the right one
        QGraphicsView* viewport = views.first( );

        if ( ok ) { *ok = true; }
        res = viewport->mapFromScene( p );
        return res;
    }

    QPointF toScene( QPoint p, bool* ok = 0 )
    {
        QPointF res;
        if ( this->scene == NULL )
        {
            if ( ok ) { *ok = false; }
            return res;
        }
        QList< QGraphicsView* > views = this->scene->views( );
        if ( views.size( ) == 0 )
        {
            if ( ok ) { *ok = false; }
            return res;
        }

        // assumes the first view is the right one
        QGraphicsView* viewport = views.first( );

        if ( ok ) { *ok = true; }
        res = viewport->mapToScene( p );
        return res;
    }

    int fromScene( double d, bool* ok = 0 )
    {
        QPointF p( d, 0 );
        QPoint pp = this->fromScene( p, ok );
        return pp.x( );
    }

    double toScene( int i, bool* ok = 0 )
    {
        QPoint p( i, 0 );
        QPointF pp = this->toScene( p, ok );
        return pp.x( );
    }

protected: // fields
    QGraphicsScene* scene;
};

BOOST_MPL_HAS_XXX_TRAIT_DEF( Approximate_2 )

template < class Arr_, bool b = has_Approximate_2< Arr_ >::value >
        struct Supports_landmarks
{
        typedef CGAL::Boolean_tag< b > Tag;
        struct LandmarksType { };
        };

template < class Arr_ >
struct Supports_landmarks< Arr_, true >
{
    typedef CGAL::Tag_true Tag;
    typedef CGAL::Arr_landmarks_point_location< Arr_ > LandmarksType;
};

/**
   Support for new ArrTraits should specify types:

   * Kernel - a not-necessarily-exact kernel to represent the arrangement
   graphically. We'll use the Point_2 type provided by this kernel for
   computing distances
   * Point_2 - the point type used in the particular arrangement
   * CoordinateType - the coordinate type used by the point type
   */
template < class ArrTraits >
class ArrTraitsAdaptor
{ };

template < class Kernel_ >
class ArrTraitsAdaptor< CGAL::Arr_segment_traits_2< Kernel_ > >
{
public:
    typedef Kernel_ Kernel;
    typedef CGAL::Arr_segment_traits_2< Kernel > ArrTraits;
    typedef typename ArrTraits::Point_2 Point_2;
    typedef typename Kernel::FT CoordinateType;
};

template < class Kernel_ >
class ArrTraitsAdaptor< CGAL::Arr_linear_traits_2< Kernel_ > >
{
public:
    typedef Kernel_ Kernel;
    typedef CGAL::Arr_linear_traits_2< Kernel > ArrTraits;
    typedef typename ArrTraits::Point_2 Point_2;
    typedef typename Kernel::FT CoordinateType;
};

template < class SegmentTraits >
class ArrTraitsAdaptor< CGAL::Arr_polyline_traits_2< SegmentTraits > >
{
public:
    typedef CGAL::Arr_polyline_traits_2< SegmentTraits > ArrTraits;
    typedef typename SegmentTraits::Kernel Kernel;
    typedef typename ArrTraits::Point_2 Point_2;
    typedef typename Kernel::FT CoordinateType;
};

template < class ArrTraits >
class Compute_squared_distance_2_base : public QGraphicsSceneMixin
{
public:
    typedef CGAL::Cartesian< double > InexactKernel;

public:
    // ctors
    Compute_squared_distance_2_base( ) { }

public: // methods

    template < class T1, class T2 >
    double operator() ( const T1& t1, const T2& t2 )
    {
        return this->squaredDistance( t1, t2 );
    }

protected: // fields
    typename Kernel::Compute_squared_distance_2 squared_distance;
    InexactKernel::Compute_squared_distance_2 squaredDistance;
};

template < class ArrTraits >
class Compute_squared_distance_2 :
        public Compute_squared_distance_2_base< ArrTraits >
{ };

template < class Kernel_ >
class Compute_squared_distance_2< CGAL::Arr_polyline_traits_2< Kernel_ > > :
        public Compute_squared_distance_2_base<CGAL::Arr_polyline_traits_2<Kernel_> >
{
public:
    typedef Kernel_ Kernel;
    typedef CGAL::Arr_polyline_traits_2< Kernel > Traits;
    typedef Compute_squared_distance_2_base< Traits > Superclass;
    typedef typename Kernel::FT FT;
    typedef typename Kernel::Point_2 Point_2;
    typedef typename Kernel::Segment_2 Segment_2;
    typedef typename Traits::Curve_2 Curve_2;
    typedef typename Curve_2::const_iterator Curve_const_iterator;
    typedef typename Traits::X_monotone_curve_2 X_monotone_curve_2;

    double operator() ( const Point_2& p, const X_monotone_curve_2& c ) const
    {
        Curve_const_iterator ps = c.begin();
        Curve_const_iterator pt = ps; pt++;
        bool first = true;
        FT min_dist = 0;

        while ( pt != c.end() )
        {
            const Point_2& source = *ps;
            const Point_2& target = *pt;
            Segment_2 seg( source, target );
            FT dist = this->squared_distance( p, seg );

            if ( first || dist < min_dist )
            {
                first = false;
                min_dist = dist;
            }
            ps++; pt++;
        }

        return CGAL::to_double( min_dist );
    }
};

template < class ArrTraits >
class Arr_compute_y_at_x_2 : public QGraphicsSceneMixin
{
public:
    typedef ArrTraits Traits;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef typename ArrTraitsAdaptor< Traits >::CoordinateType CoordinateType;
    // typedef typename Kernel::FT FT;
    typedef typename Kernel::Point_2                      Point_2;
    typedef typename Kernel::Line_2                       Line_2;
    typedef typename Traits::X_monotone_curve_2           X_monotone_curve_2;
    typedef typename Traits::Multiplicity                 Multiplicity;
    typedef typename Traits::Intersect_2                  Intersect_2;
    typedef std::pair< typename Traits::Point_2, Multiplicity >
    IntersectionResult;

    /*! Constructor */
    Arr_compute_y_at_x_2( ) :
        intersectCurves( this->traits.intersect_2_object( ) )
    { }

    /*! Destructor (virtual) */
    virtual ~Arr_compute_y_at_x_2() {}

    CoordinateType operator() ( const X_monotone_curve_2& curve,
                                const CoordinateType& x )
    {
        typename Traits::Left_side_category category;
        return this->operator()( curve, x, this->traits, category );
    }

    double approx( const X_monotone_curve_2& curve, const CoordinateType& x )
    {
        return CGAL::to_double( (*this)( curve, x ) );
    }

protected:
    template < class TTraits >
    CoordinateType operator() ( const X_monotone_curve_2& curve,
                                const CoordinateType& x, TTraits traits_,
                                CGAL::Arr_oblivious_side_tag )
    {
        typedef typename TTraits::Construct_x_monotone_curve_2
                Construct_x_monotone_curve_2;
        Construct_x_monotone_curve_2 construct_x_monotone_curve_2 =
                traits_.construct_x_monotone_curve_2_object( );
        CoordinateType res( 0 );
        CGAL::Bbox_2 clipRect = curve.bbox( );
        Point_2 p1c1( x, CoordinateType( clipRect.ymin( ) - 1 ) ); // clicked point
        // upper bounding box
        Point_2 p2c1( x, CoordinateType( clipRect.ymax( ) + 1 ) );

        const X_monotone_curve_2 verticalLine =
                construct_x_monotone_curve_2( p1c1, p2c1 );
        CGAL::Object o;
        CGAL::Oneset_iterator< CGAL::Object > oi( o );

        this->intersectCurves( curve, verticalLine, oi );

        IntersectionResult pair;
        if ( CGAL::assign( pair, o ) )
        {
            Point_2 pt = pair.first;
            res = pt.y( );
        }
        return res;
    }

    template < class TTraits >
    CoordinateType operator() ( const X_monotone_curve_2& curve,
                                const CoordinateType& x, TTraits traits_,
                                CGAL::Arr_open_side_tag )
    {
        typename TTraits::Construct_x_monotone_curve_2
                construct_x_monotone_curve_2 =
                traits_.construct_x_monotone_curve_2_object( );
        CoordinateType res( 0 );
        // QRectF clipRect = this->viewportRect( );
        Line_2 line = curve.supporting_line( );
        // FIXME: get a better bounding box for an unbounded segment
        Point_2 p1c1( x, CoordinateType( -10000000 ) ); // clicked point
        Point_2 p2c1( x, CoordinateType(  10000000 ) ); // upper bounding box

        const X_monotone_curve_2 verticalLine =
                construct_x_monotone_curve_2( p1c1, p2c1 );
        CGAL::Object o;
        CGAL::Oneset_iterator< CGAL::Object > oi( o );

        this->intersectCurves( curve, verticalLine, oi );

        IntersectionResult pair;
        if ( CGAL::assign( pair, o ) )
        {
            Point_2 pt = pair.first;
            res = pt.y( );
        }
        return res;
    }

protected:
    Traits traits;
    Intersect_2 intersectCurves;
};

#undef SUBCURVE_1

// TODO: Make Construct_x_monotone_subcurve_2 more generic
template < class ArrTraits >
class Construct_x_monotone_subcurve_2
{
public:
    typedef typename ArrTraitsAdaptor<ArrTraits>::Kernel  Kernel;
    typedef typename ArrTraits::X_monotone_curve_2        X_monotone_curve_2;
    typedef typename ArrTraits::Split_2                   Split_2;
    typedef typename ArrTraits::Intersect_2               Intersect_2;
    typedef typename ArrTraits::Multiplicity              Multiplicity;
    typedef typename ArrTraits::Construct_min_vertex_2    Construct_min_vertex_2;
    typedef typename ArrTraits::Construct_max_vertex_2    Construct_max_vertex_2;
    typedef typename ArrTraits::Compare_x_2               Compare_x_2;
    typedef typename Kernel::FT                           FT;
    typedef typename ArrTraitsAdaptor< ArrTraits >::CoordinateType
    CoordinateType;
    typedef typename ArrTraits::Point_2                   Point_2;
    typedef typename Kernel::Point_2                      Kernel_point_2;
    //typedef typename Kernel::Line_2 Line_2;
    //typedef typename Kernel::Compute_y_at_x_2 Compute_y_at_x_2;

    Construct_x_monotone_subcurve_2( ):
        intersect_2( this->traits.intersect_2_object( ) ),
        split_2( this->traits.split_2_object( ) ),
        compare_x_2( this->traits.compare_x_2_object( ) ),
        construct_min_vertex_2( this->traits.construct_min_vertex_2_object( ) ),
        construct_max_vertex_2( this->traits.construct_max_vertex_2_object( ) )
    { }

    /*
    Return the subcurve of curve bracketed by pLeft and pRight.

    We assume pLeft and pRight don't lie on the curve and always do a vertical
    projection.
  */
    X_monotone_curve_2 operator() ( const X_monotone_curve_2& curve,
                                    const Point_2& pLeft, const Point_2& pRight )
    {
        Point_2 pMin = this->construct_min_vertex_2( curve );
        Point_2 pMax = this->construct_max_vertex_2( curve );
        X_monotone_curve_2 subcurve;
        X_monotone_curve_2 unusedTrimmings;
        X_monotone_curve_2 finalSubcurve;
        if ( this->compare_x_2( pLeft, pMin ) == CGAL::LARGER )
        {
            CoordinateType y1 = this->compute_y_at_x( curve, pLeft.x( ) );
            Point_2 splitPoint( pLeft.x( ), y1 );
            this->split_2( curve, splitPoint, unusedTrimmings, subcurve );
        }
        else
        {
            subcurve = curve;
        }

        if ( this->compare_x_2( pRight, pMax ) == CGAL::SMALLER )
        {
            CoordinateType y2 = this->compute_y_at_x( subcurve, pRight.x( ) );
            Point_2 splitPoint( pRight.x( ), y2 );
            this->split_2( subcurve, splitPoint, finalSubcurve, unusedTrimmings );
        }
        else
        {
            finalSubcurve = subcurve;
        }

        return finalSubcurve;
    }

protected:
    ArrTraits traits;
    Intersect_2 intersect_2;
    Split_2 split_2;
    Compare_x_2 compare_x_2;
    Arr_compute_y_at_x_2< ArrTraits > compute_y_at_x;
    Construct_min_vertex_2 construct_min_vertex_2;
    Construct_max_vertex_2 construct_max_vertex_2;
}; // class Construct_x_monotone_subcurve_2


// FIXME: return Traits::Point_2 instead of Kernel::Point_2
template < class ArrTraits >
class SnapStrategy : public QGraphicsSceneMixin
{
public:
    //typedef typename ArrTraitsAdaptor< ArrTraits >::Kernel Kernel;
    typedef typename ArrTraits::Point_2 Point_2;

    virtual Point_2 snapPoint( QGraphicsSceneMouseEvent* event ) = 0;

protected:
    SnapStrategy( QGraphicsScene* scene_ );
}; // class SnapStrategy

template < class ArrTraits >
SnapStrategy< ArrTraits >::SnapStrategy( QGraphicsScene* scene_ )
{ 
    this->scene = scene_;
}

template < class ArrTraits >
class SnapToGridStrategy : public SnapStrategy< ArrTraits >
{
public:
    typedef typename ArrTraitsAdaptor< ArrTraits >::Kernel Kernel;
    typedef typename ArrTraits::Point_2                   Point_2;
    typedef typename Kernel::Point_2                      Kernel_point_2;
    typedef SnapStrategy< ArrTraits >                     Superclass;

    /*! Constructors */
    SnapToGridStrategy( ) :
        Superclass( NULL ),
        gridSize( 50 )
    { }

    SnapToGridStrategy( QGraphicsScene* scene ) :
        Superclass( scene ),
        gridSize( 50 )
    { }

    /*! Destructors (virtual) */
    ~SnapToGridStrategy() {}

    Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
    {
        return this->snapPoint( event, ArrTraits( ) );
    }

    template < class TTraits >
    Point_2 snapPoint(QGraphicsSceneMouseEvent* event, TTraits /* traits */)
    {
        QPointF clickedPoint = event->scenePos( );
        QRectF viewportRect = this->viewportRect( );
        if ( viewportRect == QRectF( ) )
        { // fallback case; we usually shouldn't end up here
            Kernel_point_2 res = this->convert( event->scenePos( ) );
            return Point_2( CGAL::to_double(res.x( )), CGAL::to_double(res.y()) );
        }

        qreal d( this->gridSize / 2.0 );
        int left = int( viewportRect.left( ) ) -
                (int( viewportRect.left( ) ) % this->gridSize);
        int right = int( viewportRect.right( ) ) +
                (this->gridSize - int( viewportRect.right( ) ) % this->gridSize);
        int x = int(clickedPoint.x( ));
        int y = int(clickedPoint.y( ));
        for ( int i = left - this->gridSize; i <= right; i += this->gridSize )
        {
            if ( i - d <= clickedPoint.x( ) && clickedPoint.x( ) <= i + d )
            {
                x = i;
                break;
            }
        }
        int top = int( viewportRect.top( ) ) -
                (int( viewportRect.top( ) ) % this->gridSize);
        int bottom = int( viewportRect.bottom( ) ) +
                (this->gridSize - int( viewportRect.bottom( ) ) % this->gridSize);
        for ( int i = top - this->gridSize; i <= bottom; i += this->gridSize )
        {
            if ( i - d <= clickedPoint.y( ) && clickedPoint.y( ) <= i + d )
            {
                y = i;
                break;
            }
        }
        //return this->convert( QPointF( x, y ) );
        Point_2 res( x, y );
        return res;
    }

    void setGridSize( int size )
    {
        this->gridSize = size;
    }

protected:
    int gridSize;
    CGAL::Qt::Converter< Kernel > convert;
}; // class SnapToGridStrategy

template < class Arr_ >
class SnapToArrangementVertexStrategy:
        public SnapStrategy< typename Arr_::Geometry_traits_2 >
{
public:
    typedef Arr_                                          Arrangement;
    typedef typename Arrangement::Geometry_traits_2       Traits;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef SnapStrategy< Traits >                        Superclass;
    typedef typename Arrangement::Vertex_iterator         Vertex_iterator;
    typedef typename Kernel::Compute_squared_distance_2
    Compute_squared_distance_2;
    typedef typename Kernel::FT                           FT;
    typedef typename Traits::Point_2                      Point_2;
    typedef typename Kernel::Point_2                      Kernel_point_2;

    SnapToArrangementVertexStrategy( ):
        Superclass( NULL ),
        arrangement( NULL )
    { }

    SnapToArrangementVertexStrategy( Arrangement* arr, QGraphicsScene* scene_ ):
        Superclass( scene_ ),
        arrangement( arr )
    { }

    Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
    {
        Kernel_point_2 clickedPoint = this->convert( event->scenePos( ) );
        return this->snapPoint( clickedPoint, Traits( ) );
    }

    template < class TTraits >
    Point_2 snapPoint(const Kernel_point_2& clickedPoint, TTraits /* traits */)
    {
        Point_2 initialPoint( CGAL::to_double(clickedPoint.x()),
                              CGAL::to_double(clickedPoint.y()) );
        Point_2 closestPoint( CGAL::to_double(clickedPoint.x()),
                              CGAL::to_double(clickedPoint.y()) );
        bool first = true;
        FT minDist( 0 );
        QRectF viewportRect = this->viewportRect( );
        if ( viewportRect == QRectF( ) )
        {
            return initialPoint;
        }

        FT maxDist( ( viewportRect.right( ) - viewportRect.left( ) ) / 4.0 );
        for ( Vertex_iterator vit = this->arrangement->vertices_begin( );
              vit != this->arrangement->vertices_end( ); ++vit )
        {
            Point_2 point = vit->point( );
            Kernel_point_2 thisPoint( CGAL::to_double(point.x()),
                                      CGAL::to_double(point.y()) );
            FT dist = this->compute_squared_distance_2( clickedPoint, thisPoint );
            if ( first || ( dist < minDist ) )
            {
                first = false;
                minDist = dist;
                closestPoint = point;
            }
        }
        if ( ! first && minDist < maxDist )
        {
            return closestPoint;
        }
        else
        {
            return initialPoint;
        }
    }

    void setArrangement( Arrangement* arr )
    {
        this->arrangement = arr;
    }

protected:
    Arrangement* arrangement;
    Compute_squared_distance_2 compute_squared_distance_2;
    CGAL::Qt::Converter< Kernel > convert;
}; // class SnapToArrangementVertexStrategy

/**
   Converts between Kernel points and Arrangement points.

   The conversion is not necessarily exact.
*/
template < class ArrTraits >
class Arr_construct_point_2
{
    typedef typename ArrTraits::Point_2                            Point_2;
    typedef typename ArrTraitsAdaptor< ArrTraits >::CoordinateType CoordinateType;
    typedef typename ArrTraitsAdaptor< ArrTraits >::Kernel         Kernel;
    typedef typename Kernel::Point_2                               Kernel_point_2;

public:
    Point_2 operator()( const Kernel_point_2& pt )
    {
        return (*this)( pt.x(), pt.y() );
    }

    template < class T >
    Point_2 operator()( const T& x, const T& y )
    {
        return (*this)( x, y, ArrTraits( ) );
    }

protected:
    template < class T, class TTraits >
    Point_2 operator()(const T& x, const T& y, TTraits /* traits */)
    {
        CoordinateType xx( x );
        CoordinateType yy( y );
        Point_2 res( xx, yy );
        return res;
    }
};

class Find_nearest_edge_base : public QGraphicsSceneMixin
{
public:
    /*! Destructor (virtual) */
    virtual ~Find_nearest_edge_base() {}
};

template < class Arr_, class ArrTraits = typename Arr_::Geometry_traits_2 >
class Find_nearest_edge : public Find_nearest_edge_base
{
public: // typedefs
    typedef Arr_ Arrangement;
    //typedef typename Arrangement::Geometry_traits_2 ArrTraits;
    typedef Compute_squared_distance_2< ArrTraits > Point_curve_distance;
    typedef typename ArrTraits::X_monotone_curve_2 X_monotone_curve_2;
    typedef CGAL::Arr_walk_along_line_point_location< Arrangement >
    Point_location_strategy;
    typedef typename ArrTraitsAdaptor<ArrTraits>::Kernel  Kernel;
    typedef typename Kernel::Point_2                      Point_2;
    typedef typename Arrangement::Face_const_handle       Face_const_handle;
    typedef typename Arrangement::Halfedge_const_handle   Halfedge_const_handle;
    typedef typename Arrangement::Vertex_const_handle     Vertex_const_handle;
    typedef typename Arrangement::Ccb_halfedge_const_circulator
    Ccb_halfedge_const_circulator;
    typedef typename Point_curve_distance::FT             FT;
    typedef typename Arrangement::Hole_const_iterator     Hole_const_iterator;
    typedef typename Arrangement::Halfedge_around_vertex_const_circulator
    Halfedge_around_vertex_const_circulator;

public:
    /*! constructor */
    Find_nearest_edge( Arrangement* arr_ ) :
        Find_nearest_edge_base( ),
        arr( arr_ ),
        pointLocationStrategy( Point_location_strategy( *arr_ ) )
    { }

    /*! Destructor (virtual) */
    virtual ~Find_nearest_edge() {}

public: // member methods
    Halfedge_const_handle operator()( const Point_2& queryPt )
    {
        typename ArrTraits::Point_2 pt = this->toArrPoint( queryPt );
        CGAL::Object pointLocationResult = this->pointLocationStrategy.locate( pt );
        Face_const_handle face = this->getFace( pointLocationResult );
        bool first = 1;
        X_monotone_curve_2 closestCurve;
        Halfedge_const_handle closestEdge;
        double minDist( 0 );

        if ( ! face->is_unbounded( ) )
        { // it is an interior face so it has a ccb
            Ccb_halfedge_const_circulator cc = face->outer_ccb( );
            do
            {
                X_monotone_curve_2 curve = cc->curve( );
                double dist = this->pointCurveDistance( queryPt, curve );
                if ( first || dist < minDist )
                {
                    first = 0;
                    minDist = dist;
                    closestEdge = cc;
                }
            }
            while ( ++cc != face->outer_ccb( ) );
        }

        Hole_const_iterator hit;
        Hole_const_iterator eit = face->holes_end( );
        // int counter = 0;
        for ( hit = face->holes_begin( ); hit != eit; ++hit )
        { // check any holes inside this face
            Ccb_halfedge_const_circulator cc = *hit;
            do
            {
                X_monotone_curve_2 curve = cc->curve( );
                double dist = this->pointCurveDistance( queryPt, curve );
                if ( first || dist < minDist )
                {
                    first = 0;
                    minDist = dist;
                    closestEdge = cc;
                }
                cc++;
            }
            while ( cc != *hit );
        }

        return closestEdge;
    }

    virtual void setScene( QGraphicsScene* scene_ )
    {
        this->pointCurveDistance.setScene( scene_ );
        Find_nearest_edge_base::setScene( scene_ );
    }

protected: // member methods
    Face_const_handle getFace( const CGAL::Object& obj )
    {
        Face_const_handle f;
        if ( CGAL::assign( f, obj ) )
            return f;

        Halfedge_const_handle he;
        if (CGAL::assign( he, obj ))
            return (he->face( ));

        Vertex_const_handle v;
        CGAL_assertion(CGAL::assign( v, obj ));
        CGAL::assign( v, obj );
        if ( v->is_isolated( ) )
            return v->face( );
        Halfedge_around_vertex_const_circulator eit = v->incident_halfedges( );
        return  (eit->face( ));
    }

protected: // member fields
    Arrangement* arr;
    Point_curve_distance pointCurveDistance;
    Point_location_strategy pointLocationStrategy;
    Arr_construct_point_2< ArrTraits > toArrPoint;

}; // class Find_nearest_edge

#endif // CGAL_ARRANGEMENTS_DEMO_UTILS_H
