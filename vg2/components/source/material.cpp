#include "material.h"

namespace vg {
namespace shader {

material::material(sd_shader_base pshader) : _pshader(pshader) {
  _pshader->spawn_mp_sd_shader_variable(_uniform_variales);
}
void material::refresh_uniform_variables() {
  mp_sd_shader_variable temp_uniforms;
  _pshader->spawn_mp_sd_shader_variable(temp_uniforms);
  for (auto &ishd_v : temp_uniforms) { // NOTE:为了最大程度内保留值
    auto key = ishd_v.first;
    auto if_shdv = _uniform_variales.find(key);
    if (if_shdv != _uniform_variales.end()) {
      auto &shd_v_new = *ishd_v.second.get();
      auto sd_v_old = if_shdv->second.get();
      // DOUBT:没办法!
      shd_v_new = sd_v_old;
    }
  }
  _uniform_variales.clear();
  _uniform_variales = temp_uniforms;
}
void material::use() {
  _pshader->use();
  for (auto &shd_v : _uniform_variales) {
    shd_v.second->set_data_to_host();
  }
}
void refresh_material(sd_shader_base &sd_shd_i) {
  for (auto &imtl : _mp_sd_materials) {
    auto &sd_mtl = imtl.second;
    if (sd_mtl->be_same_shader(sd_shd_i)) {
      sd_mtl->refresh_uniform_variables();
    }
  }
}
} // namespace shader
} // namespace vg