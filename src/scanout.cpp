#include "scanout.hpp"
#include "hls_stream.h"
#include "pixel.hpp"

template <int W, int H>
void scanout(Pixel *fb_mem, hls::stream<bool> &ready, hls::stream<bool> &freed,
             hls::stream<video_packet_t> &video_out) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port = fb_mem offset = slave bundle =              \
    gmem1 max_widen_bitwidth = 128 max_write_burst_length =                    \
        256 max_read_burst_length = 256 num_read_outstanding = 4
#pragma HLS interface axis port = ready
#pragma HLS interface axis port = freed
#pragma HLS interface axis port = video_out
#pragma HLS INTERFACE ap_ctrl_none port = return
#endif

  static bool curr = true;
  for (;;) {
    // hold last frame (read from current buffer) until next is written
    if (!ready.empty()) { // non-blocking
      bool next = ready.read();
      freed << curr;
      curr = next;
    }

    Pixel *buff = fb_mem + curr * (W * H);
    FrameBuffer<W, H> fb{buff};
    read_lines<W, H>(fb, video_out);
  }
}
