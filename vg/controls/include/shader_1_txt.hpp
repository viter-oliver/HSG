#pragma once
#include "shader_3d.h"
#include "vg_texture.h"
namespace vg{
  class shader_1_txt:public shader_3d{
    DEF_STRUCT_WITH_INIT(_sd_pt,
     (f32_3,light_position),
     (f32_3,light_ambient),
     (f32_3,light_diffuse),
     (f32_3,light_specular),
     (f32_3,viewPos),
     (texture_key, texture_diffuse))
  public:
    void set_diffuse( texture_key& txtk ) { 
      _sd_pt.texture_diffuse = txtk;
    }
    
  };
  REG_CTL_2_FAC(shader_1_txt);
}