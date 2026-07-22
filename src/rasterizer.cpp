#include "rasterizer.hpp"
#include "hls_stream.h"
#include "scene.hpp"

void rasterizer(Pixel *fb_mem, hls::stream<bool> &ready,
                hls::stream<bool> &freed) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port = fb_mem offset = off bundle =                \
    gmem0 max_widen_bitwidth = 128 max_write_burst_length =                    \
        256 max_read_burst_length = 256 num_write_outstanding = 8
#pragma HLS interface axis port = ready
#pragma HLS interface axis port = freed
#pragma HLS INTERFACE ap_ctrl_none port = return
#endif

  static bool curr = false;
  static int t = 0;
  static const Triangle base[scene_count] = {scene[0], scene[1], scene[2]};

  for (;;) {
    constexpr int period = 400;
    int off = t % period;
    for (int i = 0; i < scene_count; ++i) {
      scene[i].a.x = base[i].a.x + off;
      scene[i].b.x = base[i].b.x + off;
      scene[i].c.x = base[i].c.x + off;
    }

    Pixel *buff = fb_mem + curr * (FB_W * FB_H);
    FrameBuffer<FB_W, FB_H> fb{buff};
    render_frame(fb, scene, scene_count);
    ++t;

    // handshake (swap buffers)
    ready << curr;
    curr = freed.read();
  }
}
