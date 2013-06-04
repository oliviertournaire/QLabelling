#ifndef __ARRANGEMENT_OBSERVER_H
#define __ARRANGEMENT_OBSERVER_H__

#include <CGAL/Arr_observer.h>

#include "QLabellingLogWidget.hpp"

template <class Arrangement_>
class Arrangement_Observer : public CGAL::Arr_observer<Arrangement_>
{
public:

    typedef Arrangement_                                  Arrangement;
    typedef CGAL::Arr_observer<Arrangement>               Arr_observer;
    typedef typename Arrangement::Face_handle             Face_handle;

    Arrangement_Observer (Arrangement& arr) : Arr_observer (arr) {}

    /// Issued just before a new vertex that corresponds to the point p is created
    virtual void before_create_vertex(Point_2 p)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after a new vertex v has been created. Note that the vertex still has no incident edges and is not connected to any other vertex.
    virtual void after_create_vertex(Vertex_handle v)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after a new vertex v has been created. Note that the vertex still has no incident edges and is not connected to any other vertex.
    virtual void after_create_boundary_vertex(Vertex_handle v)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued just before a new edge that corresponds to the x-monotone curve c and connects the vertices v1 and v2 is created.
    virtual void before_create_edge( X_monotone_curve_2 c, Vertex_handle v1, Vertex_handle v2)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after a new edge e has been created. The halfedge that is sent to this function is always directed from v1 to v2
    virtual void after_create_edge( Halfedge_handle e)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued just before a vertex v is modified to be associated with the point p
    virtual void before_modify_vertex(Vertex_handle v, Point_2 p)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after an existing vertex v has been modified.
    virtual void after_modify_vertex(Vertex_handle v)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued just before an edge e is modified to be associated with the x-monotone curve c.
    virtual void before_modify_edge(Halfedge_handle e, X_monotone_curve_2 c)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after an existing edge e has been modified.
    virtual void after_modify_edge(Halfedge_handle e)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued just before an edge e is split into two edges that should be associated with the x-monotone curves c1 and c2. The vertex v corresponds to the split point, and will be used to separate the two resulting edges.
    virtual void before_split_edge(Halfedge_handle e,
        Vertex_handle v,
        X_monotone_curve_2 c1,
        X_monotone_curve_2 c2)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after an existing edge has been split into the two given edges e1 and e2.
    virtual void after_split_edge(Halfedge_handle e1, Halfedge_handle e2)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }

    /// Issued immediately after the existing face f has been split, such that a portion of it now forms a new face new_f. The flag is_hole designates whether new_f forms a hole inside f.
    virtual void after_split_face(Face_handle  f,
        Face_handle  new_f,
        bool /* is_hole */)
    {
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
        QLabellingLogWidget::instance()->logTrace( "Face color --> " + f->color().name() );
        QLabellingLogWidget::instance()->logTrace( "Face label --> " + f->label() );
        new_f->set_color(f->color());
        new_f->set_label(f->label());
    }

    /// Issued just before a face f is split into two, as a result of the insertion of the edge e into the arrangement.
    virtual void before_split_face(Face_handle f, Halfedge_handle e)
    {
        // TODO
        QLabellingLogWidget::instance()->logDebug( QString(__FUNCTION__) );
    }
};

#endif // __ARRANGEMENT_OBSERVER_H__