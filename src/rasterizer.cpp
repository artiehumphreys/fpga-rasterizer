#include "rasterizer.hpp"

void rasterizer(FrameBuffer<1920, 1080> &fb) {
  fb.clear();
  draw_triangle(fb,
                Triangle{{600, 200}, {200, 200}, {400, 400}, {255, 0, 0, 255}});
}
