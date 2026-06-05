#include <cstdio>
#include <memory>

#include "rasterizer.hpp"
#include "ppm.hpp"

int main() {
  // heap-allocated: FrameBuffer<1920,1080> is ~8 MB, too big for the stack
  auto fb = std::make_unique<FrameBuffer<1920, 1080>>();
  rasterizer(*fb);

  const char *path = "out.ppm";
  if (!write_ppm(*fb, path)) {
    std::fprintf(stderr, "failed to write %s\n", path);
    return 1;
  }
  std::printf("wrote %s\n", path);
  return 0;
}
