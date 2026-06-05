#pragma once

struct Point {
  int x, y;
};

constexpr int calculate_cross_product(Point A, Point B, Point P) {
  int lhs = (B.x - A.x) * (P.y - A.y);
  int rhs = (B.y - A.y) * (P.x - A.x);
  return lhs - rhs;
}

constexpr bool is_inside_triangle(Point A, Point B, Point C, Point P) {
  // NOTE: Instead of depending on CW ordering of points, just check that
  // determinant signs match
  int d0 = calculate_cross_product(A, B, P);
  int d1 = calculate_cross_product(B, C, P);
  int d2 = calculate_cross_product(C, A, P);

  bool has_neg = (d0 < 0) || (d1 < 0) || (d2 < 0);
  bool has_pos = (d0 > 0) || (d1 > 0) || (d2 > 0);

  return !(has_neg && has_pos); // mixed signs -> outside
}
