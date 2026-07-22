#pragma once

#include "geometry.hpp"

inline Triangle scene[] = {
    {{400, 133}, {133, 133}, {267, 267}, {0.5f, 0.0f, 0.75f}},
    {{600, 200}, {867, 200}, {733, 467}, {1.0f, 0.25f, 0.5f}},
    {{200, 400}, {467, 533}, {200, 600}, {0.4f, 0.6f, 0.8f}}};
inline constexpr int scene_count = sizeof(scene) / sizeof(scene[0]);
