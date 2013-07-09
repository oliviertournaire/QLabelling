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

#ifndef __ARRANGEMENT_TYPES_H__
#define __ARRANGEMENT_TYPES_H__

#include <QMetaType>
#include <QColor>

#include <CGAL/Object.h>
#include <CGAL/basic.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_simple_point_location.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Bbox_2.h>

#include <CGAL/Polygon_2.h>

// Coordinate related typedef - using inexact number type
typedef float                                           Coord_type;
typedef CGAL::Cartesian<Coord_type>                     Coord_kernel;

// polygon is usefull for filling faces
typedef CGAL::Polygon_2<Coord_kernel> My_polygon;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
struct Kernel : public CGAL::Epeck {};

class Face_with_color
{
    QColor    m_color;
    bool      m_visited;
    QString   m_label; // Le label associé à la face

public:
    Face_with_color() : m_color(), m_label("Undefined"), m_visited(false) { }

    // Getting the color for this face
    QColor color() const { return m_color; }
    // Getting the label for this face
    QString label() const { return m_label; }
    // Setting the color for this face
    void set_color(const QColor& c) { m_color = c; }
    // Setting the label for this face
    void set_label(const QString& l) { m_label = l; }
    bool visited() const{ return m_visited; }
    void set_visited(bool b) { m_visited = b; }
};

std::ostream& operator<<(std::ostream &o, const QColor &c);
std::istream& operator>>(std::istream &i, QColor &c);
std::ostream& operator<<(std::ostream &o, const Face_with_color &f);
std::istream& operator>>(std::istream &i, Face_with_color &f);

// Segments
typedef CGAL::Arr_segment_traits_2<Kernel>                          Seg_traits;

// Polyline
typedef CGAL::Arr_polyline_traits_2<Seg_traits>                     Pol_traits;
typedef Pol_traits::Curve_2                                         Arr_pol_2;
typedef Pol_traits::X_monotone_curve_2                              Arr_xpol_2;
typedef Pol_traits::Point_2                                         Arr_pol_point_2;
typedef CGAL::Arr_face_extended_dcel<Pol_traits, Face_with_color>   Pol_dcel;
typedef CGAL::Arrangement_with_history_2<Pol_traits, Pol_dcel>      Pol_arr;
typedef Pol_arr::Halfedge_handle                                    Pol_halfedge_handle;
typedef Pol_arr::Face_handle                                        Pol_face_handle;
typedef Pol_arr::Ccb_halfedge_circulator                            Pol_ccb_halfedge_circulator;
typedef Pol_arr::Hole_iterator                                      Pol_holes_iterator;
typedef Pol_arr::Halfedge                                           Pol_halfedge;
typedef Pol_arr::Face_iterator                                      Pol_face_iterator;
typedef std::list<Arr_pol_2*>                                       Arr_pol_list;
typedef Arr_pol_list::const_iterator                                Arr_pol_const_iter;
typedef Arr_pol_list::iterator                                      Arr_pol_iter;
// point location: just use (for simplification) a simple point location policy
typedef CGAL::Arr_simple_point_location<Pol_arr>                    Pol_simple_point_location;

Q_DECLARE_METATYPE( CGAL::Object );

#endif // __ARRANGEMENT_TYPES_H__
