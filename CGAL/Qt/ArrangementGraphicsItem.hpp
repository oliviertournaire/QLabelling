#ifndef ARRANGEMENTGRAPHICSITEM_HPP
#define ARRANGEMENTGRAPHICSITEM_HPP

#include <CGAL/Qt/GraphicsItem.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/Converter.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

namespace CGAL {
namespace Qt {

template <typename T>
class ArrangementGraphicsItem : public CGAL::Qt::GraphicsItem
{
    typedef typename T::Geometry_traits_2 Geometry_traits_2;
    typedef typename T::Point_2           Point_2;
public:
    ArrangementGraphicsItem(T* t_) {}

    void modelChanged() {}

public:

    QRectF boundingRect() const {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

    virtual void operator()(typename T::Face_handle fh) {}

    const QPen& verticesPen() const
    {
        return vertices_pen;
    }

    const QPen& edgesPen() const
    {
        return edges_pen;
    }

    void setVerticesPen(const QPen& pen)
    {
        vertices_pen = pen;
    }

    void setEdgesPen(const QPen& pen)
    {
        edges_pen = pen;
    }

    bool visibleVertices() const
    {
        return visible_vertices;
    }

    void setVisibleVertices(const bool b)
    {
        visible_vertices = b;
        update();
    }

    bool visibleEdges() const
    {
        return visible_edges;
    }

    void setVisibleEdges(const bool b)
    {
        visible_edges = b;
        update();
    }

protected:
    virtual void drawAll(QPainter *painter) {}
    void paintVertices(QPainter *painter) {}
    void paintOneVertex(const Point_2& point) {}
    virtual void paintVertex(typename T::Vertex_handle vh) {}
    void updateBoundingBox() {}

    T *t;
    //PainterOstream<T> _painterOstream;
    QPainter* m_painter;

    typename T::Vertex_handle vh;
    CGAL::Bbox_2 bb;
    bool bb_initialized;
    QRectF bounding_rect;

    QPen vertices_pen;
    QPen edges_pen;
    bool visible_edges;
    bool visible_vertices;
};

} // namespace Qt
} // namespace CGAL

#endif // ARRANGEMENTGRAPHICSITEM_HPP
