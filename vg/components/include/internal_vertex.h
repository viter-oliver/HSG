#pragma once
#include "vg_type.h"
namespace vg {
  inline float triangle[] = {
      // positions         // colors
        1.f, -1.f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
      -1.f, -1.f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  1.f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
  };
  inline float squre[] = {
      // positions         // uv
      -0.5f, -0.5f,0.f, 0.f, 1.f,
     0.5f, -0.5f, 0.f,1.f, 1.f,
     -0.5f, 0.5f, 0.f,0.f, 0.f,
     0.5f, 0.5f,0.f, 1.f, 0.f,
  };
}