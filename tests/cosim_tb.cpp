#include <cstdio>
#include <memory>

#include "rasterizer.hpp"

static bool is_rgb(const Pixel &p, int r, int g, int b) {
  return red(p) == r && green(p) == g && blue(p) == b;
}

int main() {
  auto buf = std::make_unique<Pixel[]>(1920 * 1080);
  rasterizer(buf.get());

  auto at = [&](int x, int y) -> const Pixel & { return buf[y * 1920 + x]; };

  int fails = 0;
  fails += !is_rgb(at(400, 266), 255, 0, 0);
  fails += !is_rgb(at(1100, 433), 0, 255, 0);
  fails += !is_rgb(at(433, 766), 0, 0, 255);
  fails += !is_rgb(at(0, 0), 0, 0, 0);

  if (fails) {
    std::printf("cosim_tb FAIL: %d/4 checks failed\n", fails);
    return 1;
  }
  std::printf("cosim_tb PASS\n");
  return 0;
}
