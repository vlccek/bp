//
// Created by jvlk on 18.10.23.
//

#ifndef BP_OCTRERNODEBUILDER_H
#define BP_OCTRERNODEBUILDER_H


#include <vector>
#include <array>
#include <format>
#include <iostream>
#include "voro++.hh"

#include "point.h"
#include "OctrerNodeBuilder.h"
#include "box.h"
#include "polyhedron.h"
#include "gjk.h"

/**
 * Class for building temporary tree for computing tree layout
 */
class OctrerNodeBuilder {

public:
    explicit OctrerNodeBuilder(int level, int *maxLevel);

    Box border = Box({0, 0, 0}, {0, 0, 0});

    int childCount = 0;
    int level;
    int *maxLevel;


    std::vector<Polyhedron *> voronoiCells;
    std::array<OctrerNodeBuilder *, 8> childs = {0};

    inline void setBox(Point &min, Point &max) {
        border.min = min;
        border.max = max;
    }

    void buildTree();

    void alocateIfNeccesary(int index, Box &b);

    void addVoroCell(Polyhedron *vc);

    void getLeafs(std::vector<OctrerNodeBuilder *> &leafs);

    ~OctrerNodeBuilder();

};


#endif //BP_OCTRERNODEBUILDER_H
