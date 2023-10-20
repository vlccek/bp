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
#include <tuple>
#include "point.h"
#include "voro++.hh"
#include "OctrerNodeBuilder.h"
#include "polyhedron.h"
#include "bkj.h"

class HashOctree {
    OctrerNodeBuilder *root = new OctrerNodeBuilder();
    std::vector<Polyhedron> voronoiCells;
public:

    Point min;
    Point max;

    /**
     * Constructor for class Hash occtre
     *
     * Construction progress:
     *  1) Compute voronoi cells
     *  2) Create Octree - on top of the voronoi cells
     *      a) take subspace and find how many voronoi cells are intersecting with this subspace
     *        boudingBox) if number of voronoi cells is bigger then `mmax` split this space by 8
     *      c) Save this subspace to the tree. If the boudingBox) is true save only the subspace, otherwise save the subspace with point
     *  ....
     * @param p vec of point to be procesed
     * @param min start of the subspace
     * @param max end of the subspace
     * @param mmax number that is use in building tree
     */
    HashOctree(std::vector<Point> &p, const Point &min, const Point &max);

    inline void split_and_enqueue(const Point &min, const Point &max, std::queue<Box> &q);

    bool isPointInBox(std::vector<double> &v, Box &b);

    bool isCellInBox(voro::voronoicell &vc, Box &box);

    void initTree();

    void buildTree();

};


#endif //BP_HASTREE_H
