#pragma once

#include <algorithm>

#include "framebuffer.hpp"
#include "geometry.hpp"

constexpr int FB_W = 1280;
constexpr int FB_H = 720;

template <int W, int H> void draw_triangle(FrameBuffer<W, H> &fb, Triangle T) {
  auto [A, B, C, color] = T;
  int min_x = std::clamp(std::min({A.x, B.x, C.x}), 0, W - 1);
  int max_x = std::clamp(std::max({A.x, B.x, C.x}), 0, W - 1);
  int min_y = std::clamp(std::min({A.y, B.y, C.y}), 0, H - 1);
  int max_y = std::clamp(std::max({A.y, B.y, C.y}), 0, H - 1);

  for (int y = min_y; y <= max_y; ++y) {
    Pixel line[W];

    // TODO: span-only burst across [min_x, max_x]
    for (int i = 0; i < W; ++i) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      line[i] = fb.data[y * W + i];
    }

    for (int x = min_x; x <= max_x; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS loop_tripcount min = 1 max = FB_W
#pragma HLS pipeline II = 1
#endif
      if (is_inside_triangle(T, {x, y})) {
        line[x] = color;
      }
    }

    for (int i = 0; i < W; ++i) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      fb.data[y * W + i] = line[i];
    }
  }
}

template <int W, int H>
void draw_triangles(FrameBuffer<W, H> &fb, const Triangle *tris, int n) {
  for (int i = 0; i < n; ++i) {
    draw_triangle(fb, tris[i]);
  }
}

void rasterizer(Pixel *fb_mem);
