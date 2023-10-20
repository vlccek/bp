//
// Created by jvlk on 19.10.23.
//

#ifndef BP_BKJ_H
#define BP_BKJ_H

#include "point.h"
#include "box.h"
#include "polyhedron.h"

Point support(Box &b, Polyhedron *p, Point d);

bool gjk(Box &b, Polyhedron *p);

bool nextSimplex(std::vector<Point> &simplex, Point &direction);

bool tetrahedron(std::vector<Point> &simplex, Point &direction);

bool triangle(std::vector<Point> &simplex, Point &direction);

bool line(std::vector<Point> &simplex, Point &direction);

bool sameDirection(const Point &direction, const Point &ao);

#endif //BP_BKJ_H
