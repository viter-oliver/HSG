#include "textblock.h"
#include "user_control_imgui.h"

namespace vg {
static bool get_font_item(void *data, int idx, const char **out_str) {
  auto &nm_list = font_manager::get_dic_fonts();
  *out_str = nm_list[idx]->_name.c_str();

  return true;
}
static const char *omit_show[en_omit_type_number] = {
    "no omit",
    "omit rest",
    "omit rest with ellipsis",
};
void textblock::ex_init_fun() {
  reg_property_handle(&_txt_pt, &_txt_pt._omit_rest, [this]() {
    ImGui::Combo("omit:", &_txt_pt._omit_rest, omit_show, en_omit_type_number);
  });
  reg_property_handle(&_txt_pt, &_txt_pt._font_id, [this]() {
    auto &nm_list = font_manager::get_dic_fonts();
    if (ImGui::Combo("font:", &_txt_pt._font_id, &get_font_item, 0,
                     nm_list.size())) {
      _pfont_unit = nm_list[_txt_pt._font_id];
    }
  });
  reg_property_handle(&_txt_pt, &_txt_pt._font_size, [this]() {
    ImGui::SliderInt("Font size", &_txt_pt._font_size, 8, 60);
  });
  reg_property_handle(&_txt_pt, &_txt_pt._font_scale, [this]() {
    ImGui::DragFloat("Font scale", &_txt_pt._font_scale, 0.005f, 1.f, 10.0f,
                     "%.1f"); // Scale only this font
  });
}
} // namespace vg