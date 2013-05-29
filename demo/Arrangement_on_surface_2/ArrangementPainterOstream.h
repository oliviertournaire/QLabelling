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

#ifndef CGAL_QT_ARRANGEMENT_PAINTER_OSTREAM_H
#define CGAL_QT_ARRANGEMENT_PAINTER_OSTREAM_H

#include <QRectF>
#include <vector>

// TODO: should be included in PainterOstream.h
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Qt/Converter.h>
#include <CGAL/Arr_polyline_traits_2.h>

#include "Utils.h"

// #include <CGAL/Curved_kernel_via_analysis_2/Curve_renderer_facade.h>

class QPainter;

namespace CGAL {
namespace Qt {

template < typename ArrTraits >
class ArrangementPainterOstreamBase : public QGraphicsSceneMixin
{
public:
    // typedefs
    typedef ArrTraits Traits;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef typename Kernel::Point_2                      Point_2;
    typedef typename Kernel::Segment_2                    Segment_2;
    typedef typename Kernel::Ray_2                        Ray_2;
    typedef typename Kernel::Line_2                       Line_2;
    typedef typename Kernel::Triangle_2                   Triangle_2;
    typedef typename Kernel::Iso_rectangle_2              Iso_rectangle_2;
    typedef typename Kernel::Circle_2                     Circle_2;

public:
    /*! Constructor */
    ArrangementPainterOstreamBase( QPainter* p,
                                   QRectF clippingRectangle = QRectF( ) ) :
        painterOstream( p, clippingRectangle ),
        qp( p ),
        convert( clippingRectangle ),
        // scene( NULL ),
        clippingRect( QRectF( ) ), // null rectangle
        scale( 1.0 )
    {
        if ( p != 0 )
        {
            this->scale = p->worldTransform( ).m11( );
        }
    }

    /*! Destructor (virtual) */
    virtual ~ArrangementPainterOstreamBase() {}

    // methods
    template < typename T >
    ArrangementPainterOstreamBase& operator<<( const T& t )
    {
        this->painterOstream << t;
        return *this;
    }

    void setScene( QGraphicsScene* scene_ )
    {
        this->scene = scene_;

        // set the clipping rectangle
        if ( scene_ == NULL )
        {
            return;
        }
        this->clippingRect = this->viewportRect( );
        this->convert = Converter< Kernel >( this->clippingRect );
    }

#if 0
    void setScene( QGraphicsScene* scene_ )
    {
        this->scene = scene_;

        // set the clipping rectangle
        if ( scene_ == NULL )
        {
            return;
        }
        this->clippingRect = this->getViewportRect( );
    }
#endif

protected: // methods
#if 0
    QRectF getViewportRect( ) const
    {
        // assumes scene is not null and attached to exactly one view
        QGraphicsView* view = this->scene->views( ).first( );
        QPointF p1 = view->mapToScene( 0, 0 );
        QPointF p2 = view->mapToScene( view->width( ), view->height( ) );
        QRectF clipRect = QRectF( p1, p2 );

        return clipRect;
    }
#endif

protected:
    // fields
    PainterOstream< Kernel > painterOstream;
    QPainter* qp;
    Converter< Kernel > convert;
    // QGraphicsScene* scene;
    QRectF clippingRect;
    double scale;

}; // class ArrangementPainterOstreamBase

template < typename ArrTraits >
class ArrangementPainterOstream :
        public ArrangementPainterOstreamBase< ArrTraits >
{
public:
    /*! Constructor */
    ArrangementPainterOstream(QPainter* p, QRectF clippingRectangle = QRectF()):
        ArrangementPainterOstreamBase< ArrTraits >( p, clippingRectangle )
    { }

    /*! Destructor (virtual) */
    virtual ~ArrangementPainterOstream() {}
};

template < typename SegmentTraits >
class ArrangementPainterOstream<CGAL::Arr_polyline_traits_2<SegmentTraits> > :
        public ArrangementPainterOstreamBase<CGAL::Arr_polyline_traits_2<
        SegmentTraits> >
{
public: // typedefs
    typedef ArrangementPainterOstreamBase<CGAL::Arr_polyline_traits_2<
    SegmentTraits> > Superclass;
    typedef typename Superclass::Traits                   Traits;
    typedef typename Superclass::Kernel                   Kernel;
    typedef typename Superclass::Point_2                  Point_2;
    typedef typename Superclass::Segment_2                Segment_2;
    typedef typename Superclass::Ray_2                    Ray_2;
    typedef typename Superclass::Line_2                   Line_2;
    typedef typename Superclass::Triangle_2               Triangle_2;
    typedef typename Superclass::Iso_rectangle_2          Iso_rectangle_2;
    typedef typename Superclass::Circle_2                 Circle_2;
    typedef typename Traits::Curve_2                      Curve_2;
    typedef typename Traits::X_monotone_curve_2           X_monotone_curve_2;

public:
    /*! Constructor */
    ArrangementPainterOstream(QPainter* p, QRectF clippingRectangle = QRectF()):
        Superclass( p, clippingRectangle )
    { }

    /*! Destructor (virtual) */
    virtual ~ArrangementPainterOstream() {}

public: // methods
    ArrangementPainterOstream& operator<<( const X_monotone_curve_2& curve )
    {
        for ( unsigned int i = 0; i < curve.size( ); ++i )
        {
            Segment_2 segment = curve[ i ];
            this->painterOstream << segment;
        }
        // TODO: implement polyline painting
#if 0
        const Point_2& p1 = curve.source( );
        const Point_2& p2 = curve.target( );
        Segment_2 seg( p1, p2 );
        this->painterOstream << seg;
#endif
        return *this;
    }

    // cloned from segtraits painter
    ArrangementPainterOstream& operator<<( const Point_2& p )
    {
        QPointF qpt = this->convert( p );
        QPen savePen = this->qp->pen( );
        this->qp->setBrush( QBrush( savePen.color( ) ) );
        double radius = savePen.width( ) / 2.0;
        radius /= this->scale;

        this->qp->drawEllipse( qpt, radius, radius );

        this->qp->setBrush( QBrush( ) );
        this->qp->setPen( savePen );
        return *this;
    }

    template < typename T >
    ArrangementPainterOstream& operator<<( const T& p )
    {
        (*(static_cast< Superclass* >(this)) << p);
        return *this;
    }
};

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_ARRANGEMENT_PAINTER_OSTREAM_H
