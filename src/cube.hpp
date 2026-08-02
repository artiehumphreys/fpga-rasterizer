#pragma once

#include "geometry.hpp"

constexpr int h = 1;

constexpr int VERTICES = 8;
constexpr int CUBE_FACES = 6;
constexpr int CUBE_TRIS = CUBE_FACES * 2;

constexpr vec3 cube_verts[VERTICES] = {{-h, -h, -h}, {h, -h, -h}, {-h, h, -h},
                                       {h, h, -h},   {-h, -h, h}, {h, -h, h},
                                       {-h, h, h},   {h, h, h}};

constexpr int faces[CUBE_FACES][4] = {
    // NOTE: CCW ordering of vertices required for back culling
    {0, 2, 3, 1}, // front
    {6, 4, 5, 7}, // back
    {4, 6, 2, 0}, // left
    {1, 3, 7, 5}, // right
    {0, 1, 5, 4}, // bottom
    {2, 6, 7, 3}, // top
};

constexpr Pixel face_colors[CUBE_FACES] = {
    rgba(255, 0, 0, 0),   rgba(0, 255, 0, 0),   rgba(0, 0, 255, 0),
    rgba(255, 255, 0, 0), rgba(0, 255, 255, 0), rgba(255, 0, 255, 0),
};

inline void build_cube(Tri3 out[CUBE_TRIS]) {
  for (int f = 0; f < CUBE_FACES; ++f) {
    const int *q = faces[f];
    out[f * 2] = {cube_verts[q[0]],
                  cube_verts[q[1]],
                  cube_verts[q[2]],
                  {1, 1, 1},
                  face_colors[f]};
    out[f * 2 + 1] = {cube_verts[q[0]],
                      cube_verts[q[2]],
                      cube_verts[q[3]],
                      {1, 1, 1},
                      face_colors[f]};
  }
}
