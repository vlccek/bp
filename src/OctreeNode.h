//
// Created by jvlk on 18.10.23.
//

#ifndef BP_OCTRERNODEBUILDER_H
#define BP_OCTRERNODEBUILDER_H

#include "voro++.hh"
#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "OctreeNode.h"
#include "box.h"
#include "gjk.h"
#include "point.h"
#include "polyhedron.h"

/**
 * Class for building temporary tree for computing tree layout
 */
class OctreeNode {

public:
  explicit OctreeNode(int level, int *maxLevel, int maxPointsIncell = 5,
                      OctreeNode *parent = nullptr);

  Box border = Box({0, 0, 0}, {0, 0, 0});

  int childCount = 0;
  int level;
  int *maxLevel;
  OctreeNode *parent = nullptr;

  const int maxPointsInNode;

  std::vector<Polyhedron *> voronoiCells;
  std::array<OctreeNode *, 8> childs = {0};

  inline void setBox(Point &min, Point &max) {
    border.min = min;
    border.max = max;
  }

  void buildTree();

  void alocateIfNeccesary(int index, Box &b);

  bool intersect(Box &b, Polyhedron &vc);

  inline void addVoroCell(Polyhedron *vc) { voronoiCells.push_back(vc); }

  void getLeafs(std::vector<OctreeNode *> &leafs);

  void printVoronoiCells(std::ofstream &file);

  ~OctreeNode();

  void getAllNodes(std::set<OctreeNode *> &allNodes);

  inline std::vector<Point *> getPoints(OctreeNode *node) {
    std::vector<Point *> points;
    points.reserve(node->voronoiCells.size());
    for (auto &i : node->voronoiCells) {
      points.push_back(&i->p);
    }
    return points;
  }

  inline OctreeNode *reverseTreeLookup(int reqquiredPoints) {
    OctreeNode *node = this;
    while (node->voronoiCells.size() < reqquiredPoints*2) {
      if (node->parent == nullptr) {
        return node;
      }
      node = node->parent;
    }
    return node;
  }
};

#endif // BP_OCTRERNODEBUILDER_H
