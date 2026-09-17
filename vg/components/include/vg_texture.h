#pragma once
#include "vg_type.h"
#include "macro_tool.h"

namespace vg {
struct sub_txt {
  std::string item_name;
  float _x0, _y0, _x1, _y1;
  float width() { return _x1 - _x0; }
  float height() { return _y1 - _y0; }
};
using s_sub_txt=std::shared_ptr<sub_txt>;
using v_s_sub_txt=std::vector<s_sub_txt>;
struct texture {
  
  u32 _txt_id{0};
  u32 _width, _height;
  //bool separated = {false};
  bool mipmap = {false};
  v_s_sub_txt _v_sd_sub_txt;
  virtual ~texture();
};
DEF_TYPE_DIC(texture)
} // namespace vg