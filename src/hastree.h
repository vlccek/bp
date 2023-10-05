//
// Created by jvlk on 27.9.23.
//

#ifndef BP_HASTREE_H
#define BP_HASTREE_H

#include <vector>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end
#include <coroutine>
#include <queue>
#include <iostream>
#include <format>
#include <array>
#include "point.h"
#include "voro++.hh"

class Box;

class OctreeLeaf {
    Point p = Point(0, 0, 0);
    bool valid_point = false;
public:
    const Point &getPoint() const;

    bool isPointValid();

    OctreeLeaf(Point &p);

    OctreeLeaf();

    std::array<OctreeLeaf *, 8> childs;

    OctreeLeaf(Point point);
};

class HashOctree {
    voro::container con;
    OctreeLeaf *root = new OctreeLeaf();
    const int mmax;
public:
    /**
     * Constructor for class Hash occtre
     *
     * Construction progress:
     *  1) Compute voronoi cells
     *  2) Create Octree - on top of the voronoi cells
     *      a) take subspace and find how many voronoi cells are intersecting with this subspace
     *        b) if number of voronoi cells is bigger then `mmax` split this space by 8
     *      c) Save this subspace to the tree. If the b) is true save only the subspace, otherwise save the subspace with point
     *  ....
     * @param p vec of point to be procesed
     * @param min start of the subspace
     * @param max end of the subspace
     * @param mmax number that is use in building tree
     */
    HashOctree(std::vector<Point> &p, const Point &min, const Point &max, const int mmax = 5);

    inline void split_and_enqueue(const Point &min, const Point &max, std::queue<Box> &q);
    int count_cells_in_box(voro::c_loop_subset &cls, int c_voronoi, OctreeLeaf *tmp_parent,
               const Box &procesed_box) const;
};


class Box {

    Box *parent;

public:
    int level_order; // level counter
    Point min;
    Point max;

    Box(Point &min, Point &max, Box *parent, int id);

    Box(double xa, double ya, double za, double xb, double yb, double zb, int id, Box *parent = nullptr);
};

std::vector<Box> splitboxby8(const Point &minP, const Point &maxP);

#endif //BP_HASTREE_H
