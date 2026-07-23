#pragma once

struct Point {
  int x, y;
};

struct Triangle {
  Point a, b, c;
  float intensities[3];
};

struct Bary {
  float u, v, w;
};

struct SubAreas { // triangles formed by lines connecting p to all vertices
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

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle//barycentric-coordinates.html
constexpr Bary to_barycentric(const SubAreas &areas, float inv_area) {
  // u -> BCP / ABC, v -> CAP / ABC, w -> ABP / ABC
  float u = areas.bcp * inv_area;
  float v = areas.cap * inv_area;
  float w = 1 - u - v; // avoid unnecessary division
  return {u, v, w};
}

constexpr bool is_inside_triangle(const SubAreas &areas) {
  // NOTE: Instead of depending on CW ordering of points, just check that
  // determinant signs match
  bool has_neg = (areas.bcp < 0) || (areas.cap < 0) || (areas.abp < 0);
  bool has_pos = (areas.bcp > 0) || (areas.cap > 0) || (areas.abp > 0);

  return !(has_neg && has_pos); // mixed signs -> outside
}

constexpr bool is_inside_triangle(const Triangle &T, Point p) {
  SubAreas areas;
  get_sub_areas(T, p, areas);
  return is_inside_triangle(areas);
}
