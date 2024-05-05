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
#include <immintrin.h>
#include <omp.h>
// #include "gjk.h"

inline float dotNorm(Point &a, Point &b) {
  return a.nor_x * b.nor_x + a.nor_y * b.nor_y + a.nor_z * b.nor_z;
}

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

class nnNormalIterator {
  std::vector<Point *> points;
  int counter = 0;
  Point p;

public:
  nnNormalIterator(std::vector<Point *> points, Point &p)
      : points(points), p(p){
                            // empty
                        };
  Point *next() {

    auto normalCmp = [this](Point *a, Point *b) {
      return dotNorm(*a, p) < dotNorm(*b, p);
    };
    auto resultIterator =
        std::max_element(points.begin(), points.end(), normalCmp);

    Point *res = *resultIterator;
    if (!isEmpty())
      this->points.erase(resultIterator);

    return *resultIterator;
  }

  bool isEmpty() { return points.size() == 1; };
};

inline __m256 normalizePoint(Point p, Point min, Point max) {
  __m256 pVec = _mm256_set_ps(0, p.z, p.y, p.x, 0, p.z, p.y, p.x);
  __m256 minVec = _mm256_set_ps(0, min.z, min.y, min.x, 0, min.z, min.y, min.x);
  __m256 maxVec = _mm256_set_ps(0, max.z, max.y, max.x, 0, max.z, max.y, max.x);

  // Subtract min from p and max
  __m256 pMinusMin = _mm256_sub_ps(pVec, minVec);
  __m256 maxMinusMin = _mm256_sub_ps(maxVec, minVec);

  // Divide (p - min) / (max - min)
  __m256 result = _mm256_div_ps(pMinusMin, maxMinusMin);

  return result;
}

class HashOctree {
  OctreeNode *root;
  voro::container_3d con;
  std::vector<Polyhedron> voronoiCells;

  int minLeafLevel = 0;

public:
  int maxLevel = 0;
  chrono::time_point<chrono::high_resolution_clock> start;
  chrono::time_point<chrono::high_resolution_clock> voroBuild;
  chrono::time_point<chrono::high_resolution_clock> treeBuild;
  chrono::time_point<chrono::high_resolution_clock> htBuild;

  Point min, max;

  const int maxPointsInNode = 5;
  /**
   * Constructor for class Hash occtre
   *
   * Construction progress:
   *  1) Compute voronoi cells
   *  2) Create Octree - on top of the voronoi cells
   *      a) take subspace and find how many voronoi cells are intersecting
   * with this subspace boudingBox) if number of voronoi cells is bigger then
   * `mmax` split this space by 8 c) Save this subspace to the tree. If the
   * boudingBox) is true save only the subspace, otherwise save the subspace
   * with point
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

  void createArrayofPoints();

  void printHashTable();

  std::vector<OctreeNode *> getLeafs();

  std::set<OctreeNode *> getAllNodes();

  inline const OctreeNode *getRoot() { return root; }

  /**
   * Find the closes point to the given point
   * Steps:
   *  1) find id of voxel in which is point P
   *  1a)
   *  2) TODO
   * @param p point
   * @return the closes point
   */
  Point &nn(Point &p);

  Point &nnFirstNormal(Point &p, int effort = 0, float tolerance = 0.1);

  Point &nnBestNormalLeaf(Point &p, int effort = 0);
  nnNormalIterator nnNormalSearch(Point &p, float distance);

  std::vector<Point *> knn(Point &p, int k);

  OctreeNode *findClosesNode(Point &p);

  Point &findClosesPointInNode(Point &p, const OctreeNode *node);
  Point &findClosesPointInNodeLeaf(Point &p, const OctreeNode *node);

  inline std::tuple<int, int, int>
  findBellogingIntervalsByLevel(Point p, int level) const {

    if (level == 0)
      return std::make_tuple(0, 0, 0);

    __m256 normalizedPoint = normalizePoint(p, min, max);

    // if the point is on the edge of the box, then it is in the last box
    // so we need to subtract a small number from the point

    int boxCount = 1 << (level);

    __m256 boxCountVec = _mm256_set1_ps(static_cast<float>(boxCount));

    __m256 boxf = normalizedPoint * boxCountVec;
    __m256i box = _mm256_cvttps_epi32(boxf);

    // correction

    __m256i correction = _mm256_cmpeq_epi32(box, _mm256_set1_epi32(boxCount));

    box = _mm256_add_epi32(box, correction);
    int *box_array = (int *)&box;

    return {box_array[0], box_array[1], box_array[2]};
  }

  inline void printBuildTimes() const {
    auto duration = duration_cast<chrono::nanoseconds>(voroBuild - start);
    cout << "Voronoi finish: " << duration.count() << "ms" << endl;

    duration = duration_cast<chrono::nanoseconds>(treeBuild - voroBuild);
    cout << "tree finish: " << duration.count() << "ms" << endl;

    duration = duration_cast<chrono::nanoseconds>(htBuild - treeBuild);
    cout << "HT finish: " << duration.count() << "ms" << endl;
  }

public:
  std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctreeNode *>
      hashTable{};
  size_t pointCount = 0;

  static void printNodePoints(OctreeNode *value);
};

inline int roundUp8(int x) { return ((x + 7) & (-8)); }

#endif // BP_HASTREE_H
