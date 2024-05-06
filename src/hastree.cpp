/**
 * @file hastree.cpp
 * @brief Implementation of HashOctree class

 */


#include "hastree.h"

// #define TREE_PRINT_BUILDTIME

HashOctree::HashOctree(std::vector<Point> &p, const Point &min,
                       const Point &max, int threads, int maxPointsInNode)
    : min(min), max(max), con(min.x, max.x, min.y, max.y, min.z, max.z, 160,
                              160, 160, false, false, false, 8, threads),
      maxPointsInNode(maxPointsInNode) {
  root = new OctreeNode(0, &maxLevel, maxPointsInNode);

  pointCount = (p.size());

  int id = 0;
  for (auto &i : p) {
    con.put(id++, i.x, i.y, i.z);
  }
  voro::container_3d::iterator cli;

  start = chrono::high_resolution_clock::now();

  voronoiCells.resize(pointCount);
#pragma omp parallel
  {
    voro::voronoicell_3d c(con);
    double x, y, z;
#pragma omp for
    for (cli = con.begin(); cli < con.end(); cli++) {
      if (con.compute_cell(c, cli)) {
        con.pos(cli, x, y, z);

        int index = cli - con.begin();
        Point po(p[index]);
        // Point po(x, y, z, p[index].nor_x, p[index].nor_y, p[index].nor_z);
        auto poly = Polyhedron(c, po);

        voronoiCells[index] = poly;
      }
    }
  }

  voroBuild = chrono::high_resolution_clock::now();

  initTree();

  buildTree();

  treeBuild = chrono::high_resolution_clock::now();

  buildHashTable();

  htBuild = chrono::high_resolution_clock::now();

  createArrayofPoints();
#ifdef TREE_PRINT_BUILDTIME
  printBuildTimes();
#endif
}

void HashOctree::initTree() {
  for (int i = 0; i < voronoiCells.size(); ++i) {
    root->addVoroCell(&(this->voronoiCells[i]));
  }
  root->setBox(min, max);
}

void HashOctree::buildTree() { root->buildTree(); }

void HashOctree::buildHashTable() {
  std::set<OctreeNode *> allNodes = {root};
  root->getAllNodes(allNodes);
  hashTable.rehash(allNodes.size());

  std::vector<OctreeNode *> nodes(allNodes.begin(), allNodes.end());
  std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctreeNode *>
      hashTableThreads[omp_get_max_threads()];
#pragma omp parallel
  {
    std::tuple<int, int, int> p;

    std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctreeNode *>
        &hashTableThread = hashTableThreads[omp_get_thread_num()];
    // hashTableThread.reserve(allNodes.size());
    //  https://en.cppreference.com/w/cpp/container/unordered_map/load_factor
    //  zkusit realoakace

#pragma omp for
    for (OctreeNode *i : nodes) {
      p = findBellogingIntervalsByLevel(i->border.center(), i->level);
      auto pair = std::make_pair(i->level, p);

      hashTableThread.emplace(pair, i);
    }
#pragma omp critical
    { hashTable.merge(hashTableThread); }
  }

  hashTable.max_load_factor(0.5);
}

Point &HashOctree::nn(Point &p) {

  OctreeNode *closesNode = findClosesNode(p);

  // printNodePoints(iterator->second);
  return findClosesPointInNodeLeaf(
      p, closesNode); // find closes point in selected mode
}

Point &HashOctree::findClosesPointInNode(Point &p, const OctreeNode *node) {
  float smallestDistance = std::numeric_limits<float>::max();
  Point *closesPoint;

  for (auto &i : node->voronoiCells) {
    auto distance = i->p.distance(p);
    if (smallestDistance > distance) {
      smallestDistance = distance;
      closesPoint = &i->p;
    }
  }

  return *closesPoint;
}

