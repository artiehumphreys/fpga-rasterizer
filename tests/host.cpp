#include "framebuffer.hpp"
#include "rasterizer.hpp"
#include <cassert>

void test_triangle_rasterization() {
  const int W = 10, H = 7;
  const char *expected[H] = {
      "..........", "..........", "..#####...", "...###....",
      "....#.....", "..........", "..........",
  };

  Point A{6, 2}, B{2, 2}, C{4, 4};

  FrameBuffer<W, H> fb;
  fb.clear();
  draw_triangle(fb, A, B, C, Pixel{255, 0, 0, 255});

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      bool is_set = fb.data[fb.get_pixel_addr(x, y)].r == 255;
      assert(is_set == expected[y][x]);
    }
  }
}
