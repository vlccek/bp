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
    // voro::voronoicell_neighbor_3d cell;
    std::vector<int> neighbors;// id of neigbors voronoi cells
    int id;


    Polyhedron(voro::voronoicell_neighbor_3d &vc, Point &p, int id);

    Polyhedron(std::vector<Point> &vertext, Point &centerPoint, voro::voronoicell_neighbor_3d &cell);


    void setNeigbors(std::vector<int> &n) {
        neighbors = n;
    }

    operator std::string const() {
        std::string s = "\n";
        for (auto &i: vertexPoints) {
            s += std::format("\t({:.2f},{:.2f},{:.2f})\n", i.x, i.y, i.z);
        }
        return std::format("Vertex: {}", s);
    }

    Point &futherPoint(PointDouble &d);
};


#endif //BP_VORONOI_H
