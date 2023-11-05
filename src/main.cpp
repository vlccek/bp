#include "point.h"
#include "randompoint.h"

#include "hastree.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>

using namespace voro;


int main() {
    std::vector<Point> p{
            Point(0, 0, 0.99)
    };

    for (int i = 0; i < 20; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
        // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() - 1].y, p[p.size() - 1].z) << std::endl;
    }


    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});


    auto point = Point(0, 0, 0.95);
    Point pica(0, 0, 0);
    for (int i = 0; i < 1221510; i++)
        pica = tree.nn(point);

    std::cout << std::format("({},{},{})", pica.x, pica.y, pica.z) << std::endl;

    return 0;
}
