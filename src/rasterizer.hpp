#pragma once

#include <algorithm>

#include "framebuffer.hpp"
#include "geometry.hpp"
#include "pixel.hpp"

constexpr int FB_W = 1280;
constexpr int FB_H = 720;
constexpr int BURST_BITS = 128;

constexpr std::uint8_t get_channel(std::uint8_t channel, float intensity) {
  float val = intensity * channel + 0.5f; // rounding
  return static_cast<uint8_t>(std::clamp(val, 0.0f, 255.0f));
};

constexpr Pixel color_pixel(const Triangle &T, const SubAreas &areas,
                            float inv_area) {
  Bary c = to_barycentric(areas, inv_area);
  float intensity =
      c.u * T.intensities[0] + c.v * T.intensities[1] + c.w * T.intensities[2];
  float adjusted_intensity = std::clamp(intensity, 0.0f, 1.0f);

  Pixel out = rgba(get_channel(red(T.color), adjusted_intensity),
                   get_channel(green(T.color), adjusted_intensity),
                   get_channel(blue(T.color), adjusted_intensity), 0);
  return out;
}

template <int W>
void fill_span(Pixel *line, const Triangle &T, int y, float inv_area) {
  int min_x = std::clamp(std::min({T.a.x, T.b.x, T.c.x}), 0, W - 1);
  int max_x = std::clamp(std::max({T.a.x, T.b.x, T.c.x}), 0, W - 1);
  int min_y = std::min({T.a.y, T.b.y, T.c.y});
  int max_y = std::max({T.a.y, T.b.y, T.c.y});
  if (y < min_y || y > max_y)
    return;

  for (int x = min_x; x <= max_x; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS loop_tripcount min = 1 max = W
#pragma HLS pipeline II = 1
#endif
    Point p = {x, y};
    SubAreas areas;
    get_sub_areas(T, p, areas);
    if (is_inside_triangle(areas))
      line[x] = color_pixel(T, areas, inv_area);
  }
}

template <int W, int H> void draw_triangle(FrameBuffer<W, H> &fb, Triangle T) {
  int min_y = std::clamp(std::min({T.a.y, T.b.y, T.c.y}), 0, H - 1);
  int max_y = std::clamp(std::max({T.a.y, T.b.y, T.c.y}), 0, H - 1);
  float inv_area = 1.0f / calculate_cross_product(T.a, T.b, T.c);

  for (int y = min_y; y <= max_y; ++y) {
    Pixel line[W];

    // TODO: span-only burst across [min_x, max_x]
    for (int i = 0; i < W; ++i) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      line[i] = fb.data[y * W + i];
    }

    fill_span<W>(line, T, y, inv_area);

    for (int i = 0; i < W; ++i) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      fb.data[y * W + i] = line[i];
    }
  }
}

template <int W, int H>
void draw_triangles(FrameBuffer<W, H> &fb, const Triangle *tris, int n) {
  for (int i = 0; i < n; ++i) {
    draw_triangle(fb, tris[i]);
  }
}

template <int W, int H>
void render_frame(FrameBuffer<W, H> &fb, const Triangle *tris, int n) {
  for (int y = 0; y < H; ++y) {
    Pixel line[W];

    for (int x = 0; x < W; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      line[x] = 0;
    }

    for (int t = 0; t < n; ++t) {
      Triangle T = tris[t];
      float inv_area = 1.0f / calculate_cross_product(T.a, T.b, T.c);
      fill_span<W>(line, T, y, inv_area);
    }

    for (int x = 0; x < W; ++x) {
#ifdef __SYNTHESIS__
#pragma HLS pipeline II = 1
#endif
      fb.data[y * W + x] = line[x];
    }
  }
}
