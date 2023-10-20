//
// Created by jvlk on 27.9.23.
//
#include <vector>

#include "point.h"
#include "voro++.hh"
#include "box.h"

#ifndef BP_VORONOI_H
#define BP_VORONOI_H

// todo rename this class/file to "mnogostěn"
class Polyhedron {
public:
    Point p;
    std::vector<Point> vertexPoints; // list of all vertex
    BoudingBox boudingBox;

    Polyhedron(voro::voronoicell &vc, Point &p);

    Point & futherPoint(Point &d);
};


#endif //BP_VORONOI_H
