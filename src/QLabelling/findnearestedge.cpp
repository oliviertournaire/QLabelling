#include "findnearestedge.h"



findnearestedge::findnearestedge(){

}

Pol_arr::Halfedge_const_handle findnearestedge::getnearestedge(QGraphicsSceneMouseEvent* event,Pol_arr* arr,QGraphicsScene* _scene)
{


    // find the nearest curve to the cursor to be the new highlighted curve
    Point_2 p = _converter( event->scenePos( ) );
    Find_nearest_edge< Pol_arr> findNearestEdge( arr );
    findNearestEdge.setScene( _scene );
    Pol_arr::Halfedge_const_handle nearestEdge = findNearestEdge( p );
    return nearestEdge;
}
