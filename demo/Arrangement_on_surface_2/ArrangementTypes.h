// Copyright (c) 2005,2012 Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/features/gsoc2012-Arrangement_on_surface_2-demo-atsui/Arrangement_on_surface_2/demo/Arrangement_on_surface_2/cgal_types.h $
// $Id: cgal_types.h 67117 2012-01-13 18:14:48Z lrineau $
//
//
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>
//                 Alex Tsui <alextsui05@gmail.com>

#ifndef ARRANGEMENT_DEMO_TYPES_H
#define ARRANGEMENT_DEMO_TYPES_H

#include <CGAL/basic.h>

/*
#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_Polygon_2.h>
#include <CGAL/IO/Qt_help_window.h>
*/

#include <CGAL/Cartesian.h>
#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arr_consolidated_curve_data_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_simple_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Bbox_2.h>

#include <CGAL/Arr_observer.h>
#include <CGAL/Polygon_2.h>

// Coordinate related typedef - using inexact number type
typedef float                                           Coord_type;
typedef CGAL::Cartesian<Coord_type>                     Coord_kernel;
typedef Coord_kernel::Point_2                           Coord_point;
typedef Coord_kernel::Segment_2                         Coord_segment;
typedef Coord_kernel::Circle_2                          Coord_circle;


typedef CGAL::Polygon_2<Coord_kernel> My_polygon;
                                      // polygon is usefull for filling faces

#ifdef CGAL_USE_GMP
  #include <CGAL/Gmpq.h>
  typedef CGAL::Gmpq                                    NT;
#else
  #include <CGAL/MP_Float.h>
  #include <CGAL/Quotient.h>
  typedef CGAL::Quotient<CGAL::MP_Float>                NT;
#endif
// instead of
//typedef CGAL::Cartesian<NT>                           Kernel;
// workaround for VC++
struct Kernel : public CGAL::Cartesian<NT> {};

class Face_with_color : public CGAL::Arr_face_base
{
  QColor    m_color;
  bool      m_visited;

public:
  Face_with_color() : CGAL::Arr_face_base(), m_color(), m_visited(false) { }

  QColor color() const { return m_color; }
  void set_color(const QColor& c) { m_color = c; }
  bool visited() const{ return m_visited; }
  void set_visited(bool b) { m_visited = b; }
};

template <class Traits>
class Dcel :
  public CGAL::Arr_dcel_base<CGAL::Arr_vertex_base<typename Traits::Point_2>,
                             CGAL::Arr_halfedge_base<
                               typename Traits::X_monotone_curve_2>,
                             Face_with_color>
{
public:
   /*! \struct
   * An auxiliary structure for rebinding the DCEL with a new traits class.
   */
  template <typename T>
  struct rebind
  {
    typedef Dcel<T> other;
  };

  // CREATION
  Dcel() {}
};

// Segments:
typedef CGAL::Arr_segment_traits_2<Kernel>              Seg_traits;
typedef Seg_traits::Curve_2                             Arr_seg_2;
typedef Seg_traits::X_monotone_curve_2                  Arr_xseg_2;
typedef Seg_traits::Point_2                             Arr_seg_point_2;
typedef Dcel<Seg_traits>                                Seg_dcel;
typedef CGAL::Arrangement_with_history_2<Seg_traits, Seg_dcel>
                                                        Seg_arr;
typedef Seg_arr::Halfedge                               Seg_halfedge;
typedef Seg_arr::Halfedge_handle                        Seg_halfedge_handle;
typedef Seg_arr::Face_handle                            Seg_face_handle;
typedef Seg_arr::Ccb_halfedge_circulator
  Seg_ccb_halfedge_circulator;
typedef Seg_arr::Hole_iterator                          Seg_holes_iterator;
typedef Seg_arr::Face_iterator                          Seg_face_iterator;
typedef std::list<Arr_seg_2*>                           Arr_seg_list;
typedef Arr_seg_list::const_iterator                    Arr_seg_const_iter;
typedef Arr_seg_list::iterator                          Arr_seg_iter;


// point location
typedef CGAL::Arr_trapezoid_ric_point_location<Seg_arr>
  Seg_trap_point_location;
typedef CGAL::Arr_simple_point_location<Seg_arr>
  Seg_simple_point_location;
typedef CGAL::Arr_walk_along_line_point_location<Seg_arr>
  Seg_walk_point_location;
