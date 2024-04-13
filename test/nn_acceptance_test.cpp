#include <format>
#include <iostream>
#include <map>
#include <random>
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

void compute_10_neibors(std::vector<Point *> &p,
                        std::vector<Point> &tested_points, std::string name,
                        Point *begin) {

  std::ofstream MyFile(name);
  for (auto i : tested_points) {

    auto distanceFunction = [&i](Point *a, Point *b) {
      return a->distance(i) < b->distance(i);
    };

    std::sort(p.begin(), p.end(), distanceFunction);

    for (Point *i : p) {
      MyFile << std::format("{}", i - begin) << std::endl;
    }
    MyFile << std::endl;
  }
}

std::vector<Point> loadPointsFromFile(std::string filename) {
  std::vector<Point> rest;

  std::ifstream file(filename);
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
  std::vector<std::vector<Point *>> map = {std::vector<Point *>()};

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

void generateFiles() {
  constexpr int from = -100;
  constexpr int to = 100;
  auto testedPoints = genPoints<from, to>(10);
  auto p = genPoints<from, to>(100);

  auto insetedPoints = vectOfValuesToVectOfPoints(p);
  auto testedPointsVect = vectOfValuesToVectOfPoints(testedPoints);

  printAllPoints(insetedPoints, "points_1m.txt");
  printAllPoints(testedPointsVect, "points_1m_tested.txt");
  compute_10_neibors(insetedPoints, testedPoints,
                     "points_1m_coputed_neigbors.txt", insetedPoints[0]);
}

double distance(Point &a, Point &b) {
  return std::sqrt(std::pow(static_cast<double>(a.x - b.x), 2) +
                   std::pow(static_cast<double>(a.y - b.y), 2) +
                   std::pow(static_cast<double>(a.z - b.z), 2));
}

int main() {
  constexpr int from = -100;
  constexpr int to = 100;

#if 0
  generateFiles();

  return 0;
#endif
  auto insetedPoints = loadPointsFromFile("points_1m.txt");
  auto testedPoints = loadPointsFromFile("points_1m_tested.txt");
  auto knn = loadPointsFromFilePointer("points_1m_coputed_neigbors.txt",
                                       insetedPoints.data());

  HashOctree tree(insetedPoints, from, to);

  int err_case = 0;
  int err_case_float = 0;

  int kn_size = 20; // how many neigbors are computed

  double err_sum = 0;

  for (int l = 0; l < testedPoints.size(); l++) {
    auto a = tree.knn(testedPoints[l], kn_size);
    // a.resize(kn_size);
    std::vector<Point *> b = knn[l];
    for (int i = 0; i < a.size(); i++) {
      if (*a[i] != *b[i])
        err_case_float++;
      if (testedPoints[l].distance(*b[i]) != testedPoints[l].distance(*a[i])) {
        err_case++;

        err_sum += std::abs(distance(testedPoints[l], *a[i]) -
                            distance(testedPoints[l], (*b[i])));
        continue;
        std::cerr << "Error in " << i << " Diffrence: "
                  << std::format("{} ({})", distance(testedPoints[l], *b[i]),
                                 testedPoints[l].distance(*b[i]))
                  << " vs: "
                  << std::format("{} ({})", distance(testedPoints[l], *a[i]),
                                 testedPoints[l].distance(*a[i]))

                  << std::format(" {} {} {}", (*b[i]).x, (*b[i]).y, (*b[i]).z)
                  << " vs: "
                  << std::format(" {} {} {}", (*a[i]).x, (*a[i]).y, (*a[i]).z)

                  << std::endl;
      }
    }
    std::cerr << std::format("Error case: {} {}%\nFloat error case: {} \n"
                             "Err_avg: {} (total: {})",
                             err_case, // err case
                             (err_case / (double)(a.size())) *
                                 100,                    // precentage
                             err_case_float,             // float error case
                             err_sum / (double)a.size(), // avg error
                             err_sum)                    // total error
              << std::endl;
    err_sum = 0;
    err_case = 0;
  }
}
