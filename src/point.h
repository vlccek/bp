#include <cmath>
#include <iostream>
#include <string>

#ifndef BP_POINT_H
#define BP_POINT_H

template <typename T> class PointGeneric {
public:
  T x, y, z;

  PointGeneric(T x, T y, T z) : x(x), y(y), z(z) {}

  template <typename U> auto operator>(const PointGeneric<U> &p) const -> bool {
    return (x > p.x) && (y > p.y) && (z > p.z);
  }

  template <typename U> auto operator<(const PointGeneric<U> &p) const -> bool {
    return (x < p.x) && (y < p.y) && (z < p.z);
  }

  template <typename U>
  auto operator>=(const PointGeneric<U> &p) const -> bool {
    return x >= p.x && y >= p.y && z >= p.z;
  }

  template <typename U>
  auto operator<=(const PointGeneric<U> &p) const -> bool {
    return x <= p.x && y <= p.y && z <= p.z;
  }

  template <typename U>
  auto operator-(const PointGeneric<U> &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x - p.x, y - p.y, z - p.z);
  }

  template <typename U> auto operator-() const -> PointGeneric<T> {
    return PointGeneric<T>(-x, -y, -z);
  }

  template <typename U>
  auto operator+(const PointGeneric<U> &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x + p.x, y + p.y, z + p.z);
  }

  auto operator/(const int &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x / p, y / p, z / p);
  }

  template <typename U>
  auto operator/(const PointGeneric<U> &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x / p.x, y / p.y, z / p.z);
  }

  auto operator*(const T &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x * p, y * p, z * p);
  }

  auto operator*(const PointGeneric<T> &p) const -> T {
    return x * p.x + y * p.y + z * p.z;
  }

  auto operator==(const PointGeneric<T> &p) const -> bool {
    return (x == p.x) && (y == p.y) && (z == p.z);
  }

  template <class U>
  auto operator*(const PointGeneric<U> &p) const -> PointGeneric<T> {
    return PointGeneric<T>(y * p.z - z * p.y, z * p.x - x * p.z,
                           x * p.y - y * p.x);
  }

  auto operator-(const PointGeneric<T> &p) const -> PointGeneric<T> {
    return PointGeneric<T>(x - p.x, y - p.y, z - p.z);
  }

  explicit operator PointGeneric<float>() const {
    PointGeneric<double>(x, y, z);
  }

  auto operator-() const -> PointGeneric<T> {
    return PointGeneric<T>(-x, -y, -z);
  }

  template <typename U> auto distance(PointGeneric<U> p) const -> U {
    return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) +
                     (z - p.z) * (z - p.z));
  }

  operator std::string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  void repr() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }
};

template <typename T, typename U> inline T cross(const T &p1, const U &p2) {
  return {(p1.y * p2.z) - (p1.z * p2.y), (p1.z * p2.x) - (p1.x * p2.z),
          (p1.x * p2.y) - (p1.y * p2.x)};
}

using Point = PointGeneric<float>;
using PointDouble = PointGeneric<float>;
#endif
