#include "framebuffer.hpp"
#include "pixel.hpp"
#include "rasterizer.hpp"
#include <cassert>

void test_triangle_rasterization() {
  const int W = 10, H = 7;
  const char *expected[H] = {
      "..........", "..........", "..#####...", "...###....",
      "....#.....", "..........", "..........",
  };

  Point A{6, 2}, B{2, 2}, C{4, 4};

  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};
  fb.clear();
  draw_triangle(fb, {A, B, C, {1.0f, 1.0f, 1.0f}, rgba(255, 0, 0, 0)});

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      bool is_set = red(fb.data[fb.get_pixel_addr(x, y)]) == 255;
      bool want = expected[y][x] == '#';
      assert(is_set == want);
    }
  }
}

void test_draw_triangles_draws_all() {
  const int W = 10, H = 7;
  Triangle t0{{6, 2}, {2, 2}, {4, 4}, {1.0f, 1.0f, 1.0f}, rgba(255, 0, 0, 0)};
  Triangle t1{{0, 0}, {3, 0}, {0, 3}, {1.0f, 1.0f, 1.0f}, rgba(255, 0, 0, 0)};
  Triangle scene[2] = {t0, t1};

  Pixel expected_storage[W * H];
  FrameBuffer<W, H> expected{expected_storage};
  expected.clear();
  draw_triangle(expected, t0);
  draw_triangle(expected, t1);

  Pixel actual_storage[W * H];
  FrameBuffer<W, H> actual{actual_storage};
  actual.clear();
  draw_triangles(actual, scene, 2);

  for (int i = 0; i < W * H; ++i) {
    assert(actual.data[i] == expected.data[i]);
  }
}

void test_clips_offscreen_triangle() {
  const int W = 5, H = 5;
  const char *expected[H] = {
      ".....", ".....", "..###", "..###", "..###",
  };

  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};
  fb.clear();
  draw_triangle(
      fb, {{2, 2}, {2, 8}, {8, 2}, {1.0f, 1.0f, 1.0f}, rgba(255, 0, 0, 0)});

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      bool is_set = red(fb.data[fb.get_pixel_addr(x, y)]) == 255;
      assert(is_set == (expected[y][x] == '#'));
    }
  }
}

void test_offscreen_triangle_draws_nothing() {
  const int W = 5, H = 5;
  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};
  fb.clear();
  draw_triangle(
      fb,
      {{10, 10}, {12, 10}, {10, 12}, {1.0f, 1.0f, 1.0f}, rgba(255, 0, 0, 0)});

  for (int i = 0; i < W * H; ++i) {
    assert(fb.data[i] == 0);
  }
}

void test_shading() {
  const int W = 8, H = 8;
  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};
  fb.clear();

  Point A{0, 0}, B{4, 0}, C{0, 4};
  draw_triangle(fb, {A, B, C, {1.0f, 0.5f, 0.25f}, rgba(255, 0, 0, 0)});

  auto gray = [&](int x, int y) {
    return static_cast<int>(red(fb.data[fb.get_pixel_addr(x, y)]));
  };
  auto near = [](int got, int want) {
    return got >= want - 1 && got <= want + 1;
  };

  assert(near(gray(0, 0), 255)); // a: 1.00 * 255
  assert(near(gray(4, 0), 127)); // b: 0.50 * 255
  assert(near(gray(0, 4), 63));  // c: 0.25 * 255

  // interior (1,1): u=0.5, v=0.25, w=0.25 -> I = 0.6875 -> 175
  assert(near(gray(1, 1), 175));
}

int main() {
  test_triangle_rasterization();
  test_draw_triangles_draws_all();
  test_clips_offscreen_triangle();
  test_offscreen_triangle_draws_nothing();
  test_shading();
  return 0;
}
