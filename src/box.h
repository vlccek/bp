//
// Created by jvlk on 18.10.23.
//

#ifndef BP_BOX_H
#define BP_BOX_H

#include <vector>
#include <array>
#include <tuple>
#include "voro++.hh"

#include "point.h"


class Box {
public:
    Point min;
    Point max;

    Box(Point &min, Point &max);

    Box(Point min, Point max);

    inline std::array<Point, 8> allVertex() {
        std::array<Point, 8> a = {
                min,
                {min.x, max.y, min.z},
                {min.x, max.y, max.z},
                {min.x, min.y, max.z},
                max,
                {max.x, min.y, max.z},
                {max.x, max.y, min.z},
                {max.x, min.y, min.z},
        };
        return a;
    }


    std::vector<Box> splitBoxBy8();


    Box(double xa, double ya, double za, double xb, double yb, double zb);

    inline bool isInside(Point point) const {
        if (point >= min && point <= max) {
            return true;
        }
        return false;
    }

    inline bool isInside(std::array<Point, 8> &p) const {
        for (auto i: p) {
            if (isInside(i))
                return true;
        }
        return false;
    }

    inline bool isInside(Box &b) const {
        auto vertex_of_box = b.allVertex();
        return isInside(vertex_of_box);
    }

    bool isInside(const std::vector<Point> *vertex);

    inline Point center() const {
        Point edge = max - min;
        return min + (edge / 2);
    }

    auto operator==(const Box &b) const {
        return (min == b.min && max == b.max);
    }
    explicit operator std::string () const {
        return std::format("{} {} {} x {} {} {} ", min.x, min.y, min.z, max.x, max.y, max.z);
    }



    inline bool intersectionWithBox(Box &b) {
        auto vertex = allVertex();
        for (auto i: vertex) {
            if (i >= min && i <= max) {
                return true;
            }
        }
        return false;
    }

    inline Point &futherPoint(Point &d) {
        return min;
    }
};


std::vector<Box> splitboxby8(const Point &minP, const Point &maxP);

class BoudingBox : public Box {

    public:
        BoudingBox() : Box({0, 0, 0}, {0, 0, 0}) {};

        explicit BoudingBox(std::vector<double> &d);

        explicit BoudingBox(std::vector<Point> &vertex);
    };

#endif //BP_BOX_H
