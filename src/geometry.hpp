#pragma once

#include "pixel.hpp"

struct Point {
  int x, y;
};

struct Triangle {
  Point a, b, c;
  Pixel color;
};

struct Bary {
  float u, v, w;
};

struct SubAreas {
  int bcp, cap, abp;
};

constexpr int calculate_cross_product(Point A, Point B, Point P) {
  int lhs = (B.x - A.x) * (P.y - A.y);
  int rhs = (B.y - A.y) * (P.x - A.x);
  return lhs - rhs;
}

constexpr void get_sub_areas(const Triangle &T, Point p, SubAreas &areas) {
  int ABP = calculate_cross_product(T.a, T.b, p);
  int BCP = calculate_cross_product(T.b, T.c, p);
  int CAP = calculate_cross_product(T.c, T.a, p);
  areas = {BCP, CAP, ABP};
}

constexpr void get_barycentric_coordinates(const Triangle &T, Point p,
                                           Bary &coords) {
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle//barycentric-coordinates.html
  SubAreas areas;
  get_sub_areas(T, p, areas);

  float ABC = static_cast<float>(areas.bcp) + areas.cap + areas.abp;
  // u -> BCP / ABC, v -> CAP / ABC, w -> ABP / ABC
  float u = areas.bcp / ABC;
  float v = areas.cap / ABC;
  float w = areas.abp / ABC;

  coords = {u, v, w};
}

constexpr bool is_inside_triangle(const Triangle &T, Point p) {
  // NOTE: Instead of depending on CW ordering of points, just check that
  // determinant signs match
  SubAreas areas;
  get_sub_areas(T, p, areas);

  bool has_neg = (areas.bcp < 0) || (areas.cap < 0) || (areas.abp < 0);
  bool has_pos = (areas.bcp > 0) || (areas.cap > 0) || (areas.abp > 0);

  return !(has_neg && has_pos); // mixed signs -> outside
}
