#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "framebuffer.hpp"

template <int W, int H>
bool write_ppm(const FrameBuffer<W, H> &fb, const std::string &path) {
  std::ofstream out(path, std::ios::binary);
  if (!out) {
    return false;
  }

  out << "P6\n" << W << ' ' << H << "\n255\n";

  std::vector<unsigned char> rgb;
  rgb.reserve(static_cast<std::size_t>(W) * H * 3);
  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      const Pixel &p = fb.data[FrameBuffer<W, H>::get_pixel_addr(x, y)];
      rgb.push_back(red(p));
      rgb.push_back(green(p));
      rgb.push_back(blue(p));
    }
  }
  out.write(reinterpret_cast<const char *>(rgb.data()),
            static_cast<std::streamsize>(rgb.size()));

  return static_cast<bool>(out);
}
