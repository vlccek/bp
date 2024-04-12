

#include "gjk.h"

bool gjk(Box &b, Polyhedron *p) {

  Point direction = {1, 0, 0};
  Point support_point = support(b, p, direction);

  Simplex simplex;
  simplex.push(support_point);

  int maxinterations = 8*p->vertexPoints.size();

  direction = -support_point;

  while (maxinterations < 0) {
    maxinterations--;


    support_point = support(b, p, direction);

    if (dot(support_point, direction) < 0) {
      return false;
    }

    simplex.push(support_point);

    if (nextSimplex(simplex, direction)) {
      return true;
    }
  }
  return false;
}

Point support(Box &box, Polyhedron *p, Point &d) {
  std::array<Point, 8> boxVertex = box.allVertex();
  std::vector<Point> boxVertexVector(boxVertex.begin(), boxVertex.end());
  std::vector<Point> &polyVertex = p->vertexPoints;

  Point minusD = -d;

  Point a = findFurherestPoint(boxVertexVector, d);
  Point b = findFurherestPoint(polyVertex, minusD);

  return a - b;
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
bool nextSimplex(Simplex &simplex, Point &direction) {

  switch (simplex.size()) {
  case 2:
    return line(simplex, direction);
  case 3:
    return triangle(simplex, direction);
  case 4:
    return tetrahedron(simplex, direction);
  }

  return false;
}

void Simplex::push(Point p) {
  points = {p, points[0], points[1], points[2]};
  simplex_size = std::min(simplex_size + 1, 4);
}

bool sameDirection(const Point &direction, const Point &ao) {
  return dot(direction, ao) > 0;
}

bool line(Simplex &simplex, Point &direction) {
  Point a = simplex[0];
  Point b = simplex[1];

  Point ab = b - a;
  Point ao = -a;

  if (sameDirection(ab, ao)) {
    direction = cross(cross(ab, ao), ab);
  }

  else {
    simplex = {a};
    direction = ao;
  }

  return false;
}
bool triangle(Simplex &simplex, Point &direction) {
  Point a = simplex[0];
  Point b = simplex[1];
  Point c = simplex[2];

  Point ab = b - a;
  Point ac = c - a;
  Point ao = -a;

  Point abc = cross(ab, ac);

  if (sameDirection(cross(abc, ac), ao)) {
    if (sameDirection(ac, ao)) {
      simplex = {a, c};
      direction = cross(cross(ac, ao), ac);
    }

    else {
      return line(simplex = {a, b}, direction);
    }
  }

  else {
    if (sameDirection(cross(ab, abc), ao)) {
      return line(simplex = {a, b}, direction);
    }

    else {
      if (sameDirection(abc, ao)) {
        direction = abc;
      }

      else {
        simplex = {a, c, b};
        direction = -abc;
      }
    }
  }

  return false;
}

bool tetrahedron(Simplex &simplex, Point &direction) {
  Point a = simplex[0];
  Point b = simplex[1];
  Point c = simplex[2];
  Point d = simplex[3];

  Point ab = b - a;
  Point ac = c - a;
  Point ad = d - a;
  Point ao = -a;

  Point abc = cross(ab, ac);
  Point acd = cross(ac, ad);
  Point adb = cross(ad, ab);

  if (sameDirection(abc, ao)) {
    return triangle(simplex = {a, b, c}, direction);
  }

  if (sameDirection(acd, ao)) {
    return triangle(simplex = {a, c, d}, direction);
  }

  if (sameDirection(adb, ao)) {
    return triangle(simplex = {a, d, b}, direction);
  }

  return true;
}