Point &HashOctree::findClosesPointInNodeLeaf(Point &p, const OctreeNode *node) {
  float smallestDistance = std::numeric_limits<float>::max();
  Point *closesPoint;

  __m256 basex = {p.x, p.x, p.x, p.x, p.x, p.x, p.x, p.x};
  __m256 basey = {p.y, p.y, p.y, p.y, p.y, p.y, p.y, p.y};
  __m256 basez = {p.z, p.z, p.z, p.z, p.z, p.z, p.z, p.z};

  int voronoicellSize = node->voronoiCells.size();
  for (int i = 0; i < voronoicellSize; i += 8) {
    auto &int_voronoicell = node->voronoiCells;

    __m256 x = _mm256_load_ps(&(node->x[i]));
    __m256 y = _mm256_load_ps(&(node->y[i]));
    __m256 z = _mm256_load_ps(&(node->z[i]));

    __m256 diffx = x - basex;
    __m256 diffy = y - basey;
    __m256 diffz = z - basez;

    __m256 diffxpow = diffx * diffx;
    __m256 diffypow = diffy * diffy;
    __m256 diffzpow = diffz * diffz;

    __m256 add = diffxpow + diffypow + diffzpow;

    __m256 distance = _mm256_sqrt_ps(add);

    float tmp[8] __attribute__((aligned(32)));
    _mm256_store_ps(tmp, distance);

    if (voronoicellSize - i < 8) {
      for (int j = 0; j < voronoicellSize - i; j++) {
        if (tmp[j] < smallestDistance) {
          smallestDistance = tmp[j];
          closesPoint = &int_voronoicell[i + j]->p;
        }
      }
      break;
    } else {
      for (int j = 0; j < 8; j++) {
        if (tmp[j] < smallestDistance) {
          smallestDistance = tmp[j];
          closesPoint = &int_voronoicell[i + j]->p;
        }
      }
    }
  }
  return *closesPoint;
}

void HashOctree::printHashTable() {

  for (auto &[key, value] : hashTable) {
    std::cout << std::format("({},({},{},{})) ---> ( \n", key.first,
                             std::get<0>(key.second), std::get<1>(key.second),
                             std::get<2>(key.second));
    printNodePoints(value);
    //  std::cout << std::format("{:2f},{:2f},{:2f}", value->border.min.x,
    //  value->border.min.y, value->border.min.z);
    std::cout << " )" << std::endl;
  }
}

void HashOctree::printNodePoints(OctreeNode *value) {
  for (auto j : value->voronoiCells) {
    std::cout << std::format("      ({:.2f},{:.2f},{:.2f})", j->p.x, j->p.y,
                             j->p.z);
    std::cout << "\n";
  }
}

std::vector<OctreeNode *> HashOctree::getLeafs() {
  std::vector<OctreeNode *> leafs;
  root->getLeafs(leafs);
  return leafs;
}

std::set<OctreeNode *> HashOctree::getAllNodes() {
  std::set<OctreeNode *> allNodes;
  root->getAllNodes(allNodes);

  return allNodes;
}

void HashOctree::saveVoroCellToFile(voro::container_3d &con) {
  // Output the particle positions in gnuplot format
  con.draw_particles("random_points_p.gnu");

  // Output the Voronoi cells in gnuplot format
  con.draw_cells_gnuplot("random_points_v.gnu");

  // splot 'random_points_p.gnu' u 2:3:4 with points, 'random_points_v.gnu' with
  // lines
}

std::vector<Point *> HashOctree::knn(Point &p, int k) {
  OctreeNode *closesNode = findClosesNode(p);
  closesNode = closesNode->reverseTreeLookup(k);

  std::vector<Point *> knn = closesNode->getPoints(closesNode);

  auto distanceFunction = [&p](Point *a, Point *b) {
    return a->distance(p) < b->distance(p);
  };

  std::sort(knn.begin(), knn.end(), distanceFunction);

  return knn;
}

