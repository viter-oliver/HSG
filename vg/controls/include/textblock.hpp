#pragma once
#include "control_fac.h"
#include "font.h"
namespace vg {
class AFG_EXPORT textblock : public control_base {
  DEF_STRUCT_WITH_INIT(_txt_pt, 
  (vec4, _txt_clr),
  (float, _txt_alignh_nml, {0.f}),
  (float, _txt_alignv_nml, {0.f}),
  (float, _width, {200.0f}), 
  (char_50, _content, {0}),
  (bool, _width_limit, {false}), 
  (int, _omit_rest, {0}),
  (bool, _playing, {false}), 
  (float, _play_circle, {1.0f}),
  (font_unit_key, _font_key, {0}),
  (int, _face_id, {0}), 
  (int, _font_size, {16}),
  (float, _font_scale, {1.f}),
  (float ,_font_line_offset,{0.0f}))
  
  area_f _txt_area;
  sd_font_unit _pfont_unit;
  std::wstring _str_bk;
  // shared_ptr<vg_font_res_set> _pfont_res_set;
public:
  textblock(); // : _txt_pt(), _txt_area(0.f, 0.f, 0.f, 0.f){}
  void draw();
  void set_content(const char *strct) { strcpy(_txt_pt._content.value, strct); }
  const char *content() { return _txt_pt._content.value; }
  
  void set_txt_clr(float r, float g, float b) {
    _txt_pt._txt_clr.x() = r;
    _txt_pt._txt_clr.y() = g;
    _txt_pt._txt_clr.z() = b;
  }
  bool contain(vec2& tar_pos) {
     return _txt_area.contain(tar_pos);
  }
  DECLARE_EX_INT
  DECLARE_DRAW_SEL_ANCHOR
};
REG_CTL_2_FAC(textblock);
} // namespace vg