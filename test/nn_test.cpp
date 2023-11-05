//
// Created by jvlk on 4.11.23.
//


#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"

class FindOnePointInManyNotNotFound :
        public testing::TestWithParam<int> {
    // You can implement all the usual fixture class members here.
    // To access the test parameter, call GetParam() from class
    // TestWithParam<T>.
};
/**
 * Test if algorithm can even find some point and not returning (0,0,0) (not found)
 */
TEST_P(FindOnePointInManyNotNotFound, FindOnePointInManyNotNotFound) {
    int n = GetParam();
    std::vector<Point> p;

    for (int i = 0; i < n; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }

    auto point = Point(0, 0, 0.95);
    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});
    Point pica = tree.nn(point);
    EXPECT_NE(pica, Point(0, 0, 0));
}

INSTANTIATE_TEST_SUITE_P(PositiveNumber,
                         FindOnePointInManyNotNotFound,
                         testing::Range(10, 10000, 1000)
);

TEST(FindManyPointsInManyPoints, test) {
    std::vector<Point> p
            {
                    Point(0, 0, 0.99),
                    Point(0.99, 0, 0),
                    Point(0.99, 0, 0.99)
            };

    for (int i = 0; i < 10; i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
    }
    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});


    std::vector<Point> findedvalues
            {
                    Point(0, 0, 0.99),
                    Point(0.99, 0, 0),
                    Point(0.99, 0, 0.99)
            };

    for (int i = 0; i < 3; i++) {
        Point findedpoint = tree.nn(findedvalues[i]);
        EXPECT_EQ(findedpoint, findedvalues[i]);
    }
}
