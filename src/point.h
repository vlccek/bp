#include <cmath>
#include <iostream>
#include <string>

#ifndef BP_POINT_H
#define BP_POINT_H

class Point {
public:
  float x, y, z;
  float nor_x, nor_y, nor_z; // normal vector

  Point(float x, float y, float z, float nor_x = 0, float nor_y = 0,
        float nor_z = 0)
      : x(x), y(y), z(z), nor_x(nor_x), nor_y(nor_y), nor_z(nor_z) {}

  auto operator>(const Point &p) const -> bool {
    return (x > p.x) && (y > p.y) && (z > p.z);
  }

  auto operator<(const Point &p) const -> bool {
    return (x < p.x) && (y < p.y) && (z < p.z);
  }

  auto operator>=(const Point &p) const -> bool {
    return x >= p.x && y >= p.y && z >= p.z;
  }

  auto operator<=(const Point &p) const -> bool {
    return x <= p.x && y <= p.y && z <= p.z;
  }

  auto operator-(const Point &p) const -> Point {
    return Point(x - p.x, y - p.y, z - p.z);
  }

  auto operator-() const -> Point { return Point(-x, -y, -z); }

  auto operator+(const Point &p) const -> Point {
    return Point(x + p.x, y + p.y, z + p.z);
  }

  auto operator/(const int &p) const -> Point {
    return Point(x / p, y / p, z / p);
  }

  auto operator/(const Point &p) const -> Point {
    return Point(x / p.x, y / p.y, z / p.z);
  }

  auto operator*(const float &p) const -> Point {
    return Point(x * p, y * p, z * p);
  }

  auto operator*(const Point &p) const -> float {
    return x * p.x + y * p.y + z * p.z;
  }

  auto operator==(const Point &p) const -> bool {
    return (x == p.x) && (y == p.y) && (z == p.z);
  }

  template <class U> auto operator*(const Point &p) const -> Point {
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
  }

  [[nodiscard]] float distance(Point p) const {
    return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) +
                     (z - p.z) * (z - p.z));
  }

  explicit operator std::string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  void repr() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }
};

inline Point cross(const Point &p1, const Point &p2) {
  return {(p1.y * p2.z) - (p1.z * p2.y), (p1.z * p2.x) - (p1.x * p2.z),
          (p1.x * p2.y) - (p1.y * p2.x)};
}

#endif
