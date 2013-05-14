#ifndef QZOOMABLEGRAPHICSVIEW_HPP
#define QZOOMABLEGRAPHICSVIEW_HPP

class QWheelEvent;
class QMouseEvent;

#include <QGraphicsView>

class QZoomableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QZoomableGraphicsView() : QGraphicsView() {}

public slots:
    void zoomIn()  { scaleView(qreal(1.2));     }
    void zoomOut() { scaleView(1 / qreal(1.2)); }

protected:
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    
};

#endif // QZOOMABLEGRAPHICSVIEW_HPP
