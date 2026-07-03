#pragma once

#include <cstdint>

using Pixel = std::uint32_t;

constexpr Pixel rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b,
                     std::uint8_t a) {
  return (Pixel(r) | Pixel(g) << 8 | Pixel(b) << 16 | Pixel(a) << 24);
}

constexpr std::uint8_t red(Pixel p) { return p & 0xff; }
constexpr std::uint8_t green(Pixel p) { return (p >> 8) & 0xff; }
constexpr std::uint8_t blue(Pixel p) { return (p >> 16) & 0xff; }
