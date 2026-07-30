#pragma once

#include "pixel.hpp"
struct Point {
  int x, y;
};

struct vec3 {
  float x, y, z;
};

struct Triangle { // 2D pixel-space, rasterizer input
  Point a, b, c;
  float intensities[3];
  Pixel color;
};

struct Tri3 { // 3D model-space
  vec3 a, b, c;
  float intensities[3];
  Pixel color;
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

inline bool is_inside_triangle(const Triangle &T, Point p) {
  SubAreas areas;
  get_sub_areas(T, p, areas);
  return is_inside_triangle(areas);
}

template <int W, int H> constexpr Point project(vec3 v, float focal) {
  float recip = -1.0f / v.z;
  float x = focal * v.x * recip;
  float y = focal * v.y * recip;

  // map normalized coords [-1,1] to pixels: offset to center, scale by half
  // NOTE: screen y grows down
  float half = H * 0.5f;
  int sx = static_cast<int>(W * 0.5f + x * half + 0.5f);
  int sy = static_cast<int>(H * 0.5f - y * half + 0.5f);
  return {sx, sy};
}

template <int W, int H> constexpr Triangle project(const Tri3 &t, float focal) {
  return {project<W, H>(t.a, focal),
          project<W, H>(t.b, focal),
          project<W, H>(t.c, focal),
          {t.intensities[0], t.intensities[1], t.intensities[2]},
          t.color};
}
