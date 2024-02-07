//
// Created by jvlk on 7.11.23.
//
#include <gtest/gtest.h>
#include <random>
#include <iostream>

#include "point.h"
#include "randompoint.h"
#include "hastree.h"


TEST(findBellogingIntervals, half) {
    constexpr int from = 0;
    constexpr int to = 1;

    auto p = genPoints<from, to>(1);

    HashOctree tree(p, from, to);

    auto sp = tree.findBellogingIntervals(Point(0, 0, 0), 2);
    EXPECT_EQ(sp, std::make_tuple(0, 0, 0));
    auto sp2 = tree.findBellogingIntervals(Point(1, 1, 1), 2);
    EXPECT_EQ(sp2, std::make_tuple(1, 1, 1));
    auto sp3 = tree.findBellogingIntervals(Point(0.5, 0.5, 0.5), 2);
    EXPECT_EQ(sp3, std::make_tuple(1, 1, 1));
    auto sp4 = tree.findBellogingIntervals(Point(0.5, 0.5, 0.5), 1);
}

TEST(findBellogingIntervals, quad) {
    constexpr int from = 0;
    constexpr int to = 1;

    auto p = genPoints<from, to>(1);

    HashOctree tree(p, from, to);

    auto sp = tree.findBellogingIntervals(Point(0, 0, 0), 4);
    EXPECT_EQ(sp, std::make_tuple(0, 0, 0));
    auto sp2 = tree.findBellogingIntervals(Point(1, 1, 1), 4);
    EXPECT_EQ(sp2, std::make_tuple(3,3,3));
    auto sp3 = tree.findBellogingIntervals(Point(0.5, 0.5, 0.5), 4);
    EXPECT_EQ(sp3, std::make_tuple(2,2,2));
    auto sp4 = tree.findBellogingIntervals(Point(0.25, 0.25, 0.25), 4);
    EXPECT_EQ(sp4, std::make_tuple(1,1,1));
    auto sp5 = tree.findBellogingIntervals(Point(0.75, 0.75, 0.75), 4);
    EXPECT_EQ(sp5, std::make_tuple(3,3,3));
}

TEST(findBellogingIntervals, oct) {
    constexpr int from = 0;
    constexpr int to = 1;

    auto p = genPoints<from, to>(1);

    HashOctree tree(p, from, to);

    auto sp = tree.findBellogingIntervals(Point(0, 0, 0), 8);
    EXPECT_EQ(sp, std::make_tuple(0, 0, 0));
    auto sp2 = tree.findBellogingIntervals(Point(1, 1, 1), 8);
    EXPECT_EQ(sp2, std::make_tuple(7,7,7));
    auto sp3 = tree.findBellogingIntervals(Point(0.5, 0.5, 0.5), 8);
    EXPECT_EQ(sp3, std::make_tuple(4,4,4));
    auto sp4 = tree.findBellogingIntervals(Point(0.25, 0.25, 0.25), 8);
    EXPECT_EQ(sp4, std::make_tuple(2,2,2));
    auto sp5 = tree.findBellogingIntervals(Point(0.75, 0.75, 0.75), 8);
    EXPECT_EQ(sp5, std::make_tuple(6,6,6));
}

TEST(findBellogingIntervalsByLevel, one) {
    constexpr int from = 0;
    constexpr int to = 1;

    auto p = genPoints<from, to>(1);

    HashOctree tree(p, from, to);

    auto sp = tree.findBellogingIntervals(Point(0, 0, 0), 1);
    EXPECT_EQ(sp, std::make_tuple(0, 0, 0));
    auto sp2 = tree.findBellogingIntervals(Point(1, 1, 1), 1);
    EXPECT_EQ(sp2, std::make_tuple(1, 1, 1));

}
