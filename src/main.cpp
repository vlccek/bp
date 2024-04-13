#include "point.h"
#include "randompoint.h"

#include "hastree.h"
#include "omp.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>

using namespace voro;

const int ten_milion = 10000000;
const int milion = 1000000;

int main() {

  auto p = genPoints<0, 100>(10000);

  HashOctree tree(p, 0, 100, omp_get_max_threads());

  std::vector<Point *> res = tree.knn(p[4], 6);

  return 0;
}
