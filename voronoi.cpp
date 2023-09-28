//
// Created by jvlk on 27.9.23.
//

#include "voronoi.h"
#include "point.h"
#include <queue>

class VoronoiCells{
    Point p;
    std::vector<Point> peakPoints;
};


/**
 * eventtype
 */
enum etype{
    p = 0, // point
    c , // circle
    i, // intersection
};


struct Event{

};


std::vector<VoronoiCells> *VoronoiDiagram(int x, int y, std::vector<Point> *p) {
    while (p->empty()){

    }
}