typedef CGAL::Arr_landmarks_point_location<Seg_arr>
  Seg_lanmarks_point_location;

// Polyline
typedef CGAL::Arr_polyline_traits_2<Seg_traits>         Pol_traits;

typedef Pol_traits::Curve_2                             Arr_pol_2;
typedef Pol_traits::X_monotone_curve_2                  Arr_xpol_2;

typedef Pol_traits::Point_2                             Arr_pol_point_2;
typedef Dcel<Pol_traits>                                Pol_dcel;
typedef CGAL::Arrangement_with_history_2<Pol_traits,
                                         Pol_dcel>      Pol_arr;
typedef Pol_arr::Halfedge_handle                        Pol_halfedge_handle;
typedef Pol_arr::Face_handle                            Pol_face_handle;
typedef Pol_arr::Ccb_halfedge_circulator
  Pol_ccb_halfedge_circulator;
typedef Pol_arr::Hole_iterator                          Pol_holes_iterator;
typedef Pol_arr::Halfedge                               Pol_halfedge;
typedef Pol_arr::Face_iterator                          Pol_face_iterator;

typedef std::list<Arr_pol_2*>                            Arr_pol_list;
typedef Arr_pol_list::const_iterator                     Arr_pol_const_iter;
typedef Arr_pol_list::iterator                           Arr_pol_iter;

// point location
typedef CGAL::Arr_trapezoid_ric_point_location<Pol_arr>
  Pol_trap_point_location;
typedef CGAL::Arr_simple_point_location<Pol_arr>
  Pol_simple_point_location;
typedef CGAL::Arr_walk_along_line_point_location<Pol_arr>
  Pol_walk_point_location;
typedef CGAL::Arr_landmarks_point_location<Pol_arr>
  Pol_lanmarks_point_location;


// Linear:
typedef CGAL::Arr_linear_traits_2<Kernel>               Lin_traits;
typedef Lin_traits::Curve_2                             Arr_lin_2;
typedef Lin_traits::X_monotone_curve_2                  Arr_xlin_2;
typedef Lin_traits::Point_2                             Arr_lin_point_2;
typedef Dcel<Lin_traits>                                Lin_dcel;
typedef CGAL::Arrangement_with_history_2<Lin_traits, Lin_dcel>
                                                        Lin_arr;
typedef Lin_arr::Halfedge                               Lin_halfedge;
typedef Lin_arr::Halfedge_handle                        Lin_halfedge_handle;
typedef Lin_arr::Face_handle                            Lin_face_handle;
typedef Lin_arr::Ccb_halfedge_circulator
  Lin_ccb_halfedge_circulator;
typedef Lin_arr::Hole_iterator                          Lin_holes_iterator;
typedef Lin_arr::Face_iterator                          Lin_face_iterator;
typedef std::list<Arr_lin_2*>                           Arr_lin_list;
typedef Arr_lin_list::const_iterator                    Arr_lin_const_iter;
typedef Arr_lin_list::iterator                          Arr_lin_iter;

//point location
typedef CGAL::Arr_trapezoid_ric_point_location<Lin_arr>
  Lin_trap_point_location;
typedef CGAL::Arr_simple_point_location<Lin_arr>
  Lin_simple_point_location;
typedef CGAL::Arr_walk_along_line_point_location<Lin_arr>
  Lin_walk_point_location;
typedef CGAL::Arr_landmarks_point_location<Lin_arr>
  Lin_landmarks_point_location;

template <class Arrangement_>
class My_observer : public CGAL::Arr_observer<Arrangement_>
{
public:

  typedef Arrangement_                                  Arrangement;
  typedef CGAL::Arr_observer<Arrangement>               Arr_observer;
  typedef typename Arrangement::Face_handle             Face_handle;

  My_observer (Arrangement& arr) : Arr_observer (arr) {}

   virtual void after_split_face (Face_handle  f ,
                                  Face_handle  new_f ,
                                  bool         /* is_hole */)
  {
    new_f ->set_color(f->color());
  }

};

//Q_DECLARE_METATYPE( Seg_arr )
//Q_DECLARE_METATYPE( Pol_arr )
//Q_DECLARE_METATYPE( Conic_arr )
Q_DECLARE_METATYPE( CGAL::Object )

#endif // ARRANGEMENT_DEMO_TYPES_H