//
// Created by jvlk on 18.10.23.
//

#include "box.h"
#include <iostream>

Box::Box(Point &min, Point &max) : min(min), max(max) {
  // empty :)
}

Box::Box(Point min, Point max) : min(min), max(max) {
  // empty :)
}

Box::Box(float xa, float ya, float za, float xb, float yb, float zb)
    : min(xa, ya, za), max(xb, yb, zb) {
  // empty :)
}

Box::~Box() { delete splited; }

std::array<Box, 8> Box::splitBoxBy8() { return splitboxby8(min, max); }

void Box::writeGnuFormat(std::string &filename) {
  std::ofstream file;
  file.open(filename);
  int id = 0;
  auto allVertex = this->allVertex();
  for (auto l : {0, 1, 2, 3, 0, 7, 5, 3, 5, 4, 6, 7, 5, 4, 2, 1, 6}) {
    auto i = allVertex[l];
    file << std::format("{} {} {}", i.x, i.y, i.z) << std::endl;
  }

  file.close();
}

BoudingBox::BoudingBox(std::vector<float> &d) : Box({0, 0, 0}, {0, 0, 0}) {
  for (int i = 0; i < d.size(); ++i) {
    if (min.x > d[i]) {
      min.x = d[i];
    }
    if (min.y > d[i + 1]) {
      min.y = d[i + 1];
    }
    if (min.z > d[i + 2]) {
      min.z = d[i + 2];
    }
    if (max.x > d[i]) {
      min.x = d[i];
    }
    if (max.y > d[i + 1]) {
      min.y = d[i + 1];
    }
    if (max.z > d[i + 2]) {
      min.z = d[i + 2];
    }
  }
}

BoudingBox::BoudingBox(std::vector<Point> &vertex)
    : Box({std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
           std::numeric_limits<float>::max()},
          {std::numeric_limits<float>::min(), std::numeric_limits<float>::min(),
           std::numeric_limits<float>::min()}) {
  for (auto i : vertex) {
    if (min.x > i.x) {
      min.x = i.x;
    }
    if (min.y > i.y) {
      min.y = i.y;
    }
    if (min.z > i.z) {
      min.z = i.z;
    }
    if (max.x < i.x) {
      max.x = i.x;
    }
    if (max.y < i.y) {
      max.y = i.y;
    }
    if (max.z < i.z) {
      max.z = i.z;
    }
  }
}

std::array<Box, 8> splitboxby8(const Point &minP, const Point &maxP) {

  // std::cout << std::format("splitboxby8 with min: {} and max: {}\n",
  // minP.operator std::string(), maxP.operator std::string());

  const Point min = minP;
  const Point max = maxP;
  // Rozdělení podle středu každé strany
  float midX = (min.x + max.x) / 2.0f;
  float midY = (min.y + max.y) / 2.0f;
  float midZ = (min.z + max.z) / 2.0f;

  // Vytvoření osmi podboxů
  std::array<Box, 8> result = {Box(min, {midX, midY, midZ}),
                               Box({midX, min.y, min.z}, {max.x, midY, midZ}),
                               Box({min.x, midY, min.z}, {midX, max.y, midZ}),
                               Box({midX, midY, min.z}, {max.x, max.y, midZ}),
                               Box({min.x, min.y, midZ}, {midX, midY, max.z}),
                               Box({midX, min.y, midZ}, {max.x, midY, max.z}),
                               Box({min.x, midY, midZ}, {midX, max.y, max.z}),
                               Box({midX, midY, midZ}, max)};
  return result;
}
