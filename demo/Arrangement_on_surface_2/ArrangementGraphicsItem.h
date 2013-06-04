// Copyright (c) 2008, 2012  GeometryFactory Sarl (France).
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

#ifndef CGAL_QT_ARRANGEMENT_GRAPHICS_ITEM_H
#define CGAL_QT_ARRANGEMENT_GRAPHICS_ITEM_H

#include <CGAL/Bbox_2.h>
#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/Converter.h>
#include <CGAL/Arr_polyline_traits_2.h>

#include <QGraphicsScene>
#include <QPainter>

#include "ArrangementPainterOstream.h"
#include "Utils.h"
#include <iostream>

class QGraphicsScene;

namespace CGAL {
namespace Qt {

class ArrangementGraphicsItemBase :
        public GraphicsItem, public QGraphicsSceneMixin
{
public:
    ArrangementGraphicsItemBase( );

    const QPen& getVerticesPen( ) const;
    const QPen& getEdgesPen( ) const;
    void setVerticesPen( const QPen& pen );
    void setEdgesPen( const QPen& pen );
    bool visibleVertices( ) const;
    void setVisibleVertices( const bool b );
    bool visibleEdges( ) const;
    void setVisibleEdges( const bool b );
    void setBackgroundColor( QColor color );
    
    QGraphicsScene* getScene() const;

protected:
    CGAL::Bbox_2 bb;
    bool bb_initialized;

    QPen verticesPen;
    QPen edgesPen;
    bool visible_edges;
    bool visible_vertices;

    QGraphicsScene* scene;

    QColor backgroundColor;

}; // class ArrangementGraphicsItemBase

template <typename Arr_, typename ArrTraits = typename Arr_::Geometry_traits_2>
class ArrangementGraphicsItem : public ArrangementGraphicsItemBase
{
    typedef Arr_ Arrangement;
    typedef typename Arrangement::Geometry_traits_2       Traits;
    typedef typename Arrangement::Vertex_iterator         Vertex_iterator;
    typedef typename Arrangement::Curve_iterator          Curve_iterator;
    typedef typename Arrangement::Edge_iterator           Edge_iterator;
    typedef typename Arrangement::Halfedge                Halfedge;
    typedef typename Arrangement::Halfedge_handle         Halfedge_handle;
    typedef typename Arrangement::Face_handle             Face_handle;
    typedef typename Arrangement::Face_iterator           Face_iterator;
    typedef typename Arrangement::Hole_iterator           Holes_iterator;
    typedef typename Arrangement::Ccb_halfedge_circulator Ccb_halfedge_circulator;

    typedef typename ArrTraitsAdaptor< Traits >::Kernel   Kernel;
    typedef typename Traits::X_monotone_curve_2           X_monotone_curve_2;
    typedef typename Kernel::Point_2                      Kernel_point_2;
    typedef typename Traits::Point_2                      Point_2;
    //typedef typename Kernel::Segment_2 Segment_2;

    typedef ArrangementGraphicsItemBase                   Superclass;
    typedef typename Kernel::Segment_2                    Segment_2;

public:
    /*! Constructor */
    ArrangementGraphicsItem( Arrangement* t_ );

    /*! Destructor (virtual) */
    ~ArrangementGraphicsItem() {}

public:
    void modelChanged( );
    QRectF boundingRect( ) const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

protected:
    template < typename TTraits >
    void paint( QPainter* painter, TTraits traits );

    void paintFaces( QPainter* painter )
    {
        typename Traits::Left_side_category category;
        this->paintFaces( painter, category );
    }

    void paintFaces( QPainter* painter, CGAL::Arr_oblivious_side_tag )
    {
        for( Face_iterator fi = this->arr->faces_begin( );
             fi != this->arr->faces_end( ); ++fi )
        {
            fi->set_visited( false );
        }

        Face_handle unboundedFace = this->arr->unbounded_face( );
        this->paintFace( unboundedFace, painter );
    }

    void paintFaces( QPainter* painter, CGAL::Arr_open_side_tag )
    {
        for( Face_iterator fi = this->arr->faces_begin( );
             fi != this->arr->faces_end( ); ++fi )
        {
            // if ( fi->is_fictitious( ) )
            // {
            //     std::cout << "setting fictitious face not visited" << std::endl;
            // }
            fi->set_visited( false );
        }

        Face_handle fictitiousFace = this->arr->fictitious_face( );
        this->paintFace( fictitiousFace, painter );
    }

