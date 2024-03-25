//
// Created by jvlk on 4.11.23.
//


#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"

inline void PrintTo(const Point &p, ::std::ostream *os) {
    *os << std::format("({},{},{})", p.x, p.y, p.z);
}

constexpr int from = 0;
constexpr int to = 100;


class TestNNalgorithm :
        public testing::TestWithParam<int> {
};

INSTANTIATE_TEST_SUITE_P(numberOfPoints,
                         TestNNalgorithm,
                         ::testing::Values(5,20, 100, 1000, 10000, 100000, 1000000, 10000000,
                                           100000000
                         )
);

Point findNearest(Point &p, std::vector<Point> &points) {
    Point nearest = points[0];
    double distance = p.distance(nearest);
    for (auto i: points) {
        if (p.distance(i) < distance) {
            nearest = i;
            distance = p.distance(i);
        }
    }
    return nearest;
}



/**
 * Test if algorithm can even find some point and not returning (0,0,0) (not found)
 */
TEST_P(TestNNalgorithm, TestIfNNNotreturnNotFound) {


    auto p = genPoints<from, to>(GetParam());

    HashOctree tree(p, from, to);
    Point NotFound = Point(0, 0, 0);

    for (auto i: p) {
        auto findedpoint = tree.nn(i);
        EXPECT_NE(NotFound, findedpoint);
    }

    // tree.printHashTable();

}


TEST_P(TestNNalgorithm, findMorePointInTreeConstant) {

    auto p = genPoints<from, to>(GetParam());
    p.push_back(Point(0, 0, 0.99));
    p.push_back(Point(0.99, 0, 0));
    p.push_back(Point(0.99, 0, 0.99));
    HashOctree tree(p, from, to);


    std::vector<Point> findedvalues
            {
                    Point(0, 0, 0.99),
                    Point(0.99, 0, 0),
                    Point(0.99, 0, 0.99)
            };

    for (int i = 0; i < 3; i++) {
        Point findedpoint = tree.nn(findedvalues[i]);
        if (findedpoint != findedvalues[i])
            std::cout << std::format(
                    "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be 0",
                    findedvalues[i].x, findedvalues[i].y, findedvalues[i].z,
                    findedpoint.x, findedpoint.y, findedpoint.z,
                    findedpoint.distance(findedvalues[i]))
                      << std::endl;

        EXPECT_EQ(findedpoint, findedvalues[i]);
    }
}


TEST_P(TestNNalgorithm, findMorePointInTreeRandom) {

    auto p = genPoints<from, to>(GetParam());;
    HashOctree tree(p, from, to);

    std::vector<Point> testedPoint{
            Point(0, 0, 0.99),
            Point(0.99, 0, 0),
            Point(0.99, 0, 0.99)
    };

    std::vector<Point> corespondingclosespoint;
    for (auto i: testedPoint)
        corespondingclosespoint.push_back(tree.findClosesPointInNode(i, tree.getRoot()));

    for (int i = 0; i < 3; i++) {
        auto findedpoint = tree.nn(testedPoint[i]);

        if (findedpoint != corespondingclosespoint[i])
            std::cout << std::format(
                    "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be {}",
                    corespondingclosespoint[i].x, corespondingclosespoint[i].y, corespondingclosespoint[i].z,
                    findedpoint.x, findedpoint.y, findedpoint.z,
                    findedpoint.distance(testedPoint[i]),
                    testedPoint[i].distance(corespondingclosespoint[i]))
                      << std::endl;
        EXPECT_EQ(findedpoint, corespondingclosespoint[i]);
    }

}


TEST_P(TestNNalgorithm, backCheckAllInseretedPoint) {
    auto p = genPoints<from, to>(GetParam());

    HashOctree tree(p, from, to);

    for (auto i: p) {
        auto findedpoint = tree.nn(i);
        if (findedpoint != i) {
            std::cout << std::format(
                    "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be 0",
                    i.x, i.y, i.z,
                    findedpoint.x, findedpoint.y, findedpoint.z,
                    findedpoint.distance(i))
                      << std::endl;
        }
        EXPECT_EQ(findedpoint, i);
    }
    // tree.printHashTable();
}


TEST_P(TestNNalgorithm, basicCheckOneInN) {

    auto p = genPoints<from, to>(GetParam());

    HashOctree tree(p, from, to);

    Point i = Point(0.8885722140672627, 0.7254119986607919, 0.6295434181240079);

    Point nearest = findNearest(i, p);
    auto findedpoint = tree.nn(i);
    if (findedpoint != nearest)
        std::cout << std::format(
                "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be {}",
                nearest.x, nearest.y, nearest.z,
                findedpoint.x, findedpoint.y, findedpoint.z,
                findedpoint.distance(i),
                i.distance(nearest))
                  << std::endl;
    EXPECT_EQ(findedpoint, nearest);

    // tree.printHashTable();

}


TEST_F(TestNNalgorithm, basicCheckOneIn100) {

    auto p = genPoints<from, to>(20);
    p.push_back(Point(0.007698186061599179, 0.5297001931410572, 0.03457211046484744));

    HashOctree tree(p, from, to);

    // tree.printHashTable();
    Point i = Point(0.007698186061599179, 0.5297001931410572, 0.03457211046484744);
    auto findedpoint = tree.nn(i);
    if (findedpoint != i)
        std::cout << std::format(
                "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be 0",
                i.x, i.y, i.z,
                findedpoint.x, findedpoint.y, findedpoint.z,
                findedpoint.distance(i))
                  << std::endl;
    EXPECT_EQ(findedpoint, i);
}



class TestNNalgorithmNoParam :
        public testing::Test {
};


// Implementation limitation. Not working, infinite recursion
TEST_F(TestNNalgorithmNoParam, gridTest3x3) {

    constexpr int grid = 2;

    std::vector<Point> p = {
            {0.75, 0.75, .75},
            {.75,  .75,  .25},
            {.75,  .25,  .75},
            {.75,  .25,  .25},
            {.25,  .75,  .75},
            {.25,  .75,  0.25},
            {.25,  .25,  .75},
            {.25,  .25,  .25}
    };
    for (auto &i: p) {
        i = i * to;
    }


    HashOctree tree(p, from, to);

    auto po = Point(0.1, 0.1, 0.1);
    auto findedPoint = tree.nn(po);
}


TEST_F(TestNNalgorithmNoParam, gridTest) {
    constexpr int grid = 2;

    std::vector<Point> p;


    for (int i = 0; i < grid; i++) {
        for (int l = 0; l < grid; l++) {
            for (int k = 0; k < grid; k++) {
                p.push_back(Point(i / (double) grid, l / (double) grid, k / (double) grid) * to);
            }
        }
    }

    HashOctree tree(p, from, to);

    auto po = Point(0.1, 0.1, 0.1);
    auto findedPoint = tree.nn(po);
}

