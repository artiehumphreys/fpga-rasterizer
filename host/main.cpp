#include <memory>

#include "../src/rasterizer.hpp"

int main() {
  // heap-allocated: FrameBuffer<1920,1080> is ~8 MB, too big for the stack
  auto fb = std::make_unique<FrameBuffer<1920, 1080>>();
  rasterizer(*fb);
  // TODO: dump fb to a PPM so the render can be viewed
  return 0;
}
