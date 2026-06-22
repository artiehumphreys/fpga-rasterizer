#include <cstdio>
#include <memory>

#include "rasterizer.hpp"
#include "ppm.hpp"

int main() {
  // heap-allocated: ~8 MB, too big for the stack. On hardware this lives in DDR3.
  auto buf = std::make_unique<Pixel[]>(1920 * 1080);
  rasterizer(buf.get());

  FrameBuffer<1920, 1080> fb{buf.get()};
  const char *path = "out.ppm";
  if (!write_ppm(fb, path)) {
    std::fprintf(stderr, "failed to write %s\n", path);
    return 1;
  }
  std::printf("wrote %s\n", path);
  return 0;
}
