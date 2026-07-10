#pragma once

#include "geometry.hpp"
#include "pixel.hpp"

inline constexpr Triangle scene[] = {
    {{400, 133}, {133, 133}, {267, 267}, rgba(255, 0, 0, 255)},
    {{600, 200}, {867, 200}, {733, 467}, rgba(0, 255, 0, 255)},
    {{200, 400}, {467, 533}, {200, 600}, rgba(0, 0, 255, 255)},
};
inline constexpr int scene_count = sizeof(scene) / sizeof(scene[0]);
