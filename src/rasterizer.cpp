#include "rasterizer.hpp"

static const Triangle scene[] = {
    {{600, 200}, {200, 200}, {400, 400}, rgba(255, 0, 0, 255)},
    {{900, 300}, {1300, 300}, {1100, 700}, rgba(0, 255, 0, 255)},
    {{300, 600}, {700, 800}, {300, 900}, rgba(0, 0, 255, 255)},
};

void rasterizer(Pixel *fb_mem) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port = fb_mem offset = slave bundle =              \
    gmem0 max_widen_bitwidth = 512 max_write_burst_length =                    \
        256 max_read_burst_length = 256 num_write_outstanding = 8
#pragma HLS INTERFACE s_axilite port = return
#endif
  FrameBuffer<1920, 1080> fb{fb_mem};
  fb.clear();
  draw_triangles(fb, scene, sizeof(scene) / sizeof(scene[0]));
}
