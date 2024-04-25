//
// Created by jvlk on 27.9.23.
//

#include "point.h"
#include <random>

#ifndef BP_RANDOMPOINT_H
#define BP_RANDOMPOINT_H

/**
 * Generate point by generator provided from consturctor
 */
class RandomPoint : public Point {
public:
  explicit RandomPoint(float (*random)());
};

template <float FROM, float TO> static float genNumber() {
  static std::default_random_engine e1(1);
  static std::uniform_real_distribution<float> uniform_dist(FROM, TO);

  return static_cast<float>(uniform_dist(e1));
}

template <float FROM, float TO> static float genNumber_cluster() {
  static std::default_random_engine e1(1);

  std::mt19937 gen(e1());
  std::normal_distribution<> d(0, 20);

  return static_cast<float>(d(gen));
}

template <float FROM, float TO> static std::vector<Point> genPoints(int pCount) {
  std::vector<Point> points;
  for (int i = 0; i < pCount; i++) {
    points.push_back(RandomPoint(&genNumber<FROM, TO>));
  }
  return points;
}

template <float FROM, float TO> static std::vector<Point> genPointsCluster(int pCount) {
  std::vector<Point> points;
  for (int i = 0; i < pCount; i++) {
    points.push_back(RandomPoint(&genNumber_cluster<FROM, TO>));
  }
  return points;
}


template <int FROM, int TO> static float genNumber() {
  static std::default_random_engine e1(1);
  static std::uniform_real_distribution<float> uniform_dist(FROM, TO);

  return static_cast<float>(uniform_dist(e1));
}

template <int FROM, int TO> static float genNumber_cluster() {
  static std::default_random_engine e1(1);

  std::mt19937 gen(e1());
  std::normal_distribution<> d(0, 20);

  return static_cast<float>(d(gen));
}

template <int FROM, int TO> static std::vector<Point> genPoints(int pCount) {
  std::vector<Point> points;
  for (int i = 0; i < pCount; i++) {
    points.push_back(RandomPoint(&genNumber<FROM, TO>));
  }
  return points;
}

template <int FROM, int TO> static std::vector<Point> genPointsCluster(int pCount) {
  std::vector<Point> points;
  for (int i = 0; i < pCount; i++) {
    points.push_back(RandomPoint(&genNumber_cluster<FROM, TO>));
  }
  return points;
}


#endif // BP_RANDOMPOINT_H
