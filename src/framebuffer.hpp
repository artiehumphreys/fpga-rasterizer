#pragma once

#include <cassert>

#include "pixel.hpp"

template <int W, int H> struct FrameBuffer {
  static_assert(W > 0 && H > 0, "FrameBuffer dimensions must be positive.");

  Pixel data[W * H];

  static constexpr int get_pixel_addr(int x, int y) { return y * W + x; }

  static constexpr bool is_addr_valid(int x, int y) {
    return x >= 0 && x < W && y >= 0 && y < H;
  }

  // p cheap to copy
  void set_pixel(int x, int y, Pixel p) {
    assert(is_addr_valid(x, y) && "set_pixel: coordinate out of bounds");
    data[get_pixel_addr(x, y)] = p;
  }

  void clear(Pixel p = {}) {
    for (int y = 0; y < H; ++y) {
      for (int x = 0; x < W; ++x) {
        data[get_pixel_addr(x, y)] = p;
      }
    }
  }
};
