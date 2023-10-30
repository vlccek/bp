

#include "gjk.h"


bool gjk(Box &b, Polyhedron *p) {
    Point direction = b.center() - p->p; // compute the base direction as difference of centers of box and polyhedron

    std::vector<Point> simplex;
    Point a = support(b, p, direction);
    simplex.insert(simplex.begin(), a);
    direction = -a;

    while (true) {
        a = support(b, p, direction);
        if ((a * direction) < 0) {
            auto ahoj = &p->vertexPoints;

            if (b.isInside(ahoj)) {
                std::cout << "is inside" << std::endl;

            }
            std::cout << std::format("false box: {} \n polyhedron: {}  \n", std::string(b), std::string(*p));

            return false;
        }
        simplex.insert(simplex.begin(), a);

        if (nextSimplex(simplex, direction)) {
            std::cout << std::format("True box: {} \n polyhedron: {}  \n", std::string(b), std::string(*p));

            return true;
        }

    }
    return false;


}

bool samedirection(const Point &direction, const Point &ao) {
    return (direction * ao) > 0;
}


bool line(std::vector<Point> &simplex, Point &direction) {
    Point a = simplex[0];
    Point b = simplex[1];

    Point ab = b - a;
    Point a0 = -a;

    if (samedirection(ab, a0)) {
        direction = cross(cross(ab, a0), ab);
    } else {
        simplex = {a};
        direction = a0;
    }
    return false;
}


bool triangle(std::vector<Point> &simplex, Point &direction) {
    Point a = simplex[0];
    Point b = simplex[1];
    Point c = simplex[2];

    Point ab = b - a;
    Point ac = c - a;
    Point ao = -a;

    Point abc = cross(ab, ac);

    if (samedirection(cross(abc, ac), ao)) {
        if (samedirection(ac, ao)) {
            simplex = {a, c};
            direction = cross(cross(ac, ao), ac);
        } else {
            return line(simplex = {a, b}, direction);
        }
    } else {
        if (samedirection(cross(ab, abc), ao)) {
            return line(simplex = {a, b}, direction);
        } else {
            if (samedirection(abc, ao)) {
                direction = abc;
            } else {
                simplex = {a, c, b};
                direction = -abc;
            }
        }
    }

    return false;
}

bool tetrahedron(std::vector<Point> &simplex, Point &direction) {
    Point a = simplex[0];
    Point b = simplex[1];
    Point c = simplex[2];
    Point d = simplex[3];

    Point ab = b - a;
    Point ac = c - a;
    Point ad = d - a;
    Point ao = -a;

    Point abc = cross(ab, ac);
    Point acd = cross(ac, ad);
    Point adb = cross(ad, ab);

    if (samedirection(abc, ao)) {
        return triangle(simplex = {a, b, c}, direction);
    }

    if (samedirection(acd, ao)) {
        return triangle(simplex = {a, c, d}, direction);
    }

    if (samedirection(adb, ao)) {
        return triangle(simplex = {a, d, b}, direction);
    }

    return true;
}

bool nextSimplex(std::vector<Point> &simplex, Point &direction) {
    switch (simplex.size()) {
        case 2:
            return line(simplex, direction);
        case 3:
            return triangle(simplex, direction);
        case 4:
            return tetrahedron(simplex, direction);
    }
    return false;
}

Point support(Box &b, Polyhedron *p, Point d) {
    auto minusd = -d;
    return b.futherPoint(d) - p->futherPoint(minusd);
}