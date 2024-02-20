//
// Created by jvlk on 7.11.23.
//
#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"

class TestFindBelogingInterval :
        public testing::TestWithParam<float> {
};

INSTANTIATE_TEST_SUITE_P(constMultiply,
                         TestFindBelogingInterval,
                         ::testing::Values(1, 1.235, 10.358, 125.7, 0.233, 100, -5
                                 // ,1000000
                                 //, 10000000, 100000000
                         )
);


TEST_P(TestFindBelogingInterval, test0ToN) {
    float param = GetParam();
    float from = 0 * param;
    float to = 1 * param;

    std::vector<Point> p = {Point(0, 0, 0)};

    HashOctree tree(p, from, to); // need to build tree


    Point testedPoint{0.1, 1, 0.89};
    testedPoint = testedPoint * param;

    std::cout << "Tested point: " << testedPoint.operator std::string() << std::endl;
    EXPECT_EQ(std::make_tuple(0, 0, 0), tree.findBellogingIntervalsByLevel(testedPoint, 0));
    EXPECT_EQ(std::make_tuple(0, 1, 1), tree.findBellogingIntervalsByLevel(testedPoint, 1));
    EXPECT_EQ(std::make_tuple(0, 3, 3), tree.findBellogingIntervalsByLevel(testedPoint, 2));
    EXPECT_EQ(std::make_tuple(0, 7, 7), tree.findBellogingIntervalsByLevel(testedPoint, 3));
    EXPECT_EQ(std::make_tuple(1, 15, 14), tree.findBellogingIntervalsByLevel(testedPoint, 4));
    EXPECT_EQ(std::make_tuple(3, 31, 28), tree.findBellogingIntervalsByLevel(testedPoint, 5));
}

TEST_P(TestFindBelogingInterval, testNToN) {
    float param = GetParam();
    float from = 1 * param;
    float to = 2 * param;

    std::vector<Point> p = {Point(from, from, from)};

    HashOctree tree(p, from, to); // need to build tree


    Point testedPoint{1.1, 2, 1.89};
    testedPoint = testedPoint * param;

    std::cout << "Tested point: " << testedPoint.operator std::string() << std::endl;
    EXPECT_EQ(std::make_tuple(0, 0, 0), tree.findBellogingIntervalsByLevel(testedPoint, 0));
    EXPECT_EQ(std::make_tuple(0, 1, 1), tree.findBellogingIntervalsByLevel(testedPoint, 1));
    EXPECT_EQ(std::make_tuple(0, 3, 3), tree.findBellogingIntervalsByLevel(testedPoint, 2));
    EXPECT_EQ(std::make_tuple(0, 7, 7), tree.findBellogingIntervalsByLevel(testedPoint, 3));
    EXPECT_EQ(std::make_tuple(1, 15, 14), tree.findBellogingIntervalsByLevel(testedPoint, 4));
    EXPECT_EQ(std::make_tuple(3, 31, 28), tree.findBellogingIntervalsByLevel(testedPoint, 5));
}


TEST_P(TestFindBelogingInterval, testMinusNToN) {
    float param = GetParam();
    float from = -1 * param;
    float to = 0 * param;

    std::vector<Point> p = {Point(from, from, from)};

    HashOctree tree(p, from, to); // need to build tree


    Point testedPoint{-1+0.1, -1+0.99999, -1+0.89}; // tested point
    testedPoint = testedPoint * param;

    std::cout << "Tested point: " << testedPoint.operator std::string() << std::endl;
    EXPECT_EQ(std::make_tuple(0, 0, 0), tree.findBellogingIntervalsByLevel(testedPoint, 0));
    EXPECT_EQ(std::make_tuple(0, 1, 1), tree.findBellogingIntervalsByLevel(testedPoint, 1));
    EXPECT_EQ(std::make_tuple(0, 3, 3), tree.findBellogingIntervalsByLevel(testedPoint, 2));
    EXPECT_EQ(std::make_tuple(0, 7, 7), tree.findBellogingIntervalsByLevel(testedPoint, 3));
    EXPECT_EQ(std::make_tuple(1, 15, 14), tree.findBellogingIntervalsByLevel(testedPoint, 4));
    EXPECT_EQ(std::make_tuple(3, 31, 28), tree.findBellogingIntervalsByLevel(testedPoint, 5));
}