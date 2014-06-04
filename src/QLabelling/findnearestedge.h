#ifndef FINDNEARESTEDGE_H
#define FINDNEARESTEDGE_H

#include <CGAL/Qt/Converter.h>

class QGraphicsSceneMouseEvent;
class QGraphicsScene;

#include "ArrangementTypes.h"

class findnearestedge
{
public:
   findnearestedge() {}
   Pol_arr::Halfedge_const_handle getnearestedge(QGraphicsSceneMouseEvent* event, Pol_arr *arr, QGraphicsScene* _scene);

private:
   CGAL::Qt::Converter< Kernel > _converter;
};
#endif // FINDNEARESTEDGE_H
