#pragma once
#include "control_fac.h"
#include "font_manager.h"
namespace vg {
class AFG_EXPORT textblock : public control_base {
#define MAX_CONTENT_LEN 0x100
  DEF_STRUCT_WITH_INIT(intl_pt, _txt_pt, (vec4, _txt_clr),
                       (float, _txt_alignh_nml, {0.f}),
                       (float, _txt_alignv_nml, {0.f}),
                       (float, _width, {200.0f}),
                       (char, _content[MAX_CONTENT_LEN]),
                       (bool, _width_limit, {false}), (int, _omit_rest, {0}),
                       (bool, _playing, {false}), (float, _play_circle, {1.0f}),
                       (int, _font_id, {0}), (int, _font_size, {16}),
                       (float, _font_scale, {1.f}))
  area_f _txt_area;
  ps_font_unit _pfont_unit;
  std::wstring _str_bk;
  // shared_ptr<vg_font_res_set> _pfont_res_set;
public:
  textblock(); // : _txt_pt(), _txt_area(0.f, 0.f, 0.f, 0.f){}
  void link();
  void draw();
  void set_content(const char *strct) { strcpy(_txt_pt._content, strct); }
  const char *content() { return _txt_pt._content; }
  area_f &get_cover_area() { return _txt_area; }
  void set_txt_clr(float r, float g, float b) {
    _txt_pt._txt_clr.x = r;
    _txt_pt._txt_clr.y = g;
    _txt_pt._txt_clr.z = b;
  }
  bool contain(vec2 &tar_pos);
};
REG_CTL_2_FAC(textblock);
} // namespace vg