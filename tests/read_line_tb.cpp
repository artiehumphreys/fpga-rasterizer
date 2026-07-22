#include "framebuffer.hpp"
#include "hls_stream.h"
#include "pixel.hpp"
#include "scanout.hpp"
#include <cassert>

void test_read_line() {
  const int W = 5, H = 5;
  Pixel storage[W * H];
  FrameBuffer<W, H> fb{storage};

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      fb.data[fb.get_pixel_addr(x, y)] = x + y * W;
    }
  }

  hls::stream<video_packet_t> video_out;
  read_lines(fb, video_out);

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      auto packet = video_out.read();
      assert(packet.data == x + y * W);
      assert(packet.user == (y == 0 && x == 0));
      assert(packet.last == (x == W - 1));
    }
  }
  assert(video_out.empty());
}

int main() {
  test_read_line();
  return 0;
}
