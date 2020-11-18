//
// Created by rahul.kaplesh on 11/21/2019.
//

#ifndef POLYGONDIVIDE_BD_TYPEDEF_H
#define POLYGONDIVIDE_BD_TYPEDEF_H

#pragma once
#pragma once
#define CGAL_INTERSECTION_VERSION 1
#include <CGAL/intersections.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Vector_2.h>
#include <list>
#include <CGAL/Object.h>
#include <iostream>
#include <gmp.h>
#include <CGAL/draw_polygon_2.h>
#include <CGAL/convexity_check_2.h>
typedef CGAL::Cartesian<double>                                K;
typedef CGAL::Point_2<K>                                       Point_2;
typedef K::Line_2                                              Line_2;
typedef CGAL::Polygon_2<K>                                     Polygon_2;
typedef CGAL::Vector_2<K>                                      Vector_2;
typedef K::Segment_2                                           Segment_2;
typedef Polygon_2::Vertex_const_iterator                       VertexIterator;
typedef K::Intersect_2                                         Intersect_2;
typedef Polygon_2::Edge_const_iterator                         EdgeIterator;

#endif //POLYGONDIVIDE_BD_TYPEDEF_H
