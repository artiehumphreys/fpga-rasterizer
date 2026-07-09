#include "framebuffer.hpp"
#include "rasterizer.hpp"
#include "scanout.hpp"
#include <cassert>

void test_triangle_rasterization() {
  const int W = 10, H = 7;
  const char *expected[H] = {
      "..........", "..........", "..#####...", "...###....",
      "....#.....", "..........", "..........",
  };

  Point A{6, 2}, B{2, 2}, C{4, 4};
  Pixel P = rgba(255, 0, 0, 255);

  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};
  fb.clear();
  draw_triangle(fb, {A, B, C, P});

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
  Pixel P = rgba(255, 0, 0, 255);
  Triangle t0{{6, 2}, {2, 2}, {4, 4}, P};
  Triangle t1{{0, 0}, {3, 0}, {0, 3}, P};
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
  draw_triangle(fb, {{2, 2}, {2, 8}, {8, 2}, rgba(255, 0, 0, 255)});

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
  draw_triangle(fb, {{10, 10}, {12, 10}, {10, 12}, rgba(255, 0, 0, 255)});

  for (int i = 0; i < W * H; ++i) {
    assert(fb.data[i] == 0);
  }
}

void test_read_line() {
  const int W = 5, H = 5;
  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      fb.data[fb.get_pixel_addr(x, y)] = x + y * W;
    }
  }

  hls::stream<video_packet_t> video_out;
  read_lines(fb, video_out);

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      auto packet = video_out.read();
      assert(packet.data == x + y * W);
      assert(packet.user == (y == 0 && x == 0));
      assert(packet.last == (x == W - 1));
    }
  }
  assert(video_out.empty());
}

int main() {
  test_triangle_rasterization();
  test_draw_triangles_draws_all();
  test_clips_offscreen_triangle();
  test_offscreen_triangle_draws_nothing();
  test_read_line();
  return 0;
}
