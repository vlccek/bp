

#include "gjk.h"

bool isBegininside(std::vector<Point> &p) {
  auto b = BoudingBox(p);
  Point ploint(0, 0, 0);
  return b.isInside(ploint);
}

bool gjk(Box &b, Polyhedron *p) {

  std::array<Point, 8> boxVertex = b.allVertex();
  std::vector<Point> polyVertex = p->vertexPoints;

  std::vector<Point> minikowskiDiff;

  for (auto &i : boxVertex) {
    for (auto &j : polyVertex) {
      minikowskiDiff.push_back(i - j);
    }
  }

  return isBegininside(minikowskiDiff);
}



Point findFurherestPoint(std::vector<Point> &p, Point &d) {
  float maxdist = std::numeric_limits<float>::min();
  Point &mindistPoint = d;
  for (auto &i : p) {
    float dist = dot(i, d);
    if (dist > maxdist) {
      maxdist = dist;
      mindistPoint = i;
    }
  }
  return mindistPoint;
}

float dot(const Point &a, const Point &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}