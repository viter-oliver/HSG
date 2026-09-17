#pragma once
#include "shader.h"
#include "vertex.h"
#include "vg_texture.h"
namespace vg{
  using namespace shader;
  void add_shader_base_by_key(shader_base_key& key, sd_shader_base& s_sh);
  void add_vertex_by_key(vertex_key& key, sd_vertex& s_vtx);
  void add_texture_by_key(texture_key& key, sd_texture& s_txt);
}
