/**
* @file polyhedron.h
* @brief Polyhedron class
* @author Jakub Vlk
 */

#include <vector>
#include <format>
#include <string>

#include "point.h"
#include "voro++.hh"
#include "box.h"

#ifndef BP_VORONOI_H
#define BP_VORONOI_H

class Polyhedron {
public:
    Point p;
    std::vector<Point> vertexPoints; // list of all vertex
    BoudingBox boudingBox;
    // voro::voronoicell_neighbor_3d cell;
    std::vector<int> neighbors;// id of neigbors voronoi cells

    Polyhedron() : p(0,0,0){
    };

    Polyhedron(voro::voronoicell_3d &vc, Point &p);

    Polyhedron(std::vector<Point> &vertext, Point &centerPoint, voro::voronoicell_3d &cell): Polyhedron(vertext,centerPoint){};
    Polyhedron(std::vector<Point> &vertext, Point &centerPoint);


    void setNeigbors(std::vector<int> &n) {
        neighbors = n;
    }

    explicit operator std::string const() {
        std::string s = "\n";
        for (auto &i: vertexPoints) {
            s += std::format("\t({:.2f},{:.2f},{:.2f})\n", i.x, i.y, i.z);
        }
        return std::format("Vertex: {}", s);
    }

};


#endif //BP_VORONOI_H
