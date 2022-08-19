#pragma once
#include "vg_type.h"
#include <map>
#include <memory>
#include <string>
namespace vg {
namespace texture {
struct texture_unit {
  u32 txt_id{-1};
  u32 _width, _height;
  bool _separated = {false},_mipmap={false};
  ~texture_unit();
};
using sd_texture_unit=std::shared_ptr<texture_unit>;
using mp_texture=std::map<std::string,sd_texture_unit>;
extern mp_texture _mp_textures;
} 
} // namespace vg