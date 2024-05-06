/**
* @file randompoint.cpp
* @brief RandomPoint class
* @author Jakub Vlk
 */

#include "randompoint.h"

RandomPoint::RandomPoint(float (*random)())
    : Point(random(), random(), random()) {
  // Empty
}

RandomPointNormal::RandomPointNormal(float (*random)())
    : Point(random(), random(), random(), random(), random(), random()) {

  float len_nor = sqrtf(nor_x * nor_x + nor_y * nor_y + nor_z * nor_z);

  nor_x /= len_nor;

  nor_y /= len_nor;
  nor_z /= len_nor;
}
