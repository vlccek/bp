#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"




class TestingTreeBuilding :
        public testing::TestWithParam<int> {
};

INSTANTIATE_TEST_SUITE_P(numberOfPoints,
                         TestingTreeBuilding,
                         ::testing::Values(20, 100, 1000, 10000
                                 ,100000, 1000000
                                 //, 10000000, 100000000
                         )
);




TEST_P(TestingTreeBuilding, ContainLeafnodesAllPoints){
    constexpr float from = 0;
    constexpr float to = 1;

    auto p = genPoints<from, to>(GetParam());
    std::vector<Point> points;

    HashOctree tree(p, from, to);

    auto leafsNodes = tree.getLeafs();
    for (auto i: leafsNodes) {
        for (auto j: i->voronoiCells) {
            if (std::find(points.begin(), points.end(), j->p) == points.end()) {
                points.push_back(j->p);
            }
        }
    }

    EXPECT_EQ(points.size(), p.size());
}