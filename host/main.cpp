#include <memory>

#include "../src/framebuffer.hpp"

int main() {
  auto fb = std::make_unique<FrameBuffer<1920, 1080>>();
  fb->clear();
  return 0;
}
