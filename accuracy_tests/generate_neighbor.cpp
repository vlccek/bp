#include <format>
#include <iostream>
#include <map>
#include <omp.h>
#include <random>
#include <ranges>
#include <vector>

#include "hastree.h"
#include "point.h"
#include "randompoint.h"

void printAllPoints(std::vector<Point *> &p, std::string name) {
  std::ofstream MyFile(name);
  for (auto i : p) {
    MyFile << std::format("{} {} {}", i->x, i->y, i->z) << std::endl;
  }

  MyFile.close();
}

std::vector<Point *> vectOfValuesToVectOfPoints(std::vector<Point> &p) {
  std::vector<Point *> res;
  for (auto &i : p) {
    res.push_back(&i);
  }
  return res;
}

void compute_all_neibors(std::vector<Point *> &p,
                         std::vector<Point> &tested_points, std::string name,
                         Point *begin) {

  std::ofstream MyFile(name);
  for (auto i : tested_points) {

    auto distanceFunction = [&i](Point *a, Point *b) {
      return a->distance(i) < b->distance(i);
    };

    std::sort(p.begin(), p.end(), distanceFunction);

    for (Point *i : p | std::ranges::views::take(1000)) {
      MyFile << std::format("{}", i - begin) << std::endl;
    }
    MyFile << std::endl;
  }
}

int main() {
  constexpr float from = -10000000;
  constexpr float to = 10000000;

  std::pair<int, std::string> testCases[] = {{100, "100"},
                                             {1000, "1k"},
                                             {10000, "10k"},
                                             {100000, "100k"},
                                             {1000000, "1m"}};

  auto testedPoints = genPoints<from, to>(1000);

#pragma omp parallel for
  for (std::weakly_incrementable auto l : std::views::iota(0, 50)) {
    for (auto &i : testCases) {

      // todo skip

      std::string name = i.second;
      int number = i.first;

      auto p = genPoints<from, to>(number);

      auto insetedPoints = vectOfValuesToVectOfPoints(p);
      auto testedPointsVect = vectOfValuesToVectOfPoints(testedPoints);

      printAllPoints(insetedPoints, std::format("points_{}_{}.txt", name, l));
      printAllPoints(testedPointsVect,
                     std::format("points_{}_{}_tested.txt", name, l));
      compute_all_neibors(
          insetedPoints, testedPoints,
          std::format("points_{}_{}_computed_neigbors.txt", name, l),
          insetedPoints[0]);
    }
  }
}