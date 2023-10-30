//
// Created by jvlk on 27.9.23.
//
#include <vector>
#include <format>
#include <string>

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

    Polyhedron(std::vector<Point> &vertext, Point &centerPoint);


    operator std::string const() {
        std::string s = "\n";
        for (auto i: vertexPoints) {
            s += std::format("\t({:.2f},{:.2f},{:.2f})\n", i.x, i.y, i.z);
        }
        return std::format("Vertex: {}", s);
    }

    Point &futherPoint(Point &d);
};


#endif //BP_VORONOI_H
