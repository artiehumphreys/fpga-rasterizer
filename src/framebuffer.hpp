#pragma once

#include <cassert>

#include "pixel.hpp"

template <int W, int H> struct FrameBuffer {
  static_assert(W > 0 && H > 0, "FrameBuffer dimensions must be positive.");

  // NOTE: storage lives in DDR3
  Pixel *data;

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
    // II=2 floor: HLS won't coalesce these into a burst write
    // TODO: memcpy-style burst rewrite to reach II=1 if fill throughput matters
    for (int i = 0; i < W * H; ++i) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 2
#endif
      data[i] = p;
    }
  }
};
