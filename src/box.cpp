//
// Created by jvlk on 18.10.23.
//

#include "box.h"


Box::Box(Point &min, Point &max)
        : min(min), max(max) {
    // empty :)
}

Box::Box(Point min, Point max)
        : min(min), max(max) {
    // empty :)
}

Box::Box(double xa, double ya, double za, double xb, double yb, double zb)
        : min(xa, ya, za), max(xb, yb, zb) {
    // empty :)
}

bool Box::isInside(const std::vector<Point> *vertex) {
    for (auto p: *vertex) {
        if (isInside(p))
            return true;
    }
    return false;

}

std::vector<Box> Box::splitBoxBy8() {
    return splitboxby8(min, max);
}


BoudingBox::BoudingBox(std::vector<double> &d) : Box({0, 0, 0}, {0, 0, 0}) {
    for (int i = 0; i < d.size(); ++i) {
        if (min.x > d[i]) {
            min.x = d[i];
        }
        if (min.y > d[i + 1]) {
            min.y = d[i + 1];
        }
        if (min.z > d[i + 2]) {
            min.z = d[i + 2];
        }
        if (max.x > d[i]) {
            min.x = d[i];
        }
        if (max.y > d[i + 1]) {
            min.y = d[i + 1];
        }
        if (max.z > d[i + 2]) {
            min.z = d[i + 2];
        }
    }

}

BoudingBox::BoudingBox(std::vector<Point> &vertex) : Box({0, 0, 0}, {0, 0, 0}) {
    for (auto i: vertex) {
        if (min.x > i.x) {
            min.x = i.x;
        }
        if (min.y > i.y) {
            min.y = i.y;
        }
        if (min.z > i.z) {
            min.z = i.z;
        }
        if (max.x > i.x) {
            min.x = i.x;
        }
        if (max.y > i.y) {
            min.y = i.y;
        }
        if (max.z > i.z) {
            min.z = i.z;
        }
    }

}


std::vector<Box> splitboxby8(const Point &minP, const Point &maxP) {

    // [0] x, [1] y, [2] z
    // calculates size of box and devide it by 2
    double half_size[3] = {
            (maxP.x - minP.x) / 2,
            (maxP.y - minP.y) / 2,
            (maxP.z - minP.z) / 2,
    };

    // [0] x, [1] y, [2],
    double min[3] = {
            minP.x,
            minP.y,
            minP.z,
    };

    // [0] x, [1] y, [2] z
    double max[3] = {minP.x + half_size[0], minP.y + half_size[1],
                     minP.z + half_size[2]};

    std::vector<Box> boxs;

    int index =0;
    for (int i = 0; i < 8; ++i) {
        switch (i) {
            case 0:
                break;
            case 1:
            case 2:
                min[i] += half_size[i];
                max[i] += half_size[i];
                break;
            case 3:
                index = 1;
                min[index] -= half_size[index];
                max[index] -= half_size[index];
                break;
            case 4:
                index = 0;
                min[index] += half_size[index];
                max[index] += half_size[index];
                break;
            case 5:
                index = 1;
                min[index] += half_size[index];
                max[index] += half_size[index];
                break;

            case 6:
                index = 2;
                min[index] -= half_size[index];
                max[index] -= half_size[index];
                break;

            case 7:
                index = 1;
                min[index] -= half_size[index];
                max[index] -= half_size[index];
                break;
            default:
                break;
        }

        auto nb = Box(Point{min[0], min[1], min[2]}, Point{max[0], max[1], max[2]});
        boxs.push_back(nb);
    }



    return boxs;
}
