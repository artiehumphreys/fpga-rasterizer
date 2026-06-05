#pragma once

#include <algorithm>

#include "framebuffer.hpp"
#include "geometry.hpp"
#include "pixel.hpp"

template <int W, int H>
void draw_triangle(FrameBuffer<W, H> &fb, Point A, Point B, Point C,
                   Pixel color) {
  int min_x = std::clamp(std::min({A.x, B.x, C.x}), 0, W - 1);
  int max_x = std::clamp(std::max({A.x, B.x, C.x}), 0, W - 1);
  int min_y = std::clamp(std::min({A.y, B.y, C.y}), 0, H - 1);
  int max_y = std::clamp(std::max({A.y, B.y, C.y}), 0, H - 1);

  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      if (is_inside_triangle(A, B, C, {x, y})) {
        fb.set_pixel(x, y, color);
      }
    }
  }
}

// Concrete v1 entry point. This is the function HLS synthesizes (syn.top)
void rasterizer(FrameBuffer<1920, 1080> &fb);
