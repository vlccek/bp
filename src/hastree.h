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
#include <chrono>
#include <unordered_map>

#include "point.h"
#include "voro++.hh"
#include "OctrerNodeBuilder.h"
#include "polyhedron.h"
#include "gjk.h"

template<>
struct std::hash<std::pair<int, std::tuple<int, int, int>>> {
    std::size_t operator()(const std::pair<int, std::tuple<int, int, int>> &k) const {
        using std::size_t;
        using std::hash;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return (
                (hash<int>()(k.first)
                 ^ (hash<int>()(std::get<0>(k.second) << 1) >> 1)
                 ^ (hash<int>()(std::get<1>(k.second) << 1) >> 1)
                 ^ (hash<int>()(std::get<2>(k.second) << 1) >> 1)
                ));
    }
};


class HashOctree {
    OctrerNodeBuilder *root = new OctrerNodeBuilder(0, &maxLevel);
    std::vector<Polyhedron> voronoiCells;
    int maxLevel = 0;
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

    ~HashOctree() {
        delete root;
    }

    inline void split_and_enqueue(const Point &min, const Point &max, std::queue<Box> &q);

    bool isPointInBox(std::vector<double> &v, Box &b);

    bool isCellInBox(voro::voronoicell &vc, Box &box);

    void initTree();

    void buildTree();

    void buildHashTable();

    void printHashTable();


    inline int findBellogingInterval(double p, int boxCount) {
        double edgeLen = (max.x - min.x) / boxCount;
        return (int) (edgeLen / boxCount);
    }

    inline const OctrerNodeBuilder *getRoot() {
        return root;
    }

    std::tuple<int, int, int> findBellogingIntervals(Point &p, int boxCount) const;


    /**
     * Find the closes point to the given point
     * Steps:
     *  1) find id of voxel in witch is point P
     *  1a)
     *  2) TODO
     * @param p point
     * @return the closes point
     */
    Point nn(Point &p);

    static Point findClosesPointInNode(Point &p, const OctrerNodeBuilder *node);

private:
    std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctrerNodeBuilder *> hashTable{
    };
    int pointCount = 0;

    static void printNodePoints(OctrerNodeBuilder *value);
};


#endif //BP_HASTREE_H
