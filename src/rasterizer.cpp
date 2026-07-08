#include "rasterizer.hpp"
#include "hls_stream.h"

static const Triangle scene[] = {
    {{400, 133}, {133, 133}, {267, 267}, rgba(255, 0, 0, 255)},
    {{600, 200}, {867, 200}, {733, 467}, rgba(0, 255, 0, 255)},
    {{200, 400}, {467, 533}, {200, 600}, rgba(0, 0, 255, 255)},
};

void rasterizer(Pixel *fb_mem, hls::stream<bool> &ready,
                hls::stream<bool> &freed) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port = fb_mem offset = slave bundle =              \
    gmem0 max_widen_bitwidth = 128 max_write_burst_length =                    \
        256 max_read_burst_length = 256 num_write_outstanding = 8
#pragma HLS interface axis port = ready
#pragma HLS interface axis port = freed
#pragma HLS INTERFACE ap_ctrl_none port = return
#endif

  static bool curr = false;
  for (;;) {
    Pixel *buff = fb_mem + curr * (FB_W * FB_H);
    FrameBuffer<FB_W, FB_H> fb{buff};
    draw_triangles(fb, scene, sizeof(scene) / sizeof(scene[0]));

    // handshake (swap buffers)
    ready << curr;
    curr = freed.read();
  }
}
