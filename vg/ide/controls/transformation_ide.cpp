#include "transformation.h"
#include "user_control_imgui.h"
namespace vg {
const char *str_trans_order[en_trans_order_cnt] = {
    "translate_scale_rotate", "translate_rotate_scale",
    "scale_translate_rotate", "scale_rotate_translate",
    "rotate_translate_scale", "rotate_scale_translate",
};
const char *str_rotate_oder[en_rotate_order_cnt] = {
    "x_y_z", "x_z_y", "y_x_z", "y_z_x", "z_x_y", "z_y_x",
};
void transformation::ex_init_fun() {
  reg_property_handle(&_pt, &_pt._trans_order, [this]() {
    ImGui::Combo("transform order:", &_pt._trans_order, str_trans_order,
                 en_trans_order_cnt);
  });

  reg_property_handle(&_pt, &_pt._rotate_order, [this]() {
    ImGui::Combo("rotate order:", &_pt._rotate_order, str_rotate_oder,
                 en_rotate_order_cnt);
  });
}
} // namespace vg