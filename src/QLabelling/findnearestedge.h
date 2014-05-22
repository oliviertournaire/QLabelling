#ifndef FINDNEARESTEDGE_H
#define FINDNEARESTEDGE_H
#include "Utils.h"
#include "utilities/Singleton.hpp"
typedef CGAL::Arr_segment_traits_2<Kernel>                          Seg_traits;

// Polyline
typedef CGAL::Arr_polyline_traits_2<Seg_traits>                     Pol_traits;
typedef Pol_traits::Curve_2                                         Arr_pol_2;
typedef Pol_traits::X_monotone_curve_2                              Arr_xpol_2;
typedef Pol_traits::Point_2                                         Arr_pol_point_2;
typedef CGAL::Arr_face_extended_dcel<Pol_traits, Face_with_color>   Pol_dcel;
typedef CGAL::Arrangement_with_history_2<Pol_traits, Pol_dcel>      Pol_arr;
typedef CGAL::Arr_polyline_traits_2< Seg_traits > Traits;
typedef typename Traits::Curve_2 Curve_2;
typedef typename Seg_traits::Kernel Kernel;
typedef typename Kernel::Point_2 Point_2;
typedef typename Kernel::Segment_2 Segment_2;
class findnearestedge : public Singleton<findnearestedge>
{
public:
   findnearestedge();
   Pol_arr::Halfedge_const_handle getnearestedge(QGraphicsSceneMouseEvent* event, Pol_arr *arr, QGraphicsScene* _scene);
private:
   CGAL::Qt::Converter< Kernel > _converter;
};

#endif // FINDNEARESTEDGE_H