    void paintFace( Face_handle f, QPainter* painter )
    {
        if (! f->visited( ) )
        {
            int holes = 0;
            int inner_faces = 0;
            Holes_iterator hit; // holes iterator
            this->paintFace( f, painter, Traits( ) );
            f->set_visited( true );
            for ( hit = f->holes_begin(); hit != f->holes_end(); ++hit )
            {
                Ccb_halfedge_circulator cc = *hit;
                do {
                    Halfedge_handle he = cc;
                    Halfedge_handle he2 = he->twin();
                    Face_handle inner_face = he2->face();
                    if ( this->antenna( he ) )
                        continue;

                    // move on to next hole
                    if ( ! inner_face->visited( ) )
                        inner_faces++;
                    this->visit_ccb_faces( inner_face, painter );
                } while ( ++cc != *hit );
                holes++;
            }// for
        }
    }

    void visit_ccb_faces( Face_handle & fh, QPainter* painter )
    {
        this->paintFace( fh, painter );

        Ccb_halfedge_circulator cc=fh->outer_ccb();
        do {
            Halfedge he = *cc;
            if (! he.twin()->face()->visited())
            {
                Face_handle nei = (Face_handle) he.twin()->face();
                this->visit_ccb_faces( nei , painter );
            }
            //created from the outer boundary of the face
        } while (++cc != fh->outer_ccb());
    }

    /*! antenna - return true if the halfedge and its
   *  twin point to the same face.
   */
    bool antenna(Halfedge_handle h)
    {
        Halfedge_handle twin = h->twin();
        return (twin->face() == h->face());
    }

    template < typename Traits >
    void paintFace(Face_handle /* f */, QPainter* /* painter */,
                   Traits /* traits */)
    { }
    
    template < typename Kernel_ >
    void paintFace( Face_handle f, QPainter* painter,
                    CGAL::Arr_polyline_traits_2< Kernel_ > )
    {
        if (!f->is_unbounded())  // f is not the unbounded face
        {
            QVector< QPointF > pts; // holds the points of the polygon
            typename X_monotone_curve_2::const_iterator           pt_itr;
            typename X_monotone_curve_2::const_reverse_iterator   pt_rev_itr;
            X_monotone_curve_2 cv;

            /* running with around the outer of the face and generate from it
       * polygon
       */
            Ccb_halfedge_circulator cc = f->outer_ccb();
            do {
                cv = cc->curve();
                bool curve_has_same_direction =
                        ( *(cc->curve().begin()) == cc->source()->point() );
                if ( curve_has_same_direction )
                {
                    for( pt_itr = cv.begin() , ++pt_itr ; pt_itr != cv.end(); ++pt_itr)
                    {
                        double x = CGAL::to_double((*pt_itr).x());
                        double y = CGAL::to_double((*pt_itr).y());
                        QPointF coord_source(x , y);
                        pts.push_back(coord_source );
                    }
                }
                else
                {
                    for (pt_rev_itr = cv.rbegin() , ++pt_rev_itr; pt_rev_itr != cv.rend();
                         ++pt_rev_itr)
                    {
                        double x = CGAL::to_double((*pt_rev_itr).x());
                        double y = CGAL::to_double((*pt_rev_itr).y());
                        QPointF coord_source(x , y);
                        pts.push_back(coord_source );
                    }
                }
                //created from the outer boundary of the face
            } while (++cc != f->outer_ccb());

            // make polygon from the outer ccb of the face 'f'
            QPolygonF pgn( pts );

            // fill the face according to its color (stored at any of her
            // incidents curves)
            QBrush oldBrush = painter->brush( );
            QColor def_bg_color = this->backgroundColor;
            if (! f->color().isValid())
            {
                painter->setBrush( def_bg_color );
            }
            else
            {
                painter->setBrush( f->color( ) );
            }
            painter->drawPolygon( pgn );
            painter->setBrush( oldBrush );
        }
        else
        {
            QRectF rect = this->viewportRect( );
            QColor color = this->backgroundColor;
            painter->fillRect( rect, color );
        }
    }

    /**
     Return false if the tip of the given curve doesn't align with either of the
     endpoints of the next curve.
  */
    bool isProperOrientation( Ccb_halfedge_circulator cc )
    {
        Ccb_halfedge_circulator ccnext = cc;
        Halfedge_handle he = cc;
        X_monotone_curve_2 thisCurve = he->curve( );
        ccnext++;
        while ( this->antenna( ccnext ) ) ccnext++;
        Halfedge_handle next_he = ccnext;
        X_monotone_curve_2 nextCurve = next_he->curve( );

        QPointF thisTarget( to_double(thisCurve.target().x()),
                            to_double(thisCurve.target().y()) );
        QPointF nextSource( to_double(nextCurve.source().x()),
                            to_double(nextCurve.source().y()) );
        QPointF nextTarget( to_double(nextCurve.target().x()),
                            to_double(nextCurve.target().y()) );
        double dist1 = QLineF( thisTarget, nextSource ).length();
        double dist2 = QLineF( thisTarget, nextTarget ).length();
        bool res = ( dist1 < 1e-2 || dist2 < 1e-2 );

        return res;
    }

