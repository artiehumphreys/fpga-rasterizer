#pragma once

#include "pixel.hpp"

struct Point {
  int x, y;
};

struct Triangle {
  Point a, b, c;
  Pixel color;
};

constexpr int calculate_cross_product(Point A, Point B, Point P) {
  int lhs = (B.x - A.x) * (P.y - A.y);
  int rhs = (B.y - A.y) * (P.x - A.x);
  return lhs - rhs;
}

constexpr bool is_inside_triangle(Triangle T, Point p) {
  // NOTE: Instead of depending on CW ordering of points, just check that
  // determinant signs match
  int d0 = calculate_cross_product(T.a, T.b, p);
  int d1 = calculate_cross_product(T.b, T.c, p);
  int d2 = calculate_cross_product(T.c, T.a, p);

  bool has_neg = (d0 < 0) || (d1 < 0) || (d2 < 0);
  bool has_pos = (d0 > 0) || (d1 > 0) || (d2 > 0);

  return !(has_neg && has_pos); // mixed signs -> outside
}
