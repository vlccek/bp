//
// Created by jvlk on 18.10.23.
//

#ifndef BP_BOX_H
#define BP_BOX_H

#include <vector>
#include <array>
#include <tuple>
#include <format>
#include <iostream>
#include <fstream>
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
                min, // 0,0,0
                {min.x, max.y, min.z},// 0,1,0
                {min.x, max.y, max.z},
                {min.x, min.y, max.z},
                max,
                {max.x, min.y, max.z},
                {max.x, max.y, min.z},
                {max.x, min.y, min.z},
        };
        return a;
    }

    void writeGnuFormat(std::string &filename);

    void writeGnuFormat() {
        std::string filename = "border.gnu";
        writeGnuFormat(filename);
    };

    std::vector<Box> splitBoxBy8();


    Box(float xa, float ya, float za, float xb, float yb, float zb);

    inline bool isInside(Point point) const {
        if (point > min && point < max) {
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

    /**
     * Check if the box is inside another box
     * @param b
     * @return
     */
    inline bool isInside(Box &b) const {
        auto vertex_of_box = b.allVertex();
        return isInside(vertex_of_box);
    }

    template<typename T>
    bool isInside(const std::vector<T> *vertex) const {
        for (auto p: *vertex) {
            if (isInside(p))
                return true;
        }
        return false;

    }


    inline Point center() const {
        Point edge = max - min;
        return min + (edge / 2);
    }

    auto operator==(const Box &b) const {
        return (min == b.min && max == b.max);
    }

    explicit operator std::string() const {
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

    inline bool isTooSmall() const {
        return (max - min).x < std::numeric_limits<float>::epsilon() ||
               (max - min).y < std::numeric_limits<float>::epsilon() ||
               (max - min).z < std::numeric_limits<float>::epsilon();
    }
};


std::vector<Box> splitboxby8(const Point &minP, const Point &maxP);

class BoudingBox : public Box {

public:
    BoudingBox() : Box({0, 0, 0}, {0, 0, 0}) {};

    explicit BoudingBox(std::vector<float> &d);

    explicit BoudingBox(std::vector<Point> &vertex);
};

#endif //BP_BOX_H
