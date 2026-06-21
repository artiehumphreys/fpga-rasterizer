#include "rasterizer.hpp"

static const Triangle scene[] = {
    {{600, 200}, {200, 200}, {400, 400}, {255, 0, 0, 255}},
    {{900, 300}, {1300, 300}, {1100, 700}, {0, 255, 0, 255}},
    {{300, 600}, {700, 800}, {300, 900}, {0, 0, 255, 255}},
};

void rasterizer(FrameBuffer<1920, 1080> &fb) {
  fb.clear();
  draw_triangles(fb, scene, sizeof(scene) / sizeof(scene[0]));
}
