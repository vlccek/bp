/**
 * @file main.cpp
 * @brief Main file for testing the HashOctree
 * @author Jakub Vlk
 */

#include "point.h"
#include "randompoint.h"
#include <ranges>

#include "hastree.h"
#include "omp.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>

constexpr float from = -1000000;
constexpr float to = 1000000;

const int ten_milion = 10000000;
const int milion = 1000000;

int main() {

  auto p = genPoints<from, to>(100000);

  HashOctree tree(p, from, to, omp_get_max_threads(), 8);

  // for (std::weakly_incrementable auto l : std::views::iota(0, 5000))

  Point test(0, 0, 0);

  auto res = tree.knn(test, 512);

  return 0;
}
