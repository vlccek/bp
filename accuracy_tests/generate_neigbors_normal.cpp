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

constexpr float from = -1000000;
constexpr float to = 1000000;

constexpr float maxDistance = 100000;

int main() {

#pragma omp parallel for
  for (std::weakly_incrementable auto test_number : std::views::iota(0, 30)) {
    for (auto &number_ofpoints : {100, 1000, 10000, 100000, 1000000}) {
      std::vector<Point> points = readPointsFromFile(
          std::format("points_3d_normal_plane_{}.txt", number_ofpoints));

      std::vector<Point> testedpoints = readPointsFromFile(
          std::format("points_3d_testedpoints_{}.txt", test_number));

      std::ofstream file(std::format("points_3d_computed_{}_{}.txt",
                                     test_number, number_ofpoints));
      for (auto &t : testedpoints) {

        std::vector<Point> filtered_points;
        for (auto a : points) {
          if (a.distance(t) < maxDistance) {
            filtered_points.push_back(a);
          }
        }

        auto normalComparsionFuction = [&t](Point &a, Point &b) {
          return dotNorm(a, t) > dotNorm(b, t); // smaller better :)
        };

        std::sort(filtered_points.begin(), filtered_points.end(),
                  normalComparsionFuction);

        for (auto &l : filtered_points | std::views::take(10)) {
          file << std::format("{} {} {} {} {} {}", l.x, l.y, l.z, l.nor_x,
                              l.nor_y, l.nor_z)
               << std::endl;
        }
        file << std::endl;
      }
      file.close();
    }
  }
  return 0;
}
