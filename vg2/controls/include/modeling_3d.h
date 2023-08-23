#pragma once
#include "control_fac.h"
#include "model.h"
#include "packing_texture_container.h"
#include "shader.h"
namespace vg {

class AFG_EXPORT modeling_3d : public control_base {

  DEF_STRUCT_WITH_INIT(_pty_page, 
  (char_50, _model_name),
  (vec3, _light_ambient_clr), 
  (vec3, _light_diffuse_clr),
  (vec3, _light_specular_clr), 
  (float, _light_posx, {0.f}),
  (float, _light_posy, {0.f}), 
  (float, _light_posz, {0.f}),
  (float, _light_constant_hac, {1.f}),
  (float, _light_linear_hac, {0.01f}),
  (float, _light_quadratic_hac, {0.001f}),
  (u32, _diffuse_txt),
  (u32, _specular_txt),
  (camera, _cam),
  (projection, _pj), 
  (int, _draw_mode, {en_gl_triangles}))

  shader::sd_shader_base _pshd_modeling;
  model::sd_mesh_list _pmodel;
  sd_packing_texturedef _pdiffuse;
  sd_packing_texturedef _pspecular;

public:
  modeling_3d();
  void draw();
  void link();
  DECLARE_EX_INT
  camera &get_cam() { return _pty_page._cam; }
  projection &get_proj() { return _pty_page._pj; }
};
REG_CTL_2_FAC(modeling_3d);
} // namespace vg