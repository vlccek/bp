// test for testing box class
#include  <gtest/gtest.h>
#include <format>
#include "box.h"

TEST(Box, init) {
    Box b = Box({1, 2, 3}, {4, 5, 6});
    // box is made by two points min and max
    EXPECT_EQ(b.min.x, 1);
    EXPECT_EQ(b.min.y, 2);
    EXPECT_EQ(b.min.z, 3);
    EXPECT_EQ(b.max.x, 4);
    EXPECT_EQ(b.max.y, 5);
    EXPECT_EQ(b.max.z, 6);
}

// test if all vertex works
TEST(Box, allVertex) {
    Box b = Box({1, 2, 3}, {4, 5, 6});
    auto a = b.allVertex();
    EXPECT_EQ(a[0].x, 1);
    EXPECT_EQ(a[0].y, 2);
    EXPECT_EQ(a[0].z, 3);
    EXPECT_EQ(a[1].x, 1);
    EXPECT_EQ(a[1].y, 5);
    EXPECT_EQ(a[1].z, 3);
    EXPECT_EQ(a[2].x, 1);
    EXPECT_EQ(a[2].y, 5);
    EXPECT_EQ(a[2].z, 6);
    EXPECT_EQ(a[3].x, 1);
    EXPECT_EQ(a[3].y, 2);
    EXPECT_EQ(a[3].z, 6);
    EXPECT_EQ(a[4].x, 4);
    EXPECT_EQ(a[4].y, 5);
    EXPECT_EQ(a[4].z, 6);
    EXPECT_EQ(a[5].x, 4);
    EXPECT_EQ(a[5].y, 2);
    EXPECT_EQ(a[5].z, 6);
    EXPECT_EQ(a[6].x, 4);
    EXPECT_EQ(a[6].y, 5);
    EXPECT_EQ(a[6].z, 3);
    EXPECT_EQ(a[7].x, 4);
    EXPECT_EQ(a[7].y, 2);
    EXPECT_EQ(a[7].z, 3);

    Box b2 = Box({-1, -2, -3}, {4, 5, 6});
    auto a2 = b2.allVertex();
    EXPECT_EQ(a2[0].x, -1);
    EXPECT_EQ(a2[0].y, -2);
    EXPECT_EQ(a2[0].z, -3);
    EXPECT_EQ(a2[1].x, -1);
    EXPECT_EQ(a2[1].y, 5);
    EXPECT_EQ(a2[1].z, -3);
    EXPECT_EQ(a2[2].x, -1);
    EXPECT_EQ(a2[2].y, 5);
    EXPECT_EQ(a2[2].z, 6);
    EXPECT_EQ(a2[3].x, -1);
    EXPECT_EQ(a2[3].y, -2);
    EXPECT_EQ(a2[3].z, 6);
    EXPECT_EQ(a2[4].x, 4);
    EXPECT_EQ(a2[4].y, 5);
    EXPECT_EQ(a2[4].z, 6);
    EXPECT_EQ(a2[5].x, 4);
    EXPECT_EQ(a2[5].y, -2);
    EXPECT_EQ(a2[5].z, 6);
    EXPECT_EQ(a2[6].x, 4);
    EXPECT_EQ(a2[6].y, 5);
    EXPECT_EQ(a2[6].z, -3);
    EXPECT_EQ(a2[7].x, 4);
    EXPECT_EQ(a2[7].y, -2);
    EXPECT_EQ(a2[7].z, -3);
}

class boxSplitTest :
        public testing::TestWithParam<std::pair<double, double>> {
    // You can implement all the usual fixture class members here.
    // To access the test parameter, call GetParam() from class
    // TestWithParam<T>.
};

TEST_P(boxSplitTest, postiveNumber) {

    std::cout << std::format("Testing splitBoxBy8 with min: {} and max: {}", GetParam().first, GetParam().second);
    double min = GetParam().first;
    double max = GetParam().second;
    double half = (max - min) / 2;
    Box b = Box({min, min, min}, {max, max, max});
    auto a = b.splitBoxBy8();
    std::vector<Box> splitedbox = {
            Box({min, min, min}, {min + half, min + half, min + half}),
            Box({min + half, min, min}, {max, min + half, min + half}),
    };
    for (auto i: a) {
        for (const auto &it: splitedbox) {
            if (i == it) {
                splitedbox.erase(std::remove(splitedbox.begin(), splitedbox.end(), it), splitedbox.end());
                continue;
            }
        }

    }
    for (auto i: splitedbox) {
        std::cout << std::format("{} {} {} x {} {} {} ", i.min.x, i.min.y, i.min.z, i.max.x, i.max.y, i.max.z);
    }

    EXPECT_EQ(splitedbox.size(), 0);
}

