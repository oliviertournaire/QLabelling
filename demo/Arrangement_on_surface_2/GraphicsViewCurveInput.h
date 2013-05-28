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

#ifndef CGAL_QT_GRAPHICS_VIEW_CURVE_INPUT_H
#define CGAL_QT_GRAPHICS_VIEW_CURVE_INPUT_H

#include <iostream>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Qt/GraphicsViewInput.h>
#include <CGAL/Qt/Converter.h>
#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

#include "Callback.h"
#include "ISnappable.h"
#include "PointsGraphicsItem.h"

namespace CGAL {
namespace Qt {

class GraphicsViewCurveInputBase :
    public GraphicsViewInput, public ISnappable, public QGraphicsSceneMixin
{
public:
  /**
     Add our helper graphics items to the scene.
     @override
  */
  virtual void setScene( QGraphicsScene* scene_ );
  //virtual QGraphicsScene* getScene( ) const;

  void setSnappingEnabled( bool b );
  void setSnapToGridEnabled( bool b );
  virtual void setColor( QColor c );
  QColor getColor( ) const;

protected:
  GraphicsViewCurveInputBase( QObject* parent );
  virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
  virtual void mousePressEvent( QGraphicsSceneMouseEvent* event );
  virtual bool eventFilter( QObject* obj, QEvent* event );

  PointsGraphicsItem pointsGraphicsItem; // shows user specified curve points
  bool snappingEnabled;
  bool snapToGridEnabled;
  QColor color;

}; // class GraphicsViewCurveInputBase

template < typename ArrTraits >
class GraphicsViewCurveInput : public GraphicsViewCurveInputBase { };

/**
   Specialization of GraphicsViewCurveInput for Arr_segment_traits_2; handles
   user-guided generation of line segment curves.
*/
template < typename Kernel_ >
class GraphicsViewCurveInput< CGAL::Arr_segment_traits_2< Kernel_ > >:
  public GraphicsViewCurveInputBase
{
public:
  typedef Kernel_ Kernel;
  typedef CGAL::Arr_segment_traits_2< Kernel > Traits;
  typedef typename Traits::Curve_2 Curve_2;
  typedef typename Kernel::Point_2 Point_2;
  typedef typename Kernel::Segment_2 Segment_2;

  GraphicsViewCurveInput( QObject* parent ):
    GraphicsViewCurveInputBase( parent ),
    second( false )
  {
    this->segmentGuide.setZValue( 100 );
    this->setColor( this->color );
  }

  void setColor( QColor c )
  {
    this->GraphicsViewCurveInputBase::setColor( c );

    QPen pen = this->segmentGuide.pen( );
    pen.setColor( this->color );
    this->segmentGuide.setPen( pen );
  }

protected:
  void mouseMoveEvent( QGraphicsSceneMouseEvent* event )
  {
    if ( this->second )
    {
      Point_2 clickedPoint = this->snapPoint( event );
      Segment_2 segment( this->p1, clickedPoint );
      QLineF qSegment = this->convert( segment );
      this->segmentGuide.setLine( qSegment );
    }
  }

  void mousePressEvent( QGraphicsSceneMouseEvent* event )
  {
    if ( !this->second )
    {
      this->second = true;
      this->p1 = this->snapPoint( event );
      QPointF pt = this->convert( this->p1 );
      this->segmentGuide.setLine( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
      if ( this->scene != NULL )
      {
        this->scene->addItem( &( this->segmentGuide ) );
      }
      this->pointsGraphicsItem.insert( pt );
    }
    else
    {
      this->second = false;
      this->p2 = this->snapPoint( event );
      if ( this->scene != NULL )
      {
        this->scene->removeItem( &( this->segmentGuide ) );
      }
      if ( traits.compare_xy_2_object()( this->p1, this->p2 ) == CGAL::EQUAL )
      {
        return;
      }
      this->pointsGraphicsItem.clear( );
      Curve_2 res( this->p1, this->p2 );
      emit generate( CGAL::make_object( res ) );
    }
  }

  // override this to snap to the points you like
  virtual Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
  {
    Point_2 clickedPoint = this->convert( event->scenePos( ) );
    return clickedPoint;
  }

  Traits traits;
  Converter< Kernel > convert;
  Point_2 p1;
  Point_2 p2;
  bool second;

  QGraphicsLineItem segmentGuide;
}; // class GraphicsViewCurveInput< CGAL::Arr_segment_traits_2< Kernel_ > >

/**
   Specialization of GraphicsViewCurveInput for Arr_polyline_traits_2; handles
   user-guided generation of line segment curves.
*/
template < typename SegmentTraits >
class GraphicsViewCurveInput< CGAL::Arr_polyline_traits_2< SegmentTraits > >:
  public GraphicsViewCurveInputBase
{
public:
  typedef CGAL::Arr_polyline_traits_2< SegmentTraits > Traits;
  typedef typename Traits::Curve_2 Curve_2;
  typedef typename SegmentTraits::Kernel Kernel;
  typedef typename Kernel::Point_2 Point_2;
  typedef typename Kernel::Segment_2 Segment_2;

  GraphicsViewCurveInput( QObject* parent ):
    GraphicsViewCurveInputBase( parent )
  { }

protected:
  void mouseMoveEvent( QGraphicsSceneMouseEvent* event )
  {
    if ( ! this->polylineGuide.empty( ) )
    {
      Point_2 clickedPoint = this->snapPoint( event );
      // TODO: make it work for the latest line segment
      Segment_2 segment( this->points.back( ), clickedPoint );
      QLineF qSegment = this->convert( segment );
      this->polylineGuide.back( )->setLine( qSegment );
    }
  }

  void mousePressEvent( QGraphicsSceneMouseEvent* event )
  {
    Point_2 clickedPoint = this->snapPoint( event );
    if ( this->points.empty( ) )
    { // first
      // add clicked point to polyline
      this->points.push_back( clickedPoint );

      QPointF pt = this->convert( clickedPoint );
      QGraphicsLineItem* lineItem =
        new QGraphicsLineItem( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
      lineItem->setZValue( 100 );
      QPen pen = lineItem->pen( );
      pen.setColor( this->color );
      lineItem->setPen( pen );
      this->polylineGuide.push_back( lineItem );
      if ( this->scene != NULL )
      {
        this->scene->addItem( this->polylineGuide.back( ) );
      }
    }
    else
    {
      // add clicked point to polyline
      this->points.push_back( clickedPoint );

      if ( event->button( ) == ::Qt::RightButton )
      { // finalize polyline input
        for ( unsigned int i = 0; i < this->polylineGuide.size( ); ++i )
        {
          if ( this->scene != NULL )
          {
            this->scene->removeItem( this->polylineGuide[ i ] );
          }
          delete this->polylineGuide[ i ];
        }
        this->polylineGuide.clear( );
        Curve_2 res( this->points.begin( ), this->points.end( ) );
        this->points.clear( );

        emit generate( CGAL::make_object( res ) );
      }
      else
      { // start the next segment
        QPointF pt = this->convert( clickedPoint );
        QGraphicsLineItem* lineItem =
          new QGraphicsLineItem( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
        lineItem->setZValue( 100 );
        QPen pen = lineItem->pen( );
        pen.setColor( this->color );
        lineItem->setPen( pen );
        this->polylineGuide.push_back( lineItem );
        if ( this->scene != NULL )
        {
          this->scene->addItem( this->polylineGuide.back( ) );
        }
      }
    }
  }

  // override this to snap to the points you like
  virtual Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
  {
    Point_2 clickedPoint = this->convert( event->scenePos( ) );
    return clickedPoint;
  }

  Converter< Kernel > convert;
  std::vector< Point_2 > points;

  std::vector< QGraphicsLineItem* > polylineGuide;
};

/**
   Specialization of GraphicsViewCurveInput for Arr_linear_traits_2; handles
   user-guided generation of line segment curves.
*/
template < typename Kernel_ >
class GraphicsViewCurveInput< CGAL::Arr_linear_traits_2< Kernel_ > >:
  public GraphicsViewCurveInputBase
{
public: // typedefs
  typedef Kernel_ Kernel;
  typedef GraphicsViewCurveInputBase Superclass;
  typedef CGAL::Arr_linear_traits_2< Kernel > Traits;
  typedef typename Traits::Curve_2 Curve_2;
  typedef typename Kernel::Point_2 Point_2;
  typedef typename Kernel::Segment_2 Segment_2;
  typedef typename Kernel::Ray_2 Ray_2;
  typedef typename Kernel::Line_2 Line_2;
  enum CurveType
    {
      SEGMENT, RAY, LINE
    };

public: // constructors
  GraphicsViewCurveInput( QObject* parent ):
    GraphicsViewCurveInputBase( parent ),
    second( false ),
    curveType( SEGMENT )
  {
    this->setColor( this->color );
  }

public: // methods
  void setCurveType( CurveType type )
  {
    this->curveType = type;
  }

  void setColor( QColor c )
  {
    this->GraphicsViewCurveInputBase::setColor( c );
    QPen pen = this->segmentGuide.pen( );
    pen.setColor( c );
    this->segmentGuide.setPen( pen );
  }

protected: // methods
  virtual bool eventFilter( QObject* obj, QEvent* event )
  {
    // before we do anything, update the clipping rect
    // TODO: somehow only update this when the view changes
    QRectF clippingRect = this->viewportRect( );
    if ( !clippingRect.isValid( ) )
    {
      //std::cout << "Warning: invalid clipping rect" << std::endl;
    }
    this->convert = Converter< Kernel >( clippingRect );

    // now handle the event
    return Superclass::eventFilter( obj, event );
  }

  void mouseMoveEvent( QGraphicsSceneMouseEvent* event )
  {
    if ( this->second )
    {
      Point_2 hoverPoint = this->snapPoint( event );
      if ( p1 == hoverPoint )
        return;
      QLineF qSegment;
      if ( this->curveType == SEGMENT )
      {
        Segment_2 segment( this->p1, hoverPoint );
        qSegment = this->convert( segment );
      }
      else if ( this->curveType == RAY )
      {
        Ray_2 ray( this->p1, hoverPoint );
        qSegment = this->convert( ray );
      }
      else // this->curveType == LINE
      {
        Line_2 line( this->p1, hoverPoint );
        qSegment = this->convert( line );
      }
      this->segmentGuide.setLine( qSegment );
    }
  }

  void mousePressEvent( QGraphicsSceneMouseEvent* event )
  {
    if ( !this->second )
    { // fix our first point
      this->second = true;
      this->p1 = this->snapPoint( event );
      QPointF pt = this->convert( this->p1 );
      this->segmentGuide.setLine( pt.x( ), pt.y( ), pt.x( ), pt.y( ) );
      if ( this->scene != NULL )
      {
        this->scene->addItem( &( this->segmentGuide ) );
      }
    }
    else // this->second == true
    {
      this->second = false;
      this->p2 = this->snapPoint( event );

      // skip if degenerate
      if ( this->p1 == this->p2 )
        return;

      if ( this->scene != NULL )
      {
        this->scene->removeItem( &( this->segmentGuide ) );
      }

      Curve_2 res;
      if ( this->curveType == SEGMENT )
      {
        res = Curve_2( Segment_2( this->p1, this->p2 ) );
      }
      else if ( this->curveType == RAY )
      {
        res = Curve_2( Ray_2( this->p1, this->p2 ) );
      }
      else // this->curveType == LINE
      {
        res = Curve_2( Line_2( this->p1, this->p2 ) );
      }

      emit generate( CGAL::make_object( res ) );
    }
  }

  // override this to snap to the points you like
  virtual Point_2 snapPoint( QGraphicsSceneMouseEvent* event )
  {
    Point_2 clickedPoint = this->convert( event->scenePos( ) );
    return clickedPoint;
  }

protected: // fields
  Converter< Kernel > convert;
  Point_2 p1;
  Point_2 p2;
  bool second;

  QGraphicsLineItem segmentGuide;
  CurveType curveType;
}; // class GraphicsViewCurveInput< CGAL::Arr_linear_traits_2< Kernel_ > >

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_GRAPHICS_VIEW_SEGMENT_INPUT_H
