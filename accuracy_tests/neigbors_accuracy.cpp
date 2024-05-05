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

std::vector<Point> loadPointsFromFile(std::string filename) {
  std::vector<Point> rest;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "File not found" << std::endl;
    exit(1);
  };
  std::string str;
  while (std::getline(file, str)) {
    std::istringstream iss(str);
    float x, y, z;
    iss >> x >> y >> z;
    rest.emplace_back(x, y, z);
  }
  return rest;
}

std::vector<std::vector<Point *>>
loadPointsFromFilePointer(std::string filename, Point *begin) {
  std::vector<std::vector<Point *>> map{std::vector<Point *>()};

  std::ifstream file(filename);
  std::string str;
  int c = 0;
  while (std::getline(file, str)) {
    if (str.empty()) {
      map.push_back(std::vector<Point *>());
      c++;
      continue;
    }
    std::istringstream iss(str);
    int i;
    iss >> i;
    map[c].emplace_back(begin + i);
  }
  return map;
}

double distance(Point &a, Point &b) {
  return std::sqrt(std::pow(static_cast<double>(a.x - b.x), 2) +
                   std::pow(static_cast<double>(a.y - b.y), 2) +
                   std::pow(static_cast<double>(a.z - b.z), 2));
}

int main() {
  constexpr float from = -10000000;
  constexpr float to = 10000000;

  std::vector<std::string> testCases = {
      "100", "1k", "10k", "100k", "1m",
      //"10m"
  };

  auto mmax_range = {8, 16, 32, 64, 96, 128, 256};

#ifndef FROM_acc
#define FROM_acc 0
#endif

#ifndef TO_acc
#define TO_acc 50
#endif


  // auto mmax_range = {128, 256};

#if NNTEST
  for (std::weakly_incrementable auto test_number : std::views::iota(FROM_acc, TO_acc)) {
    for (auto &testcase : testCases) {

      for (auto &m_max : mmax_range) {
        if (testcase == "10m" && m_max == 5) {
          continue;
        }

        auto insetedPoints = loadPointsFromFile(
            std::format("points_{}_{}.txt", testcase, test_number));
        auto testedPoints = loadPointsFromFile(
            std::format("points_{}_{}_tested.txt", testcase, test_number));
        auto knn = loadPointsFromFilePointer(
            std::format("points_{}_{}_computed_neigbors.txt", testcase,
                        test_number),
            insetedPoints.data());

        HashOctree tree(insetedPoints, from, to, omp_get_max_threads(), m_max);

        int err_case = 0;
        int all_case = testedPoints.size();
        double err_sum = 0;
        double distance_sum = 0;

        for (int i = 0; i < testedPoints.size(); i++) {
          Point &testPoint = testedPoints[i];
          Point &aNeighbor = *knn[i][0]; // accurate neighbor
          Point neighbor = tree.nn(testPoint);

          distance_sum = distance(aNeighbor, testPoint);

          if (neighbor != aNeighbor) {
            if (testPoint.distance(neighbor) != testPoint.distance(aNeighbor)) {
              err_case++;
              err_sum += std::abs(distance(testPoint, neighbor) -
                                  distance(testPoint, aNeighbor));
            }
          }
        }
#if 0

      cerr << std::format(
                  "Error case for {} and m_max {}:\n"
                  "{}% (err cases: {})\n"
                  "Err_avg: {} (total: {})",
                  testcase, m_max, (err_case / (double)all_case) * 100, err_case,
                  err_sum / (double)all_case, err_sum)
           << endl;
#endif

        std::cout << std::format("{} {} {} {} {} {} ", testcase, test_number,
                                 m_max, (err_case / (double)all_case),
                                 err_sum / (double)all_case,
                                 distance_sum / all_case)
                  << endl;
      }
    }
  }
#else
  std::cout << "name tested_point_number test_number kn_size m_max accuracy "
               "avg_err err_case_float\n";



  for (std::weakly_incrementable auto test_number : std::views::iota(FROM_acc, TO_acc)) {
    for (auto &testcase : testCases) {
      for (auto &m_max : mmax_range) {

        int err_case = 0;
        int err_case_float = 0;

        // how many neigbors are computed

        double err_sum = 0;

        auto insetedPoints = loadPointsFromFile(
            std::format("points_{}_{}.txt", testcase, test_number));
        auto testedPoints = loadPointsFromFile(
            std::format("points_{}_{}_tested.txt", testcase, test_number));
        auto knns = loadPointsFromFilePointer(
            std::format("points_{}_{}_computed_neigbors.txt", testcase,
                        test_number),
            insetedPoints.data());

        HashOctree tree(insetedPoints, from, to, omp_get_max_threads(), m_max);

        for (int kn_size : {8, 16, 32, 64, 96, 128, 256, 512, 1000}) {
          // test
          for (int l = 0; l < testedPoints.size(); l++) {
            auto a = tree.knn(testedPoints[l], kn_size);
            // a.resize(kn_size);
            vector<Point *> knn = knns[l];
            if (knn.size() < kn_size) {
              continue;
            }
            for (int i = 0; i < kn_size; i++) {
              if (*a[i] != *knn[i])
                err_case_float++;
              if (testedPoints[l].distance(*knn[i]) !=
                  testedPoints[l].distance(*a[i])) {
                err_case++;

                err_sum += std::abs(distance(testedPoints[l], *a[i]) -
                                    distance(testedPoints[l], (*knn[i])));
              }
            }

            std::cout << std::format(
                             "{} {} {} {} {} {} {} {}",
                             testcase, // test case
                             l,
                             test_number,                    // test number
                             kn_size,                        // kn size
                             m_max,                          // m_max
                             (err_case / (double)(kn_size)), // precentage
                             err_sum / (double)kn_size,      // avg error
                             err_case_float)
                      << endl;

            err_sum = 0;
            err_case = 0;
          }
        }
      }
    }
  }

#endif
}
