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

std::vector<Point> readPointsFromFile(std::string filename) {
  std::vector<Point> rest;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "File not found: " << filename << std::endl;
    exit(1);
  }
  std::string str;
  while (std::getline(file, str)) {
    std::istringstream iss(str);
    float x, y, z, normalx, normaly, normalz;
    iss >> x >> y >> z >> normalx >> normaly >> normalz;
    rest.emplace_back(x, y, z, normalx, normaly, normalz);
  }

  return rest;
}

std::vector<std::vector<Point>> readVecOfPointsFromFile(std::string filename) {
  std::vector<std::vector<Point>> rest = {};

  int expected_PointsGroups = 10;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "File not found: " << filename << std::endl;
    exit(1);
  }
  std::string str;

  std::vector<Point> current;
  while (std::getline(file, str)) {
    if (str.empty()) {
      rest.emplace_back(current);
      current.clear();
      continue;
    }

    std::istringstream iss(str);
    float x, y, z, normalx, normaly, normalz;
    iss >> x >> y >> z >> normalx >> normaly >> normalz;
    current.emplace_back(x, y, z, normalx, normaly, normalz);
  }

  return rest;
}

double distance(Point &a, Point &b) {
  return std::sqrt(std::pow(static_cast<double>(a.x - b.x), 2) +
                   std::pow(static_cast<double>(a.y - b.y), 2) +
                   std::pow(static_cast<double>(a.z - b.z), 2));
}

constexpr float maxDistance = 100000;

int main() {

  constexpr float from = -1000000;
  constexpr float to = 1000000;
  for (auto &number_ofpoints : {
           //100, 1000, 10000,
           //
            1000000
       }) {
    for (auto &mmax : {8, 16, 32, 64, 96, 128, 256}) {
      std::vector<Point> points = readPointsFromFile(
          std::format("points_3d_normal_plane_{}.txt", number_ofpoints));

      HashOctree tree(points, from, to, omp_get_max_threads(), mmax);
      for (std::weakly_incrementable auto test_number :
           std::views::iota(0, 30)) {

        auto correct = readVecOfPointsFromFile(std::format(
            "points_3d_computed_{}_{}.txt", test_number, number_ofpoints));

        std::vector<Point> testedpoints = readPointsFromFile(
            std::format("points_3d_testedpoints_{}.txt", test_number));

        Point closesP(0, 0, 0);

        int count = testedpoints.size();

        for (int l = 0; l < count; l++) {
          Point &t = testedpoints[l];
          std::vector<Point> &correctPoints = correct[l];

          int first_err_count = 0;
          double first_dist_sum = 0, first_err_normal = 0;
          int leaf_err_count = 0;
          double leaf_dist_sum = 0, leaf_err_normal = 0;
          int it_err_count = 0;
          double it_dist_sum = 0, it_err_normal = 0;
          int leaf1_err_count = 0;
          double leaf1_dist_sum = 0, leaf1_err_normal = 0;

          closesP = tree.nnFirstNormal(t, 0.05);

          if (correctPoints.size() == 0) {
            continue;
          }
          if (closesP != correctPoints[0]) {
            first_err_count++;
            first_dist_sum += distance(closesP, correctPoints[0]);
            first_err_normal += dotNorm(closesP, correctPoints[0]) + 1;
          }

          closesP = tree.nnBestNormalLeaf(t, 0);

          if (closesP != correctPoints[0]) {
            leaf_err_count++;
            leaf_dist_sum += distance(closesP, correctPoints[0]);
            leaf_err_normal +=
                dotNorm(closesP, correctPoints[0]) + 1; // interval 0;2
          }

          closesP = tree.nnBestNormalLeaf(t, 1);

          if (closesP != correctPoints[0]) {
            leaf1_err_count++;
            leaf1_dist_sum += distance(closesP, correctPoints[0]);
            leaf1_err_normal +=
                dotNorm(closesP, correctPoints[0]) + 1 // interval 0;2

                ;
          }

          nnNormalIterator it = tree.nnNormalSearch(t, maxDistance);

          Point *closesPP = nullptr;
          int c;
          for (c = 0; c < 10; c++) {
            closesPP = it.next();

            if (*closesPP != correctPoints[c]) {
              it_err_count++;
              it_dist_sum += distance(closesP, correctPoints[c]);
              it_err_normal +=
                  dotNorm(closesP, correctPoints[c]) + 1; // interval 0;2
            }

            if (it.isEmpty()) {
              break;
            }
          }
          c += c == 10 ? 0 : 1; // corection if loope ends with break

          std::cout << std::format("{} {} {} {} {} {} {} {} {}\n", // place
                                   number_ofpoints, test_number, mmax,
                                   "first", // ids
                                   count, first_err_count, first_dist_sum,
                                   first_err_normal, // errors,
                                   1);
          std::cout << std::format("{} {} {} {} {} {} {} {} {}\n", // place
                                   number_ofpoints, test_number, mmax,
                                   "leaf", // ids
                                   count, leaf_err_count, leaf_dist_sum,
                                   leaf_err_normal, 1 // errors
          );
          std::cout << std::format("{} {} {} {} {} {} {} {} {}\n", // place
                                   number_ofpoints, test_number, mmax,
                                   "leaf1", // ids
                                   count, leaf1_err_count, leaf1_dist_sum,
                                   leaf1_err_normal, 1 // errors
          );
          std::cout << std::format("{} {} {} {} {} {} {} {} {}\n", // place
                                   number_ofpoints, test_number, mmax,
                                   "it", // ids
                                   count, it_err_count, it_dist_sum / (c),
                                   it_err_normal / c, // errors
                                   c);
          std::cout.flush();
        }
      }
    }
  }
  return 0;
}
