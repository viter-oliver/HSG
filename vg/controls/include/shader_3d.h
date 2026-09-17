#pragma once
#include "shader_control.h"
namespace vg{
  class shader_3d:public shader_control{
  protected:
    DEF_STRUCT_WITH_INIT(_sd3_pt,
     (f32_16,model ),
     (f32_16,view),
     (f32_16,projection))
  public:
    shader_3d(); 
  };
}