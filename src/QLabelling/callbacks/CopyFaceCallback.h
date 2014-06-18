#ifndef COPYFACECALLBACK_H
#define COPYFACECALLBACK_H

#include "Callback.h"

#include <QEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <CGAL/Qt/Converter.h>
#include "CurveGraphicsItem.h"
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_simple_point_location.h>
#include <CGAL/Arr_tags.h>
#include <CGAL/Arrangement_2.h>

#include "Utils.h"
#include "QArrangementLabellingWidget.h"
#include "QArrangementLabellingInfoWidget.h"
#include "QLabelItem.hpp"
#include "arrangement/ArrangementBuffer.h"
#include "CopyFaceSingleton.h"


class CopyFaceCallbackBase : public CGAL::Qt::Callback
{
public:
    CopyFaceCallbackBase( QObject* parent );
};

/**
   Supports visualization of point location on arrangements.

   The template parameter is a CGAL::Arrangement_with_history_2 of some type.
*/
template < class Arr_ >
class CopyFaceCallback : public CopyFaceCallbackBase
{
public:
    typedef Arr_ Arrangement;
    typedef typename Arrangement::Halfedge_handle Halfedge_handle;

    typedef typename Arrangement::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Arrangement::Halfedge_iterator Halfedge_iterator;
    typedef typename Arrangement::Face_handle Face_handle;
    typedef typename Arrangement::Face_const_handle Face_const_handle;
    typedef typename Arrangement::Vertex_const_handle Vertex_const_handle;
    typedef typename Arrangement::Halfedge_around_vertex_const_circulator
    Halfedge_around_vertex_const_circulator;
    typedef typename Arrangement::Geometry_traits_2 Traits;
    typedef typename Arrangement::Curve_handle Curve_handle;
    typedef typename Arrangement::Originating_curve_iterator
    Originating_curve_iterator;
    typedef typename Arrangement::Induced_edge_iterator Induced_edge_iterator;
    typedef typename Arrangement::Ccb_halfedge_const_circulator
    Ccb_halfedge_const_circulator;
    typedef typename Arrangement::Hole_const_iterator Hole_const_iterator;
    typedef typename Traits::X_monotone_curve_2 X_monotone_curve_2;
    typedef typename ArrTraitsAdaptor< Traits >::Kernel Kernel;
    typedef typename Kernel::Point_2 Kernel_point_2;
    typedef typename Traits::Point_2 Point_2;
    typedef typename Kernel::Segment_2 Segment_2;
    typedef typename CGAL::Arr_simple_point_location< Arrangement > SimplePointLocationStrategy;
    typedef typename CGAL::Arr_walk_along_line_point_location< Arrangement > Walk_pl_strategy;
    typedef CGAL::Arr_face_extended_dcel<Traits, int> Dcel;
    typedef CGAL::Arrangement_2<Traits, Dcel> Arrangement_2;
    CopyFaceCallback( Arrangement* arr_, QObject* parent_ );
    void reset( );


protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

    void CopyFace( QGraphicsSceneMouseEvent* event );
//    void highlightFace( QGraphicsSceneMouseEvent* event);


    Face_const_handle getFace( const CGAL::Object& o );
    CGAL::Object locate( const Kernel_point_2& point );
    CGAL::Object locate( const Kernel_point_2& point,
                         CGAL::Tag_false/*supportsLandmarks*/ );

    CGAL::Qt::Converter< Kernel > convert;
    CGAL::Object pointLocationStrategy;
    CGAL::Qt::CurveGraphicsItem< Traits >* highlightedCurve;
    Arrangement* arr;
    Arr_construct_point_2< Traits > toArrPoint;
}; // class CopyFaceCallback

/*! Constructor */
template < class Arr_ >
CopyFaceCallback<Arr_>::CopyFaceCallback(Arrangement* arr_, QObject* parent_):
    CopyFaceCallbackBase( parent_ ),
    pointLocationStrategy( CGAL::make_object( new Walk_pl_strategy( *arr_ ) ) ),
     highlightedCurve( new CGAL::Qt::CurveGraphicsItem< Traits >( ) ),
    arr( arr_ )
{QObject::connect( this, SIGNAL( modelChanged( ) ),
                   this->highlightedCurve, SLOT( modelChanged( ) ) );
}

