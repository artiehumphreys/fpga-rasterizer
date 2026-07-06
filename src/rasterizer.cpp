#include "rasterizer.hpp"

static const Triangle scene[] = {
    {{400, 133}, {133, 133}, {267, 267}, rgba(255, 0, 0, 255)},
    {{600, 200}, {867, 200}, {733, 467}, rgba(0, 255, 0, 255)},
    {{200, 400}, {467, 533}, {200, 600}, rgba(0, 0, 255, 255)},
};

void rasterizer(Pixel *fb_mem) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port = fb_mem offset = slave bundle =              \
    gmem0 max_widen_bitwidth = 512 max_write_burst_length =                    \
        256 max_read_burst_length = 256 num_write_outstanding = 8
#pragma HLS INTERFACE s_axilite port = return
#endif
  FrameBuffer<FB_W, FB_H> fb{fb_mem};
  fb.clear();
  draw_triangles(fb, scene, sizeof(scene) / sizeof(scene[0]));
}
