#include "point.h"
#include "randompoint.h"

#include "hastree.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>

using namespace voro;


int main() {
    std::vector<Point> p;

    for (int i = 0; i < 1000; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1000>));
        // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() - 1].y, p[p.size() - 1].z) << std::endl;
    }

    HashOctree tree(p, {0, 0, 0}, {1000, 1000, 1000});


    return 0;
}
