//
// Created by jvlk on 27.9.23.
//

#include "polyhedron.h"
#include "point.h"
#include <queue>
#include "voro++.hh"

Polyhedron::Polyhedron(voro::voronoicell_3d
                       &vc,
                       Point  &p
) :
        p(p) {
    std::vector<double> vertex;
    vc.vertices(p.x,p.y,p.z,vertex);
    for (int i = 0; i < vertex.size(); i += 3) {
        Point p(vertex[i], vertex[i + 1], vertex[i + 2]);
        vertexPoints.push_back(p);
    }
    boudingBox = BoudingBox(vertexPoints);
}

Point  &Polyhedron::futherPoint(PointDouble  &d) {

    std::pair<Point *, double> max{&vertexPoints[0], 0}; // saving point and his destination
    for (int i = 0; i < vertexPoints.size(); i++) {
        auto *procesPoint = &vertexPoints[i];
        double distance = procesPoint->distance(d);
        if (distance > max.second) {
            max = {procesPoint, distance};
        }
    }
    return *max.first;
}

Polyhedron::Polyhedron(std::vector<Point > &vertext, Point  &centerPoint): p(centerPoint) {

    for (auto &i: vertext) {
        vertexPoints.push_back(i);
    }
    boudingBox = BoudingBox(vertexPoints);
}
