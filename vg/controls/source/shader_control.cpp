#include "shader_control.h"
#include "internal_shader.h"
#include "non_rolling_log.h"
#include "vg_texture.h"
namespace vg{
  using namespace shader;
  void shader_control::draw(){
    std::string shader_name=typeid(*this).name();
    shader_name=shader_name.substr(sizeof("class vg:"));
    shader_base_key sd_key;
    sd_key.get_str_value(shader_name);
    auto itl_shd=intenal_shader_.find(sd_key);
    sd_shader_base sd_host;
    if(itl_shd!=intenal_shader_.end()){
      sd_host = itl_shd->second;
    }
    if (!sd_host) {
      auto ishd = shader_base_map_.find(sd_key);
      if (ishd != shader_base_map_.end()) {
        sd_host = ishd->second;
      }
    }
    if (!sd_host) {
      printf("fail to find shader by%s", shader_name.c_str());
      return;
    }
    sd_vertex sd_data;
    auto ivtx = vertex_map_.find(_pt.input_data);
    if (ivtx != vertex_map_.end()) {
      sd_data = ivtx->second;
    }
    if (!sd_host) {
      printf("fail to find vertex by%s", _pt.input_data.value);
      return;
    }
    if (!sd_host->match_format(sd_data->_format)) {
      printf("shader:%s fail to match vertex%s",
               shader_name.c_str() ,_pt.input_data.value);
      return;
    }
    sd_host->use();
    auto parent = get_parent();
    auto test_ptr = dynamic_cast<shared_properties *>(parent);
    if (test_ptr) {

      test_ptr->calculate_properties(_m_bind_var);
    }
    int texture_cnt = 0;
    for (int pt_id = input_data + 1; pt_id < _v_bind_var.size(); ++pt_id) {
      auto s_bindv = _v_bind_var[pt_id];
#define UNFORM_BASE(type)                                                    \
      auto s_bind_##type = dynamic_pointer_cast<bind_var_t<type>>(s_bindv);\
      if (s_bind_##type) {                                                       \
          auto &bind_##type = *s_bind_##type;\
        sd_host->uniform(bind_##type.var_name(), bind_##type._value);\
      }
      UNFORM_BASE(float)
      UNFORM_BASE(int)
      UNFORM_BASE(bool)
      UNFORM_BASE(double)
      UNFORM_BASE(u32)
#define UNFORM_ARRAY(type,n)                                                    \
      auto s_bind_##type##n =                                                     \
          dynamic_pointer_cast<bind_var_t<vg_array<type, n>>>(s_bindv);      \
      if (s_bind_##type##n) {                                                       \
        auto &bind_##type##n = *s_bind_##type##n;                                      \
        sd_host->uniform(bind_##type##n.var_name(), bind_##type##n._value.value);            \
      }
      UNFORM_ARRAY(float,2)
      UNFORM_ARRAY(float,3)
      UNFORM_ARRAY(float,4)
      UNFORM_ARRAY(float,6)
      UNFORM_ARRAY(float,8)
      UNFORM_ARRAY(float,16)
      auto s_bind_texture_key =
          dynamic_pointer_cast<bind_var_t<texture_key>>(s_bindv); 
      if (s_bind_texture_key) {
        auto isd_txt = texture_map_.find(s_bind_texture_key->_value);
        if (isd_txt != texture_map_.end()) {
          auto &itext = *isd_txt->second;
          glActiveTexture(GL_TEXTURE0 + texture_cnt);
          glBindTexture(GL_TEXTURE_2D,itext._txt_id);
          sd_host->uniform(s_bind_texture_key->_var_name, texture_cnt);
          texture_cnt++;
        }
      }
    }

    //sd_data->enableVertex();
    GLuint dml = draw_model[_pt.draw_mode];
    sd_data->draw(dml);
  }
}