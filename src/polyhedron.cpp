/**
* @file polyhedron.cpp
* @brief Polyhedron class
* @author Jakub Vlk
 */


#include "polyhedron.h"
#include "point.h"
#include <queue>
#include "voro++.hh"

Polyhedron::Polyhedron(voro::voronoicell_3d
                       &vc,
                       Point &p
) :
        p(p) {
    std::vector<double> vertex;
    vc.vertices(p.x, p.y, p.z, vertex);
    for (int i = 0; i < vertex.size(); i += 3) {
        Point p(vertex[i], vertex[i + 1], vertex[i + 2]);
        vertexPoints.push_back(p);
    }
    boudingBox = BoudingBox(vertexPoints);
}


Polyhedron::Polyhedron(std::vector<Point> &vertext, Point &centerPoint) : p(
        centerPoint) {

    for (auto &i: vertext) {
        vertexPoints.push_back(i);
    }
    boudingBox = BoudingBox(vertexPoints);

}
