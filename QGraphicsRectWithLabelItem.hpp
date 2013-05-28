#ifndef QGRAPHICSRECTWITHLABELITEM_HPP
#define QGRAPHICSRECTWITHLABELITEM_HPP

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class QGraphicsRectWithLabelItem : public QGraphicsRectItem
{
public:
    //! Just define a new Type to be used by qgraphicsitem_cast
    enum { Type = UserType + 1 };
    //! Return the object type to be used by qgraphicsitem_cast
    virtual int type() const { return Type; }

    //! Constructor: init all to default
    QGraphicsRectWithLabelItem():
        _label("UNKNOW"),
        _pen( QPen() ),
        _brush( QBrush() ),
        QGraphicsRectItem() {}

    //! Write access to the label of the item
    inline void setLabel(const QString& label) { _label = label; }
    //! Read only access to the label of the item
    inline const QString& label() const { return _label; }

    //! Set the pen with which the rectangle item is drawn
    inline void setPen(const QPen& pen) { _pen= pen; }
    //! Get the pen with which the rectangle item is drawn
    inline const QPen& pen() const { return _pen; }

    //! Set the brush with which the rectangle item is drawn
    inline void setBrush(const QBrush& brush) { _brush = brush; }
    //! Get the brush with which the rectangle item is drawn
    inline const QBrush& brush() const { return _brush; }

    //! Redefines (but just calls the parent class method) the method called when looking for intersecting objects
    virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const
    {
        return QGraphicsRectItem::collidesWithItem(other, mode);
    }

    //! Redefines the paint method to take into account pen and brush properties
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //! The label of te item
    QString _label;
    //! The pen to draw the item
    QPen _pen;
    //! The brush to draw the item
    QBrush _brush;
};

#endif // QGRAPHICSRECTWITHLABELITEM_HPP
