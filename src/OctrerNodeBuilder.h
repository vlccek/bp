//
// Created by jvlk on 18.10.23.
//

#ifndef BP_OCTRERNODEBUILDER_H
#define BP_OCTRERNODEBUILDER_H


#include <vector>
#include <set>
#include <array>
#include <format>
#include <iostream>
#include <fstream>
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
    explicit OctrerNodeBuilder(int level, int *maxLevel, voro::container_3d *con);

    Box border = Box({0, 0, 0}, {0, 0, 0});

    int childCount = 0;
    int level;
    int *maxLevel;


    std::vector<Polyhedron *> voronoiCells;
    std::vector<Polyhedron *> *allVoronoiCells;
    voro::container_3d *con;
    std::array<OctrerNodeBuilder *, 8> childs = {0};

    inline void setBox(PointDouble &min, PointDouble &max) {
        border.min = min;
        border.max = max;
    }

    void buildTree();

    void alocateIfNeccesary(int index, Box &b);


    bool intersect(Box &b, Polyhedron &vc);

    inline void addVoroCell(Polyhedron *vc) {
        voronoiCells.push_back(vc);
    }

    void addAllVoroCell(std::vector<Polyhedron *> *vcVector);

    void getLeafs(std::vector<OctrerNodeBuilder *> &leafs);

    void printVoronoiCells(std::ofstream &file);

    ~OctrerNodeBuilder();

    void getAllNodes(std::set<OctrerNodeBuilder *> &allNodes);
};


#endif //BP_OCTRERNODEBUILDER_H
