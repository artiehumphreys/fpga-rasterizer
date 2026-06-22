#pragma once

#include <algorithm>

#include "framebuffer.hpp"
#include "geometry.hpp"

template <int W, int H> void draw_triangle(FrameBuffer<W, H> &fb, Triangle T) {
  auto [A, B, C, color] = T;
  int min_x = std::clamp(std::min({A.x, B.x, C.x}), 0, W - 1);
  int max_x = std::clamp(std::max({A.x, B.x, C.x}), 0, W - 1);
  int min_y = std::clamp(std::min({A.y, B.y, C.y}), 0, H - 1);
  int max_y = std::clamp(std::max({A.y, B.y, C.y}), 0, H - 1);

  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS loop_tripcount min = 1 max = 1920
#pragma HLS pipeline II = 2
#endif
      if (is_inside_triangle(T, {x, y})) {
        fb.set_pixel(x, y, color);
      }
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
