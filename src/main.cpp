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

    for (int i = 0; i < 1024; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }


    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});


    auto point = Point(0, 0, 0.99);
    Point pica(0, 0, 0);
    pica = tree.nn(point);
    auto pica2 = tree.nn(p[1]);
    auto pica3 = tree.nn(p[2]);

    std::cout << std::format("({},{},{})", pica.x, pica.y, pica.z) << std::endl;

    return 0;
}
