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
#include <set>
#include <chrono>
#include <unordered_map>

#include "point.h"
#include "voro++.hh"
#include "OctrerNodeBuilder.h"
#include "polyhedron.h"
#include <omp.h>
#include <chrono>
// #include "gjk.h"

template<>
struct std::hash<std::pair<int, std::tuple<int, int, int>>> {
    std::size_t operator()(const std::pair<int, std::tuple<int, int, int>> &k) const {
        using std::size_t;
        using std::hash;
        using std::string;

        auto hash1 = std::hash<int>{}(k.first);
        auto hash2 = std::hash<int>{}(std::get<0>(k.second));
        auto hash3 = std::hash<int>{}(std::get<1>(k.second));
        auto hash4 = std::hash<int>{}(std::get<2>(k.second));

        // Combine the hashes
        std::size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash4 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};


template<typename U, typename V>
inline PointGeneric<U> normalizePoint(PointGeneric<U> p, PointGeneric<V> min, PointGeneric<V> max) {
    return (p - min) / (max - min);
}


class HashOctree {
    OctrerNodeBuilder *root;
    voro::container_3d con;
    std::vector<Polyhedron> voronoiCells;
    int maxLevel = 0;
    int minLeafLevel = 0;
    chrono::time_point<chrono::high_resolution_clock> start;
    chrono::time_point<chrono::high_resolution_clock> voroBuild;
    chrono::time_point<chrono::high_resolution_clock> treeBuild;
    chrono::time_point<chrono::high_resolution_clock> htBuild;

public:

    Point min, max;

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
    HashOctree(std::vector<Point> &p, const Point &min, const Point &max, int threads = omp_get_max_threads());

    HashOctree(std::vector<Point> &p, const float min, const float max, int threads = omp_get_max_threads())
            : HashOctree(p, Point(min, min, min),
                         Point(max, max, max), threads) {
        //empty
    };

    ~HashOctree() {
        delete root;
    }

    void saveVoroCellToFile(voro::container_3d &con);


    bool isPointInBox(std::vector<float> &v, Box &b);

    bool isCellInBox(voro::voronoicell_3d &vc, Box &box);

    void initTree();

    void buildTree();

    void buildHashTable();

    void printHashTable();

    std::vector<OctrerNodeBuilder *> getLeafs();

    std::set<OctrerNodeBuilder *> getAllNodes();


    inline const OctrerNodeBuilder *getRoot() {
        return root;
    }

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


    Point findClosesPointInNode(Point &p, const OctrerNodeBuilder *node);


    template<class T>
    inline std::tuple<int, int, int> findBellogingIntervalsByLevel(PointGeneric<T> p, int level) const {
        if (level == 0) {
            return std::make_tuple(0, 0, 0);
        }
        auto normalizedPoint = normalizePoint(p, min, max);

        int boxCount = 1 << (level);
        return std::make_tuple(
                // if the point is on the edge of the box, then it is in the last box
                static_cast<int> (((normalizedPoint.x) * boxCount == boxCount) ? boxCount - 1 : (normalizedPoint.x) *
                                                                                                boxCount),
                static_cast<int> (((normalizedPoint.y) * boxCount == boxCount) ? boxCount - 1 : (normalizedPoint.y) *
                                                                                                boxCount),
                static_cast<int> (((normalizedPoint.z) * boxCount == boxCount) ? boxCount - 1 : (normalizedPoint.z) *
                                                                                                boxCount)
        );
    }

private:
    std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctrerNodeBuilder *> hashTable{
    };
    size_t pointCount = 0;

    static void printNodePoints(OctrerNodeBuilder *value);


};


#endif //BP_HASTREE_H
