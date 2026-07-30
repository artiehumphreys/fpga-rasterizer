#include "rasterizer.hpp"
#include "cube.hpp"
#include "hls_stream.h"

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
  static float angle = 0.0f;
  Tri3 cube[CUBE_TRIS];
  build_cube(cube);

  for (;;) {
    Pixel *buff = fb_mem + curr * (FB_W * FB_H);
    FrameBuffer<FB_W, FB_H> fb{buff};
    render_cube(fb, cube, 1.0f, -5.0f, angle);
    angle += 0.02f;

    // handshake (swap buffers)
    ready << curr;
    curr = freed.read();
  }
}
