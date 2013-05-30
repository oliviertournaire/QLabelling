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
#include <CGAL/Arr_polyline_traits_2.h>
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
	    std::cout << "Insertion d'un point (premier de la ligne) en " << clickedPoint << "." << std::endl;
            
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
	    std::cout << "Insertion d'un point en " << clickedPoint << "." << std::endl;

            if ( event->button( ) == ::Qt::RightButton )
            { // finalize polyline input
                
                // Destruction de la Polyline courante
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
                this->polylineGuide.push_back( lineItem ); // Ajout à l'objet Polyline de ce nouveau segment
                if ( this->scene != NULL )
                {
                    this->scene->addItem( this->polylineGuide.back( ) ); // Ajout à la scène du dernier segment (celui qui vient d'être ajouté)
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

    std::vector< QGraphicsLineItem* > polylineGuide; // Polyline courante
};

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_GRAPHICS_VIEW_SEGMENT_INPUT_H