    bool pathTouchingSource( const QPainterPath& path, X_monotone_curve_2 c )
    {
        QPointF a = path.currentPosition( );
        QPointF b( to_double(c.source().x()), to_double(c.source().y()) );
        QPointF d( to_double(c.target().x()), to_double(c.target().y()) );
        bool res = (QLineF( a, b ).length() < 1e-2);

        return res;
    }

    void updateBoundingBox();

    template < typename TTraits>
    void updateBoundingBox(TTraits traits );

    Arrangement* arr;
    ArrangementPainterOstream< Traits > painterostream;
    CGAL::Qt::Converter< Kernel > convert;
    std::map< Curve_iterator, CGAL::Bbox_2 > curveBboxMap;
}; // class ArrangementGraphicsItem

template < typename Arr_, class ArrTraits >
ArrangementGraphicsItem< Arr_, ArrTraits >::
ArrangementGraphicsItem( Arrangement* arr_ ):
    arr( arr_ ),
    painterostream( 0 )
{
    if ( this->arr->number_of_vertices( ) == 0 ) {
        this->hide( );
    }
    this->updateBoundingBox( );
    this->setZValue( 3 );
}

template < typename Arr_, typename ArrTraits >
QRectF
ArrangementGraphicsItem< Arr_, ArrTraits >::
boundingRect( ) const
{
    QRectF rect = this->convert( this->bb );
    return rect;
}

template < typename Arr_, typename ArrTraits >
void
ArrangementGraphicsItem< Arr_, ArrTraits >::
paint(QPainter* painter,
      const QStyleOptionGraphicsItem* /* option */,
      QWidget*  /*widget*/)
{
    this->paint( painter, ArrTraits( ) );
}

template < typename Arr_, typename ArrTraits >
template < typename TTraits >
void ArrangementGraphicsItem< Arr_, ArrTraits >::
paint(QPainter* painter, TTraits /* traits */)
{
    this->paintFaces( painter );

    painter->setPen( this->verticesPen );
    this->painterostream =
            ArrangementPainterOstream< Traits >( painter, this->boundingRect( ) );
    this->painterostream.setScene( this->scene );

    for ( Vertex_iterator it = this->arr->vertices_begin( );
          it != this->arr->vertices_end( ); ++it )
    {
        Point_2 p = it->point( );
        Kernel_point_2 pt( p.x( ), p.y( ) );
        this->painterostream << pt;
    }
    painter->setPen( this->edgesPen );
    for ( Edge_iterator it = this->arr->edges_begin( );
          it != this->arr->edges_end( ); ++it )
    {
        X_monotone_curve_2 curve = it->curve( );
        this->painterostream << curve;
    }
}

// We let the bounding box only grow, so that when vertices get removed
// the maximal bbox gets refreshed in the GraphicsView
template < typename Arr_, typename ArrTraits >
void ArrangementGraphicsItem< Arr_, ArrTraits >::updateBoundingBox( )
{
    this->updateBoundingBox( ArrTraits( ) );
}

template < typename Arr_, typename ArrTraits >
template < typename TTraits >
void ArrangementGraphicsItem< Arr_, ArrTraits >::
updateBoundingBox(TTraits /* traits */)
{
    this->prepareGeometryChange( );
    if ( this->arr->number_of_vertices( ) == 0 )
    {
        this->bb = Bbox_2( 0, 0, 0, 0 );
        this->bb_initialized = false;
        return;
    }
    else
    {
        this->bb = this->arr->vertices_begin( )->point( ).bbox( );
        this->bb_initialized = true;
    }

    for ( Curve_iterator it = this->arr->curves_begin( );
          it != this->arr->curves_end( );
          ++it )
    {
        if ( this->curveBboxMap.count( it ) == 0 )
        {
            this->curveBboxMap[ it ] = it->bbox( );
        }
        this->bb = this->bb + this->curveBboxMap[ it ];
    }
}

template < typename Arr_, typename ArrTraits >
void ArrangementGraphicsItem< Arr_, ArrTraits >::modelChanged( )
{
    if ( this->arr->is_empty( ) )
    {
        this->hide( );
    }
    else
    {
        this->show( );
    }
    this->updateBoundingBox( );
    this->update( );
}

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_ARRANGEMENT_GRAPHICS_ITEM_H
