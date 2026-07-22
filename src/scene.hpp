#pragma once

#include "geometry.hpp"

inline Triangle scene[] = {
    {{400, 133}, {133, 133}, {267, 267}, {0.5, 0, 0.75}},
    {{600, 200}, {867, 200}, {733, 467}, {1, 0.25, 0.5}},
    {{200, 400}, {467, 533}, {200, 600}, {0.4, 0.6, 0.8}}};
inline constexpr int scene_count = sizeof(scene) / sizeof(scene[0]);
