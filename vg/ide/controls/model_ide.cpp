#include "model.hpp"
#include "user_control_imgui.h"
#include "imgui_internal.h"
namespace vg {
const char *str_trans_order[en_trans_order_cnt] = {
    "translate_scale_rotate", "translate_rotate_scale",
    "scale_translate_rotate", "scale_rotate_translate",
    "rotate_translate_scale", "rotate_scale_translate",
};
const char *str_rotate_oder[en_rotate_order_cnt] = {
    "x_y_z", "x_z_y", "y_x_z", "y_z_x", "z_x_y", "z_y_x",
};
void ctrl_model::ex_init_fun() {
  _user_properties_handlor[_trans_order] = [&] { 
    ImGui::Combo("Order of transform:",&_pt._trans_order, str_trans_order,
                 en_trans_order_cnt);
  };
  _user_properties_handlor[_rotate_order] = [&] {
    ImGui::Combo("Order of rotation:", &_pt._rotate_order, str_rotate_oder,
                 en_rotate_order_cnt);
  };
}
} // namespace vg