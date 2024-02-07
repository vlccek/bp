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
    explicit RandomPoint(double (*random)());
};

template<int FROM, int TO>
static double genNumber() {
    static std::default_random_engine e1(1);
    static std::uniform_real_distribution<double> uniform_dist(FROM, TO);

    return (double) uniform_dist(e1);
}

template<int FROM, int TO>
static std::vector<Point> genPoints(int pCount) {
    std::vector<Point> points;
    for (int i = 0; i < pCount; i++) {
        points.push_back(RandomPoint(&genNumber<FROM, TO>));
    }
    return points;
}

#endif // BP_RANDOMPOINT_H
