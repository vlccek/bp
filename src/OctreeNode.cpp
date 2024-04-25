//
// Created by jvlk on 18.10.23.
//

#include "OctreeNode.h"

#define ONLY_GJK 0

OctreeNode::OctreeNode(int level, int *maxLevel, const int maxPointsIncell,
                       OctreeNode *parent)
    : maxPointsInNode(maxPointsIncell) {
  this->maxLevel = maxLevel;
  this->level = level;
  this->parent = parent;

  if (level > *maxLevel) {
    *maxLevel = level;
  }
}

void OctreeNode::allocateIfNeccesary(int index, Box &b) {
  if (childs[index] == nullptr) {
    childCount++;
    childs[index] = new OctreeNode(level + 1, maxLevel, maxPointsInNode, this);
    childs[index]->border = b;
  }
}

void OctreeNode::getLeafs(std::vector<OctreeNode *> &leafs) {
  for (auto &child : childs) {
    if (child != nullptr) {
      if (child->childCount == 0) {
        leafs.push_back(child);
        continue;
      } else {
        child->getLeafs(leafs);
      }
    }
  }
}


void OctreeNode::printVoronoiCells(std::ofstream &file) {
  for (auto &vc : voronoiCells) {
    // file << vc->p.operator std::string() << std::endl;
    for (auto &p : vc->vertexPoints) {
      file << p.operator std::string() << std::endl;
    }
    file << std::endl; // empty line
  }
}

void OctreeNode::getAllNodes(std::set<OctreeNode *> &allNodes) {
#pragma omp parallel
  {
#pragma omp for
    for (auto child : childs) {
      if (child == nullptr) {
        continue;
      }
#pragma omp critical
      allNodes.insert(child);
      // iterate through all nodes
      child->getAllNodes(allNodes);
    }
  }
}

bool OctreeNode::intersect(Box &b, Polyhedron &vc) {
  Polyhedron *ph = &vc;
  Point particle = vc.p;
  std::vector<Point> &vp = vc.vertexPoints; // vertex point of the voronoi cell

  // center point of voronoi cell
  auto box = b;

  // is particle in BOX ?
  if (box.isInside(particle)) {
    return true;
  }

  // is one of vertex of voronoi cell in box ?
  if (box.isInside(vp)) {
    return true;
  }
  // check by bounding box
  auto boudingboxVertexs = ph->boudingBox.allVertex();
  // check if bounding box interferes to the box
  if (box.isInside(boudingboxVertexs)) {
    // use gjk algorithm to detect intersection
    // if 1 skips gjk algorithm and just return true (if bounding box
    // intersects)
#if 0
        return true;
#else
    if (gjk(box, ph)) {
      return true;
    }
#endif
  }
  return false;
}

void OctreeNode::buildTree() {
  if (border.isTooSmall()) {
    std::cout << "Unable to build tree becouse the region is too small"
              << std::endl;
    exit(10);
  }

  std::array<Box, 8> splitedBox = border.splitBoxBy8();

#pragma omp parallel
  {
#pragma omp single
    {
      for (int l = 0; l < splitedBox.size(); ++l) {

#pragma omp task
        {
          // go through all voronoi cells and decided if the cell intersects
          // with the box
          for (auto &voronoiCell : voronoiCells) {
            Box &box = splitedBox[l];
            Polyhedron *ph = voronoiCell;
            if (intersect(box, *voronoiCell)) {
              allocateIfNeccesary(l, box);
              childs[l]->addVoroCell(ph);
            }
          }
          if (childs[l] != nullptr &&
              childs[l]->voronoiCells.size() >= maxPointsInNode) {

            childs[l]->buildTree();
          }
        }
      }
    }
  }
}
