//
// Created by jvlk on 27.9.23.
//

#include "hastree.h"

HashOctree::HashOctree(std::vector<Point> &p, const Point &min,
                       const Point &max, const int mmax)
    : con(min.x, max.x, min.y, max.y, min.z, max.z, 6, 6, 6, false, false,
          false, 8),
      mmax(mmax) {
  int id = 0;
  for (auto i : p) {
    con.put(id++, i.x, i.y, i.z);
  }
  con.compute_all_cells();
  voro::c_loop_subset cls(con);
  int c_voronoi = 0; // counter of crosing voroni-s and rectangle

  std::queue<Box> q;
  split_and_enqueue(min, max, q);

  int c = 0;

  OctreeLeaf *tmp_parent = root;

  // TODO hodit do Boxu abych věděl kdo je parent :)))
  do {
    c++;
    /*
     * 1) Setup new box by
     * 2) find how many voronoi cells are crossed by this box
     *      a) If more then (inc) mmax: split by 8
     *      b) else: add to the tree
     * 3)
     */
    double x, y, z;
    Box procesed_box = q.front();
    q.pop();

    cls.setup_box(procesed_box.min.x, procesed_box.max.x, procesed_box.min.y,
                  procesed_box.max.y, procesed_box.min.z, procesed_box.max.z,
                  false);

    if (cls.start()) {

      do {
        c_voronoi++;
      } while (cls.inc() && c_voronoi < this->mmax);

      OctreeLeaf *ot;

      if (c_voronoi < this->mmax) {
        ot = new OctreeLeaf();
      } else {
        ot = new OctreeLeaf({});
      }

      tmp_parent->childs[procesed_box.level_order] = new OctreeLeaf();
    }
    c_voronoi = 0;

  } while (!q.empty());
}

inline void HashOctree::split_and_enqueue(const Point &min, const Point &max,
                                          std::queue<Box> &q) {
  for (auto i : splitboxby8(min, max)) {
    q.push(i);
  }
}

OctreeLeaf::OctreeLeaf(Point &p) : p(p) { valid_point = true; }

OctreeLeaf::OctreeLeaf() : p(0, 0, 0) {}

const Point &OctreeLeaf::getPoint() const { return p; }

bool OctreeLeaf::isPointValid() { return valid_point; }

OctreeLeaf::OctreeLeaf(Point point) : p(point) { valid_point = true; }

Box::Box(Point &min, Point &max, Box *parent, int id)
    : min(min), max(max), level_order(id) {
  // empty :)
}

Box::Box(double xa, double ya, double za, double xb, double yb, double zb,
         int id, Box *parent)
    : min(xa, ya, za), max(xb, yb, zb), level_order(id) {
  // empty :)
}

std::vector<Box> splitboxby8(const Point &minP, const Point &maxP) {

  std::cout << std::format("Spliting box by 8, ({},{},{}) x ({},{},{}) ",
                           minP.x, minP.y, minP.z, maxP.x, maxP.y, maxP.z)
            << std::endl;

  // [0] x, [1] y, [2] z
  // calculates size of box and devide it by 2
  double half_size[3] = {
      (maxP.x - minP.x) / 2,
      (maxP.y - minP.y) / 2,
      (maxP.z - minP.z) / 2,
  };

  // [0] x, [1] y, [2],
  double min[3] = {
      minP.x,
      minP.y,
      minP.z,
  };

  // [0] x, [1] y, [2] z
  double max[3] = {minP.x + half_size[0], minP.y + half_size[1],
                   minP.z + half_size[2]};

  std::vector<Box> boxs;

  int index;
  for (int i = 0; i < 8; ++i) {
    switch (i) {
    case 0:
    case 1:
    case 2:
      min[i] += half_size[i];
      max[i] += half_size[i];
      break;
    case 3:
      index = 1;
      min[index] -= half_size[index];
      max[index] -= half_size[index];
      break;
    case 4:
      index = 0;
      min[index] -= half_size[index];
      max[index] -= half_size[index];
      break;
    case 5:
      index = 1;
      min[index] += half_size[index];
      max[index] += half_size[index];
      break;

    case 6:
      index = 2;
      min[index] -= half_size[index];
      max[index] -= half_size[index];
      break;

    case 7:
      index = 0;
      min[index] += half_size[index];
      max[index] += half_size[index];
      break;
    default:
      break;
    }
    boxs.emplace_back(min[0], min[1], min[2], max[0], max[1], max[2], i);
  }
  return boxs;
}
