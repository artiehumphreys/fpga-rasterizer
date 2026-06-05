#include "rasterizer.hpp"

// v1 scene: a single flat-colored triangle. The geometry is fixed here for now
void rasterizer(FrameBuffer<1920, 1080> &fb) {
  fb.clear();
  draw_triangle(fb, Point{600, 200}, Point{200, 200}, Point{400, 400},
                Pixel{255, 0, 0, 255});
}
