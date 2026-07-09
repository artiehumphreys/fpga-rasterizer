#include "ap_axi_sdata.h"
#include "framebuffer.hpp"
#include "pixel.hpp"
#include <cstddef>

using video_packet_t = ap_axiu<sizeof(Pixel) * 8, 1, 0, 0>;

template <int W, int H>
void read_line(FrameBuffer<W, H> &fb, int y,
               hls::stream<video_packet_t> &video_out) {
  for (std::size_t x = 0; x < W; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
    Pixel pixel = fb.data[fb.get_pixel_addr(x, y)];
    video_packet_t p;
    p.data = pixel;

    p.keep = -1;
    p.strb = -1;                 // each bit is valid
    p.user = (y == 0 && x == 0); // SOF
    p.last = (x == W - 1);       // EOL
    video_out.write(p);
  }
}

template <int W, int H>
void read_lines(FrameBuffer<W, H> &fb, hls::stream<video_packet_t> &video_out) {
  for (std::size_t y = 0; y < H; ++y) {
    read_line(fb, y, video_out);
  }
}

template <int W, int H>
void scanout(Pixel *fb_mem, hls::stream<bool> &ready, hls::stream<bool> &freed,
             hls::stream<video_packet_t> &video_out);
