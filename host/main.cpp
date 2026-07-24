#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

#include "ppm.hpp"
#include "rasterizer.hpp"
#include "scene.hpp"

int main() {
  // heap-allocated: ~3.7 MB, too big for the stack. On hardware this lives in
  // DDR3.
  auto buf = std::make_unique<Pixel[]>(FB_W * FB_H);
  FrameBuffer<FB_W, FB_H> fb{buf.get()};
  render_frame(fb, scene);

  const char *hex = "golden_fb.hex";
  std::FILE *f = std::fopen(hex, "w");
  if (!f) {
    std::fprintf(stderr, "failed to write %s\n", hex);
    return 1;
  }
  for (int y = 0; y < FB_H; ++y) {
    for (int x = 0; x < FB_W; ++x) {
      const Pixel &p = fb.data[FrameBuffer<FB_W, FB_H>::get_pixel_addr(x, y)];
      std::uint32_t w;
      std::memcpy(&w, &p, sizeof w);
      std::fprintf(f, "%08x\n", w);
    }
  }

  std::fclose(f);
  std::printf("wrote %s\n", hex);
  return 0;
}
