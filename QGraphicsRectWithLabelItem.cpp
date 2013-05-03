#include "QGraphicsRectWithLabelItem.hpp"

#include <QPainter>

void QGraphicsRectWithLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(_pen);
    painter->setBrush(_brush);
    painter->drawRect(rect().toRect());
}
