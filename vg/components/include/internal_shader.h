#pragma once
#include "shader.h"
#include "shader_source_code.h"
namespace vg{
namespace shader{
inline shader_base_map intenal_shader_;
inline void initialize_internal_shader(){
  auto produce_shader = [](const char *vs_code, const char *fs_code) {
    auto sd_sb = std::make_shared<shader_base>();
    sd_sb->build_vs_code(vs_code);
    sd_sb->build_fs_code(fs_code);
    sd_sb->link();
    return sd_sb;
  };
#define CREARE_SHADER(shader_name, vs_code, fs_code)                           \
  shader_base_key tk_##shader_name = {#shader_name};                           \
  intenal_shader_[tk_##shader_name] = produce_shader(vs_code, fs_code);
  CREARE_SHADER(shader_1_txt, modeling_vs_1_txt, modeling_fs_1_txt)
  CREARE_SHADER(shader_col, modeling_vs_col_d, modeling_fs_col_d)
}
}
}