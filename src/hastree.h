//
// Created by jvlk on 27.9.23.
//

#ifndef BP_HASTREE_H
#define BP_HASTREE_H

#include <algorithm> // std::min_element
#include <array>
#include <chrono>
#include <coroutine>
#include <format>
#include <iostream>
#include <iterator> // std::begin, std::end
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "OctreeNode.h"
#include "point.h"
#include "polyhedron.h"
#include "voro++.hh"
#include <chrono>
#include <omp.h>
// #include "gjk.h"

template <> struct std::hash<std::pair<int, std::tuple<int, int, int>>> {
  std::size_t
  operator()(const std::pair<int, std::tuple<int, int, int>> &k) const {
    using std::hash;
    using std::size_t;
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

inline Point normalizePoint(Point p, Point min, Point max) {
  return (p - min) / (max - min);
}

class HashOctree {
  OctreeNode *root;
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

  const int maxPointsInNode = 5;
  /**
   * Constructor for class Hash occtre
   *
   * Construction progress:
   *  1) Compute voronoi cells
   *  2) Create Octree - on top of the voronoi cells
   *      a) take subspace and find how many voronoi cells are intersecting with
   * this subspace boudingBox) if number of voronoi cells is bigger then `mmax`
   * split this space by 8 c) Save this subspace to the tree. If the boudingBox)
   * is true save only the subspace, otherwise save the subspace with point
   *  ....
   * @param p vec of point to be procesed
   * @param min start of the subspace
   * @param max end of the subspace
   * @param mmax number that is use in building tree
   */
  HashOctree(std::vector<Point> &p, const Point &min, const Point &max,
             int threads = omp_get_max_threads(), int maxPointsInNode = 5);

  HashOctree(std::vector<Point> &p, const float min, const float max,
             int threads = omp_get_max_threads(), int maxCellInNode = 5)
      : HashOctree(p, Point(min, min, min), Point(max, max, max), threads,
                   maxCellInNode){
            // empty
        };

  ~HashOctree() { delete root; }

  void saveVoroCellToFile(voro::container_3d &con);

  bool isPointInBox(std::vector<float> &v, Box &b);

  bool isCellInBox(voro::voronoicell_3d &vc, Box &box);

  void initTree();

  void buildTree();

  void buildHashTable();

  void printHashTable();

  std::vector<OctreeNode *> getLeafs();

  std::set<OctreeNode *> getAllNodes();

  inline const OctreeNode *getRoot() { return root; }

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

  std::vector<Point *> knn(Point &p, int k);


  OctreeNode * findClosesNode(Point &p);

  Point findClosesPointInNode(Point &p, const OctreeNode *node);

  inline std::tuple<int, int, int>
  findBellogingIntervalsByLevel(Point p, int level) const {
    if (level == 0) {
      return std::make_tuple(0, 0, 0);
    }
    auto normalizedPoint = normalizePoint(p, min, max);

    int boxCount = 1 << (level);
    return std::make_tuple(
        // if the point is on the edge of the box, then it is in the last box
        static_cast<int>(((normalizedPoint.x) * boxCount == boxCount)
                             ? boxCount - 1
                             : (normalizedPoint.x) * boxCount),
        static_cast<int>(((normalizedPoint.y) * boxCount == boxCount)
                             ? boxCount - 1
                             : (normalizedPoint.y) * boxCount),
        static_cast<int>(((normalizedPoint.z) * boxCount == boxCount)
                             ? boxCount - 1
                             : (normalizedPoint.z) * boxCount));
  }

  inline void printBuildTimes(){
    auto duration = duration_cast<chrono::milliseconds>(voroBuild - start);
    cout << "Voronoi finish: " << duration.count() << "ms" << endl;

    duration = duration_cast<chrono::milliseconds>(treeBuild - voroBuild);
    cout << "tree finish: " << duration.count() << "ms" << endl;

    duration = duration_cast<chrono::milliseconds>(htBuild - treeBuild);
    cout << "HT finish: " << duration.count() << "ms" << endl;
  }

      private:
  std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctreeNode *>
      hashTable{};
  size_t pointCount = 0;

  static void printNodePoints(OctreeNode *value);
};

#endif // BP_HASTREE_H
