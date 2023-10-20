//
// Created by jvlk on 27.9.23.
//
#include "point.h"
#include <iostream>
#include <format>


Point::Point(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
};

void Point::repr() {
    std::cout << std::format("({},{},{}) \n", x, y, z);
}

double Point::distance(Point p) {
    return std::sqrt(
            std::pow(x - p.x, 2) +
            std::pow(y - p.y, 2) +
            std::pow(z - p.z, 2));

}




