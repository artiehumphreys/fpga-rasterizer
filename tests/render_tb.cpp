#include <cstdio>
#include <memory>

#include "rasterizer.hpp"
#include "scene.hpp"

static bool is_rgb(const Pixel &p, int r, int g, int b) {
  return red(p) == r && green(p) == g && blue(p) == b;
}

int main() {
  auto buf = std::make_unique<Pixel[]>(FB_W * FB_H);
  FrameBuffer<FB_W, FB_H> fb{buf.get()};
  fb.clear();
  draw_triangles(fb, scene, scene_count);

  auto at = [&](int x, int y) -> const Pixel & { return buf[y * FB_W + x]; };

  int fails = 0;
  fails += !is_rgb(at(267, 178), 255, 0, 0);
  fails += !is_rgb(at(733, 289), 0, 255, 0);
  fails += !is_rgb(at(289, 511), 0, 0, 255);
  fails += !is_rgb(at(0, 0), 0, 0, 0);

  if (fails) {
    std::printf("render_tb FAIL: %d/4 checks failed\n", fails);
    return 1;
  }
  std::printf("render_tb PASS\n");
  return 0;
}
