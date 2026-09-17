#pragma once
#include "shader_3d.h"
#include "vg_texture.h"
namespace vg{
  class shader_col:public shader_3d{
    DEF_STRUCT_WITH_INIT(_sd_pt,
      (f32_3,light_position),
      (f32_3,light_ambient),
      (f32_3,light_diffuse),
      (f32_3,light_specular),
      (f32_3,viewPos),
      (f32_3, ambient_clr, {0}),
      (f32_3, diffuse_clr, {0}),
      (f32_3, specular_clr, {0}))
  public:
    void set_ambient_clr(f32_3 col) { 
      _sd_pt.ambient_clr = col;
    }
    void set_diffuse_clr(f32_3 col) { 
      _sd_pt.diffuse_clr = col;
    }    
    void set_specular_clr(f32_3 col) { 
      _sd_pt.specular_clr = col;
    }    

  };
  REG_CTL_2_FAC(shader_col);
}