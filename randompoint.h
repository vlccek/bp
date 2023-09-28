//
// Created by jvlk on 27.9.23.
//

#include "point.h"

#ifndef BP_RANDOMPOINT_H
#define BP_RANDOMPOINT_H

/**
 * Generate point by generator provided from consturctor
 */
class RandomPoint : public Point {
public:
    explicit RandomPoint(double (*random)());

};

#endif //BP_RANDOMPOINT_H
