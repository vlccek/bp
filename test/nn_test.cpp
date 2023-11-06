//
// Created by jvlk on 4.11.23.
//


#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"

class TreeWithNPoint :
        public testing::TestWithParam<int> {
    // You can implement all the usual fixture class members here.
    // To access the test parameter, call GetParam() from class
    // TestWithParam<T>.
};
/**
 * Test if algorithm can even find some point and not returning (0,0,0) (not found)
 */
TEST_P(TreeWithNPoint, TestIfNNNotreturnNotFound) {
    int n = GetParam();
    std::vector<Point> p;

    for (int i = 0; i < n; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }

    auto point = Point(0, 0, 0.95);
    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});
    Point fp = tree.nn(point);
    EXPECT_NE(fp, Point(0, 0, 0));
}

INSTANTIATE_TEST_SUITE_P(numberOfPoints,
                         TreeWithNPoint,
                         ::testing::Values(100, 1000, 10000)
);


TEST_P(TreeWithNPoint, findMorePointInTreeConstant) {
    std::vector<Point> p
            {
                    Point(0, 0, 0.99),
                    Point(0.99, 0, 0),
                    Point(0.99, 0, 0.99)
            };

    for (int i = 0; i < GetParam(); i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }
    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});


    std::vector<Point> findedvalues
            {
                    Point(0, 0, 0.99),
                    Point(0.99, 0, 0),
                    Point(0.99, 0, 0.99)
            };

    tree.printHashTable();
    for (int i = 0; i < 3; i++) {
        Point findedpoint = tree.nn(findedvalues[i]);
        std::cout << std::format(
                "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be 0",
                findedvalues[i].x, findedvalues[i].y, findedvalues[i].z,
                findedpoint.x, findedpoint.y, findedpoint.z,
                findedpoint.distance(findedvalues[i]))
                  << std::endl;

        EXPECT_DOUBLE_EQ(findedpoint.x, findedvalues[i].x);
        EXPECT_DOUBLE_EQ(findedpoint.y, findedvalues[i].y);
        EXPECT_DOUBLE_EQ(findedpoint.z, findedvalues[i].z);
    }
}


TEST_P(TreeWithNPoint, findMorePointInTreeRandom) {
    std::vector<Point> p;
    for (int i = 0; i < GetParam(); i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }
    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});


    std::vector<Point> testedPoint{
            Point(0, 0, 0.99),
            Point(0.99, 0, 0),
            Point(0.99, 0, 0.99)
    };

    std::vector<Point> corespondingclosespoint;
    for (auto i: testedPoint)
        corespondingclosespoint.push_back(HashOctree::findClosesPointInNode(i, tree.getRoot()));

    for (int i = 0; i < 3; i++) {
        auto findedpoint = tree.nn(testedPoint[i]);

        std::cout << std::format(
                "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be {}",
                corespondingclosespoint[i].x, corespondingclosespoint[i].y, corespondingclosespoint[i].z,
                findedpoint.x, findedpoint.y, findedpoint.z,
                findedpoint.distance(testedPoint[i]),
                testedPoint[i].distance(corespondingclosespoint[i]))
                  << std::endl;
        EXPECT_DOUBLE_EQ(findedpoint.x, corespondingclosespoint[i].x);
        EXPECT_DOUBLE_EQ(findedpoint.y, corespondingclosespoint[i].y);
        EXPECT_DOUBLE_EQ(findedpoint.z, corespondingclosespoint[i].z);
    }

}

TEST_P(TreeWithNPoint, backCheckAllInseretedPoint) {
    std::vector<Point> p;

    for (int i = 0; i < GetParam(); i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }

    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});

    for (auto i: p) {
        auto findedpoint = tree.nn(i);
        if (findedpoint != i)
            std::cout << std::format(
                    "nn Should find: ({},{},{}), But found: ({},{},{}) distance is: {}, real distance should be 0",
                    i.x, i.y, i.z,
                    findedpoint.x, findedpoint.y, findedpoint.z,
                    findedpoint.distance(i))
                      << std::endl;
        EXPECT_DOUBLE_EQ(findedpoint.x, i.x);
        EXPECT_DOUBLE_EQ(findedpoint.y, i.y);
        EXPECT_DOUBLE_EQ(findedpoint.z, i.z);
    }
}