OctreeNode *HashOctree::findClosesNode(Point &p) {
  int lmin = 1; // minimal leaf node level
  int lmax = maxLevel;
  int lc;

  std::tuple<int, int, int> idx = {0, 0, 0};

  while (lmax - lmin > 1) {
    lc = (lmax + lmin) / 2;
    idx = findBellogingIntervalsByLevel(p, lc);
    auto pair = std::make_pair(lc, idx);
    auto iterator = this->hashTable.find(pair);
    if (iterator != hashTable.end()) { // node does exists
      [[likely]] lmin = lc;
      // change range to [lc, lmax]
    } else { // node does not exists
      lmax = lc - 1;
      // change range to [lmin, lc - 1]
    }
  }

  lc = (lmax + lmin) / 2;
  idx = findBellogingIntervalsByLevel(p, lc);
  auto pair = std::make_pair(lc, idx);
  auto iterator = hashTable.find(pair);

  return iterator->second;
}
void HashOctree::createArrayofPoints() {

  for (auto &i : hashTable) {
    int countofVoroCell = (i.second->voronoiCells.size());
    int countofVoroCellRoudnBy8 = roundUp8(countofVoroCell);
    i.second->x = new (std::align_val_t(32)) float[countofVoroCellRoudnBy8];
    i.second->y = new (std::align_val_t(32)) float[countofVoroCellRoudnBy8];
    i.second->z = new (std::align_val_t(32)) float[countofVoroCellRoudnBy8];
    for (int j = 0; j < i.second->voronoiCells.size(); j++) {
      i.second->x[j] = i.second->voronoiCells[j]->p.x;
      i.second->y[j] = i.second->voronoiCells[j]->p.y;
      i.second->z[j] = i.second->voronoiCells[j]->p.z;
    }
  }
}

/**
 * @brief Find first point oriented in the same direction as p or the closes
 * one. ignoring distance as long as is the same octree node (means
 * that they're probably close)
 * @param p
 * @return Point&
 */
Point &HashOctree::nnBestNormalLeaf(Point &p, int effort) {
  OctreeNode *searchedNode = findClosesNode(p);

  std::vector<Point *> knn = OctreeNode::getPoints(searchedNode);

  float smallest_angel = -1;
  Point *smallest_angel_point = knn[0];
  for (auto i : knn) {
    float angel = dotNorm(*i, p);
    if (angel > smallest_angel) {
      smallest_angel = angel;
      smallest_angel_point = i;
      if (angel == 1) [[unlikely]]
        break;
    }
  }
  return *smallest_angel_point;
}

/**
 * @brief Aproximation of the NN search with normal comprasion.
 * It will search only points in set node return first point with angel bigger
 * than (1 - tolerance)
 * @param p Point
 * @param effort if 0 the search is made in leaf node.
 * If 1 it will search in the parent of leaf node. And so on ...
 * @param tolerance if the angel between the point and the closes point is
 * bigger than 1 - tolerance it will stop searching.
 * @return Point&
 */
Point &HashOctree::nnFirstNormal(Point &p, int effort, float tolerance) {
  OctreeNode *searchedNode = findClosesNode(p);

  searchedNode = searchedNode->reverseTreeLookupC(effort);

  auto distanceFunction = [&p](Point *a, Point *b) {
    return a->distance(p) < b->distance(p);
  };

  std::vector<Point *> knn = OctreeNode::getPoints(searchedNode);

  std::sort(knn.begin(), knn.end(), distanceFunction);

  float smallest_angel = -1;
  Point *smallest_angel_point = knn[0];
  for (auto i : knn) {
    float angel = dotNorm(*i, p);
    if (angel > smallest_angel) {
      smallest_angel = angel;
      smallest_angel_point = i;
      if (angel > 1 - tolerance) [[unlikely]]
        break;
    }
  }
  return *smallest_angel_point;
}

/**
 *
 * @param p
 * @param tolerance
 * @return
 */
nnNormalIterator HashOctree::nnNormalSearch(Point &p, float distance) {
  OctreeNode *searchedNode = findClosesNode(p);

  auto w = -this->min + this->max;

  float wmax = std::max(w.x, std::max(w.y, w.z));

  int level = static_cast<int>(std::ceil((std::log2(((wmax / distance)))))) -
              (searchedNode->level);

  searchedNode = searchedNode->reverseTreeLookupC(level);

  std::vector<Point *> points = searchedNode->getPoints(searchedNode);

  return nnNormalIterator{points, p};
}
