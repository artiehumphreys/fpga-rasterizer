#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

#include "ppm.hpp"
#include "rasterizer.hpp"

int main() {
  // heap-allocated: ~8 MB, too big for the stack. On hardware this lives in
  // DDR3.
  auto buf = std::make_unique<Pixel[]>(1920 * 1080);
  rasterizer(buf.get());

  FrameBuffer<1920, 1080> fb{buf.get()};

  const char *hex = "golden_fb.hex";
  std::FILE *f = std::fopen(hex, "w");
  if (!f) {
    std::fprintf(stderr, "failed to write %s\n", hex);
    return 1;
  }
  for (int y = 0; y < 1080; ++y) {
    for (int x = 0; x < 1920; ++x) {
      const Pixel &p = fb.data[FrameBuffer<1920, 1080>::get_pixel_addr(x, y)];
      std::uint32_t w;
      std::memcpy(&w, &p, sizeof w);
      std::fprintf(f, "%08x\n", w);
    }
  }

  std::fclose(f);
  std::printf("wrote %s\n", hex);
  return 0;
}
