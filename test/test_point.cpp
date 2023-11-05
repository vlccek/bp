// write test for Point class
#include <gtest/gtest.h>
#include "point.h"

TEST(Point, init) {
    Point p = Point(1, 2, 3);
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 2);
    EXPECT_EQ(p.z, 3);
}

TEST(Point, minus) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 3);
    Point p3 = p - p2;
    EXPECT_EQ(p3.x, 0);
    EXPECT_EQ(p3.y, 0);
    EXPECT_EQ(p3.z, 0);
}

TEST(Point, minus_u) {
    Point p = Point(1, 2, 3);
    Point p3 = -p;
    EXPECT_EQ(p3.x, -1);
    EXPECT_EQ(p3.y, -2);
    EXPECT_EQ(p3.z, -3);
}

TEST(Point, eq) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 3);
    EXPECT_EQ(p, p2);
}

TEST(Point, neq) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    EXPECT_NE(p, p2);
}

TEST(Point, gt) {
    Point p = Point(10, 20, 30);
    Point p2 = Point(1, 2, 2);
    EXPECT_GT(p, p2);
}

TEST(Point, lt) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(10, 20, 40);
    EXPECT_LT(p, p2);
}

TEST(Point, distance) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    EXPECT_EQ(p.distance(p2), 1);
}

TEST(Point, dot) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    EXPECT_EQ(p * p2, 17);

    p = -p;
    EXPECT_EQ(p * p2, -17);

}

TEST(Point, cross3) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    Point p3 = cross(p, p2);
    Point p4 = cross(p2, p);
    EXPECT_EQ(p3, -p4);
}

TEST(Point, cross4) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    Point p3 = cross(p, p2);
    Point p4 = cross(p2, p);
    EXPECT_EQ(p3, -p4);
}

TEST(Point, cross5) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    Point p3 = cross(p, p2);
    Point p4 = cross(p2, p);
    EXPECT_EQ(p3, -p4);
}

TEST(Point, cross6) {
    Point p = Point(1, 2, 3);
    Point p2 = Point(1, 2, 4);
    Point p3 = cross(p, p2);
    Point p4 = cross(p2, p);
    EXPECT_EQ(p3, -p4);
}