template < class Arr_ >
void CopyFaceCallback< Arr_ >::reset( )
{
    emit modelChanged( );
}

template < class Arr_ >
void CopyFaceCallback<Arr_>::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    ArrangementBuffer::instance()->push_back(arr);
    this->CopyFace( event );
    emit modelChanged( );
}

//WIP debut
template < class Arr_ >
void
CopyFaceCallback< Arr_ >::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
//    this->highlightFace( event );
    //emit modelChanged( );
}
//WIP fin

template < class Arr_ >
void
CopyFaceCallback< Arr_ >::
CopyFace( QGraphicsSceneMouseEvent* event )
{   if(CopyFaceSingleton::instance()->getmode()==1){
        if(CopyFaceSingleton::instance()->getfisrtclick()==1){
            CopyFaceSingleton::instance()->firstclick_ok();
            return;
        }
        Kernel_point_2 point = this->convert( event->scenePos( ) );
        CGAL::Object pointLocationResult = this->locate( point );
        Face_const_handle face = this->getFace( pointLocationResult );
        //VERY WIP 10/06/2014
        Ccb_halfedge_const_circulator curr;
        CopyFaceSingleton::instance()->clear();
        if (! face->is_unbounded()) {
            curr = face->outer_ccb();
            do {
                CopyFaceSingleton::instance()->addPoint(curr->source()->point());
                ++curr;
             } while (curr != face->outer_ccb());
             CopyFaceSingleton::instance()->createRelativeVertex(point);
             CopyFaceSingleton::instance()->switchmode();
       }

        //end WIP
        Face_handle f = this->arr->non_const_handle( face );
    }
    else
        return;

}
//debut de WIP
//template < class Arr_ >
//void
//CopyFaceCallback< Arr_ >::
//highlightFace(QGraphicsSceneMouseEvent* event)
//{
//    Kernel_point_2 point = this->convert( event->scenePos( ) );
//    CGAL::Object pointLocationResult = this->locate( point );
//    Face_const_handle face = this->getFace( pointLocationResult );
//    Face_handle f = this->arr->non_const_handle( face );
//    Ccb_halfedge_const_circulator curr;
//    curr = face->outer_ccb();
//    if (! face->is_unbounded()) {
//        curr = face->outer_ccb();
//        do {
//            Halfedge_iterator h=curr->source();
//            Originating_curve_iterator ocit, temp;
//            ocit = this->arr->originating_curves_begin( h );
//            while (ocit != this->arr->originating_curves_end(h))
//            {
//                temp = ocit;
//                ++temp;

//                Curve_handle ch = ocit;
//                Induced_edge_iterator itr;
//                for ( itr = this->arr->induced_edges_begin( ch );
//                      itr != this->arr->induced_edges_end( ch );
//                      ++itr )
//                {
//                    X_monotone_curve_2 curve = (*curr)->curve( );
//                    this->highlightedCurve->insert( curve );
//                }
//                ocit = temp;
//            }
//                 emit modelChanged( );


//            ++curr;
//         } while (curr != face->outer_ccb());
//    }


//}





//fin de WIP
template < class Arr_ >
typename CopyFaceCallback< Arr_ >::Face_const_handle
CopyFaceCallback< Arr_ >::getFace( const CGAL::Object& obj )
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

template < class Arr_ >
CGAL::Object CopyFaceCallback< Arr_ >::locate( const Kernel_point_2& point )
{
    typename Supports_landmarks< Arrangement >::Tag supportsLandmarks;
    return this->locate( point, supportsLandmarks );
}

template < class Arr_ >
CGAL::Object
CopyFaceCallback< Arr_ >::locate( const Kernel_point_2& pt, CGAL::Tag_false )
{
    CGAL::Object pointLocationResult;
    Walk_pl_strategy* walkStrategy;
    SimplePointLocationStrategy* simpleStrategy;

    Point_2 point = this->toArrPoint( pt );

    if ( CGAL::assign( walkStrategy, this->pointLocationStrategy ) )
    {
        pointLocationResult = walkStrategy->locate( point );
    }
    else if ( CGAL::assign( simpleStrategy, this->pointLocationStrategy ) )
    {
        pointLocationResult = simpleStrategy->locate( point );
    }
    return pointLocationResult;
}

#endif // COPYFACECALLBACK_H
