//
// Created by jvlk on 27.9.23.
//

#include "randompoint.h"





RandomPoint::RandomPoint(float (*random)()) :
        Point(
                random(),
                random(),
                random()
        ) {
    // Empty
}
