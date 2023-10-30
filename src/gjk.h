/**
 * Module for computing if box and Polyhedron are intersecting by gjk algorithm
 * Create by Jakub Vlk
 * Heavily inspired by https://winter.dev/articles/gjk-algorithm
 */

#ifndef BP_GJK_H
#define BP_GJK_H

#include "point.h"
#include "box.h"
#include "polyhedron.h"
#include <iostream>
#include <format>

/**
 * Function for computing new point of interest
 * @param b box
 * @param p polyhedron
 * @param d direction of interest
 * @return new point of interest
 */
Point support(Box &b, Polyhedron *p, Point d);

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
bool nextSimplex(std::vector<Point> &simplex, Point &direction);

/**
 * Check if tetrahedron express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool tetrahedron(std::vector<Point> &simplex, Point &direction);

/**
 * Check if triangle express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool triangle(std::vector<Point> &simplex, Point &direction);

/**
 * Check if triangle express intersection
 * @param simplex
 * @param direction
 * @return if intersect
 */
bool line(std::vector<Point> &simplex, Point &direction);


/**
 * Compute if 2 vector are in same direction
 * @param simplex
 * @param direction
 * @return
 */
bool sameDirection(const Point &direction, const Point &ao);

#endif //BP_GJK_H
