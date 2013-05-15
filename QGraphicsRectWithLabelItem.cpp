#include "QGraphicsRectWithLabelItem.hpp"

#include <QPainter>

void QGraphicsRectWithLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(_pen);
    painter->setBrush(_brush);
    QRect rectToDraw = rect().toRect();
    rectToDraw.setRight( rectToDraw.right() + 1 );
    rectToDraw.setBottom( rectToDraw.bottom() + 1 );
    painter->drawRect(rectToDraw);
}
