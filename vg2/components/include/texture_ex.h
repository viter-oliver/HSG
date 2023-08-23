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
DEFINE_SMART(sub_txt)
struct texture_ex {
  
  u32 txt_id{0};
  u32 _width, _height;
  bool separated = {false};
  bool mipmap = {false};
  v_sd_sub_txt _v_sd_sub_txt;
  virtual ~texture_ex();
};
} // namespace vg