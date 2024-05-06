/**
* @file gjk.h
* @brief Implementation of the GJK algorithm, inspired by https://winter.dev/articles/gjk-algorithm
* @author Jakub Vlk

*/

#ifndef BP_GJK_H
#define BP_GJK_H

#include "box.h"
#include "point.h"
#include "polyhedron.h"
#include <format>
#include <iostream>

class Simplex;

/**
 * Function for computing new Point of interest
 * @param b box
 * @param p polyhedron
 * @param d direction of interest
 * @return new Point of minikoski difference
 */
Point support(Box &b, Polyhedron *p, Point &d);

/**
 * Main function for running intersecting check
 * @param b box
 * @param p Polyhedron
 * @return if b intersect p
 */
bool gjk(Box &b, Polyhedron *p);

/**
 * Compute next simplex and if algorithm should continue
 * @param simplex
 * @param direction
 * @return
 */
bool nextSimplex(Simplex &simplex, Point &direction);

/**
 * Check if tetrahedron express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool tetrahedron(Simplex &simplex, Point &direction);

/**
 * Check if triangle express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool triangle(Simplex &simplex, Point &direction);

/**
 * Check if triangle express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool line(Simplex &simplex, Point &direction);

/**
 * Compute if 2 vector are in same direction
 * @param simplex
 * @param direction
 * @return
 */
bool sameDirection(const Point &direction, const Point &ao);

Point findFurherestPoint(std::vector<Point> &p, Point &d);

float dot(const Point &a, const Point &b);

class Simplex {
  std::array<Point, 4> points = {
      {Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 0)}};
  int simplex_size = 1;

public:
  Simplex() : simplex_size(0){};

  Simplex &operator=(std::initializer_list<Point> list) {
    simplex_size = 0;
    for (Point point : list)
      points[simplex_size++] = point;

    return *this;
  }

  void push(Point p);

  Point &operator[](int i) { return points[i]; }
  int size() const { return simplex_size; }

  auto begin() const { return points.begin(); }
  auto end() const { return points.end() - (4 - simplex_size); }
};

#endif // BP_GJK_H
