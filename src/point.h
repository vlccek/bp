
#ifndef BP_POINT_H
#define BP_POINT_H

#include <compare>
#include <cmath>
#include <string>
#include <format>

class Point {
public:
    double x;
    double y;
    double z;

    Point(double x, double y, double z);


    void repr();


    auto operator>(const Point &p) const {
        return (x > p.x && y > p.y && z > p.z);
    }

    auto operator<(const Point &p) const {
        return ((x < p.x && y < p.y && z < p.z));
    }

    auto operator>=(const Point &p) const {
        return (x >= p.x && y >= p.y && z >= p.z);
    }

    auto operator<=(const Point &p) const {
        return ((x <= p.x && y <= p.y && z <= p.z));
    }

    auto operator-(const Point &p) const {
        return Point{
                x - p.x,
                y - p.y,
                z - p.z
        };

    }

    auto operator+(const Point &p) const {
        return Point{
                x + p.x,
                y + p.y,
                z + p.z
        };

    }

    auto operator/(const double &p) const {
        return Point{
                x / p,
                y / p,
                z / p
        };
    }

    auto operator*(const long &p) const {
        return Point{
                x * p,
                y * p,
                z * p
        };
    }

    operator std::string() const {
        return std::format("({:2f},{:2f},{:2f})", x, y, z);
    }


/**
 * dot
 * @param p
 * @return dot
 */
    auto operator*(const Point &p) const {
        return (x * p.x) +
               (y * p.y) +
               (z * p.z);
    }

    auto operator-() const {
        return Point{
                -x,
                -y,
                -z
        };
    }


    double distance(Point p);


    auto operator==(const Point &p) const {
        return ((x == p.x && y == p.y && z == p.z));
    }

};


inline Point cross(const Point &p1, const Point &p2) {
    return {
            (p1.y * p2.z) - (p1.z * p2.y),
            (p1.z * p2.x) - (p1.x * p2.z),
            (p1.x * p2.y) - (p1.y * p2.x)
    };
}


#endif