#include <gtest/gtest.h>
#include "gjk.h"
#include "box.h"
#include "polyhedron.h"



TEST(GJKTest, Collision3) {
    Box box(Point(0, 0, 0), Point(1, 1, 1));
    Point polyhedronCenter(0.5, 0.5, 0.5);
// 16 site polyhedron
    std::vector<Point> vertexPoint = {
            Point( 0,  0,  1),
            Point( 0,  0, -1),
            Point( 1,  0,  0),
            Point(-1,  0,  0),
            Point( 0,  1,  0),
            Point( 0, -1,  0)
    };
    Polyhedron polyhedron(vertexPoint, polyhedronCenter);
    EXPECT_TRUE(gjk(box, &polyhedron));
}

TEST(GJKTest, Collision4) {
    Point polyhedronCenter(0.5, 0.5, 0.5);

    Box box(Point(1.3162666796973455, 3.411004642968191,3.0178150114191657), Point(9.26479836817002, 3.419785170965907,12.008331646476094));
    std::vector<Point> vertexPoint = {
            Point(8.608756337709345, 3.0477570199787216, 6.085295554612985),
            Point(2.7943383504344634, 1.5572035661821793, 1.2628284470310935),
            Point(8.832179012531046, 9.492654667944363, 8.573738913116017),
            Point(5.2487244607999894, 0.36661722670617736, 7.222089810631214),
            Point(6.275446997451573, 8.708894171755038, 4.61376811212925),
            Point(1.0468560261732618, 0.5851743549638644, 8.481256243547671),
    };
    Polyhedron polyhedron(vertexPoint, polyhedronCenter);
    EXPECT_TRUE(gjk(box, &polyhedron));
}
TEST(GJKTest, NotCollision2) {
    Point polyhedronCenter(0.5, 0.5, 0.5);

    Box box(Point(6.668878407862166, 2.0754798452329535,8.956967117095289), Point(12.540665797227728, 6.050315493143384,16.573011654199057));
    std::vector<Point> vertexPoint = {
        Point(1.4485060479530565, 7.369402963047122, 1.8449674962624418),
        Point(0.2887043960086588, 0.3541410116083854, 2.4655339307779145),
        Point(2.1986251930220213, 8.88025920141035, 3.6470330964356377),
        Point(3.592635525338336, 1.115069709096933, 8.530204419221937),
        Point(7.923476001101135, 8.62854506268684, 7.887438327406743),
        Point(7.8072722936162755, 6.026748916955142, 1.5685420374515557),
    };
    Polyhedron polyhedron(vertexPoint, polyhedronCenter);
    EXPECT_FALSE(gjk(box, &polyhedron));
}

