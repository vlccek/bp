/**
* @file box.h
* @brief Box class
* @author Jakub Vlk
*/

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

/**
 * Box class
 */
class Box {
public:
  /**
   * Min and max point of the box
   */
    Point min;
    Point max;


    std::array<Box,8> *splited = nullptr;

    /**
     * Constructor
     * @param min
     * @param max
     */
    Box(Point &min, Point &max);

    /**
     * Constructor
     * @param min
     * @param max
     */
    Box(Point min, Point max);

    ~Box();


    /**
     * Return all vertex of the box
     * @return
     */
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
/**
 * Split the box by 2 in each dimension
 * @return
 */
    std::array<Box,8> splitBoxBy8();


    Box(float xa, float ya, float za, float xb, float yb, float zb);

    /**
     * Check if the point is inside the box
     * @param point
     * @return
     */
    inline bool isInside(Point &point) const {
        bool bigger = point > min;
        bool smaller = point < max;
        if (bigger && smaller) {
            return true;
        }
        return false;
    }

    /**
     * Check if the box is inside another box
     * @param b
     * @return
     */
    inline bool isInside(std::array<Point, 8> &p) const {
        for (auto &i: p) {
            if (isInside(i))
                return true;
        }
        return false;
    }

        /**
         * Check if one of the points is inside the box
         * @param b
         * @return
         */
    inline bool isInside(std::vector<Point> &p) const {
        for (int i = 0; i < p.size(); ++i) {
            if (isInside(p[i]))
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

        /**
         * Check if the box is inside object
         * @param b
         * @return
         */
    template<typename T>
    bool isInside(const std::vector<T> *vertex) const {
        for (auto p: *vertex) {
            if (isInside(p))
                return true;
        }
        return false;

    }

    /**
     * Compute center of box
     * @return
     */
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


    /**
     * Return the futherst point from the box
     * @param d
     * @return
     */
    inline Point &futherstPoint(Point &d) {

        float maxdist = std::numeric_limits<float>::min();
        Point &mindistPoint = d;
        for (auto &i: allVertex()) {
            float dist = d.distance(i);
            if (dist > maxdist) {
                maxdist = dist;
                mindistPoint = i;
            }
        }
        return mindistPoint;
    }

    /**
     * Tests if the box so small that another split is not possible on float type
     * @return
     */
    inline bool isTooSmall() const {
        return (max - min).x < std::numeric_limits<float>::epsilon() ||
               (max - min).y < std::numeric_limits<float>::epsilon() ||
               (max - min).z < std::numeric_limits<float>::epsilon();
    }
};

/**
 * Split the box by 2 in each dimension
 * @param minP
 * @param maxP
 * @return
 */
std::array<Box,8> splitboxby8(const Point &minP, const Point &maxP);


/**
 * BoudingBox class, creates smallest box around the points
 */
class BoudingBox : public Box {

public:
    BoudingBox() : Box({0, 0, 0}, {0, 0, 0}) {};

    explicit BoudingBox(std::vector<float> &d);

    explicit BoudingBox(std::vector<Point> &vertex);
};

#endif //BP_BOX_H
