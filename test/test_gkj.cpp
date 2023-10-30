#include <gtest/gtest.h>
#include "gjk.h"
#include "box.h"
#include "polyhedron.h"// Importujte vaši implementaci GJK algoritmu

TEST(GJKTest, NoCollision) {
    // Vytvoření boxu a polyhedronu tak, aby nekolidovaly
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Point polyhedronpolyhedronCenter(3, 3, 3);
    std::vector<Point> vertexPoints = {Point(2, 2, 2), Point(4, 4, 4), Point(5, 5, 5), Point(6, 6, 6), Point(7, 7, 7)};
    Polyhedron polyhedron(vertexPoints, polyhedronpolyhedronCenter);

    bool result = gjk(box, &polyhedron);

    EXPECT_FALSE(result);
}

TEST(GJKTest, NoCollision2) {
    // Vytvoření boxu a polyhedronu tak, aby nekolidovaly
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Point polyhedronpolyhedronCenter(3, 3, 3);
// 16 site polyhedron
    std::vector<Point> vertexPoints = {Point(2, 2, 2), Point(4, 4, 4), Point(5, 5, 5), Point(6, 6, 6), Point(7, 7, 7),
                                       Point(8, 8, 8), Point(9, 9, 9), Point(10, 10, 10), Point(11, 11, 11),
                                       Point(12, 12, 12), Point(13, 13, 13), Point(14, 14, 14), Point(15, 15, 15),
                                       Point(16, 16, 16), Point(17, 17, 17), Point(18, 18, 18)};
    Polyhedron polyhedron(vertexPoints, polyhedronpolyhedronCenter);

    bool result = gjk(box, &polyhedron);

    EXPECT_FALSE(result);
}

TEST(GJKTest, NoCollision3) {
    // Vytvoření boxu a polyhedronu tak, aby nekolidovaly
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Point polyhedronpolyhedronCenter(3, 3, 3);
    // 10 site polyhedron
    std::vector<Point> vertexPoints = {Point(2, 2, 2), Point(4, 4, 4), Point(5, 5, 5), Point(6, 6, 6), Point(7, 7, 7),
                                       Point(8, 8, 8), Point(9, 9, 9), Point(10, 10, 10), Point(11, 11, 11),
                                       Point(12, 12, 12)};
    Polyhedron polyhedron(vertexPoints, polyhedronpolyhedronCenter);
    bool result = gjk(box, &polyhedron);

    EXPECT_FALSE(result);
}

TEST(GJKTest, Collision) {
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Box box2(Point(0, 0, 0), Point(2, 2, 2));
    Point polyhedronCenter(0.5, 0.5, 0.5);
    auto myVector = box2.allVertex();
    std::vector<Point> vertexPoint(myVector.begin(), myVector.end());
    Polyhedron polyhedron(vertexPoint, polyhedronCenter);
    auto b = gjk(box, &polyhedron);
    EXPECT_TRUE(b);
}

TEST(GJKTest, Collision2) {
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Point polyhedronCenter(0.5, 0.5, 0.5);
    // 16 site polyhedron
    std::vector<Point> vertexPoint = {
            Point(1, 1, 1),
            Point(1, 1, -1),
            Point(1, -1, 1),
            Point(1, -1, -1),
            Point(-1, 1, 1),
            Point(-1, 1, -1),
            Point(-1, -1, 1),
            Point(-1, -1, -1),
            Point(0, (1 + sqrt(5)) / 2, 1 / 2),
            Point(0, (1 + sqrt(5)) / 2, -1 / 2),
            Point(0, -(1 + sqrt(5)) / 2, 1 / 2),
            Point(0, -(1 + sqrt(5)) / 2, -1 / 2),
            Point(1 / 2, 0, (1 + sqrt(5)) / 2),
            Point(1 / 2, 0, -(1 + sqrt(5)) / 2),
            Point(-1 / 2, 0, (1 + sqrt(5)) / 2),
            Point(-1 / 2, 0, -(1 + sqrt(5)) / 2),
            Point((1 + sqrt(5)) / 2, 1 / 2, 0),
            Point((1 + sqrt(5)) / 2, -1 / 2, 0),
            Point(-(1 + sqrt(5)) / 2, 1 / 2, 0),
            Point(-(1 + sqrt(5)) / 2, -1 / 2, 0)
    };
    Polyhedron polyhedron(vertexPoint, polyhedronCenter);
    EXPECT_TRUE(gjk(box, &polyhedron));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
