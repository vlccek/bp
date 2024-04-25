#include "point.h"
#include "randompoint.h"
#include <ranges>

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

  auto p = genPoints<0.f, 100.f>(1000);

  HashOctree tree(p, 0, 100, omp_get_max_threads(), 16);

  for (std::weakly_incrementable auto l : std::views::iota(0, 5000)) {
    for (auto &i : p) {
      auto res = tree.nn(i);
    }
  }

  return 0;
}