TEST_P(boxSplitTest, negativeNumber) {
    double min = GetParam().first * (-1);
    double max = GetParam().second * (-1);
    std::cout << std::format("Testing splitBoxBy8 with min: {} and max: {}", min, max);

    double half = (max - min) / 2;
    Box b = Box({min, min, min}, {max, max, max});
    auto a = b.splitBoxBy8();
    std::vector<Box> splitedbox = {
            Box({min, min, min}, {min + half, min + half, min + half}),
            Box({min + half, min, min}, {max, min + half, min + half}),
    };
    for (auto i: a) {
        for (const auto &it: splitedbox) {
            if (i == it) {
                splitedbox.erase(std::remove(splitedbox.begin(), splitedbox.end(), it), splitedbox.end());
                continue;
            }
        }

    }
    for (auto i: splitedbox) {
        std::cout << std::format("{} {} {} x {} {} {} ", i.min.x, i.min.y, i.min.z, i.max.x, i.max.y, i.max.z);
    }
    EXPECT_EQ(splitedbox.size(), 0);
}

INSTANTIATE_TEST_SUITE_P(PositiveNumber,
                         boxSplitTest,
                         testing::Values(
                                 std::pair<double, double>{0, 1},
                                 std::pair<double, double>{0, 2},
                                 std::pair<double, double>{10, 20},
                                 std::pair<double, double>{100, 200},
                                 std::pair<double, double>{111.50, 200.5}

                         )
);

TEST(Box, center) {
    Box b = Box({0, 0, 0}, {1, 1, 1});
    auto c = b.center();
    EXPECT_EQ(c.x, 0.5);
    EXPECT_EQ(c.y, 0.5);
    EXPECT_EQ(c.z, 0.5);

    Box b2 = Box({-1, -1, -1}, {1, 1, 1});
    auto c2 = b2.center();
    EXPECT_EQ(c2.x, 0);
    EXPECT_EQ(c2.y, 0);
    EXPECT_EQ(c2.z, 0);
}

TEST(Box, isinsidePoint) {
    Box b = Box({0, 0, 0}, {1, 1, 1});
    Point p = Point(0.5, 0.5, 0.5);
    EXPECT_TRUE(b.isInside(p));
    Point p2 = Point(1.5, 0.5, 0.5);
    EXPECT_FALSE(b.isInside(p2));
    Point p3 = Point(0.5, 0.5, 0.5);
    EXPECT_TRUE(b.isInside(p3));
}

TEST(Box, inIsideBox) {
    Box b = Box({0, 0, 0}, {1, 1, 1});
    Box b2 = Box({0.5, 0.5, 0.5}, {1, 1, 1});
    auto a = b2.allVertex();
    EXPECT_TRUE(b.isInside(b));
    EXPECT_TRUE(b.isInside(a));

    Box b3 = Box({0, 0, 0}, {1, 1, 1});
    auto a1 = b2.allVertex();
    EXPECT_TRUE(b.isInside(b3));
    EXPECT_TRUE(b.isInside(a1));

    Box b4 = Box({1, 1, 0.9}, {10, 10, 10});
    auto a2 = b2.allVertex();
    EXPECT_TRUE(b.isInside(b4));
    EXPECT_TRUE(b.isInside(a2));

}

TEST(Box, isInsideVoronoi) {
    Box b = Box({0, 0, 0}, {1, 1, 1});
    std::vector<Point> vorocell = {
            Point(1, 1, 1), Point(0, 50, 4), Point(0, 0, 0), Point(100, -5, 10)
    };
    EXPECT_TRUE(b.isInside(&vorocell));
}

TEST(Box, futherpoint) {
    Box b = Box({0, 0, 0}, {1, 1, 1});
    auto fp = b.futherPoint(b.max);
    EXPECT_EQ(fp, b.min);
}


