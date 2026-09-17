#include  "shader_control.h"
#include "user_control_imgui.h"
#include "imgui_internal.h"
namespace vg{
void shader_control::ex_init_fun() {
  _user_properties_handlor[draw_mode] = [&] {
    ImGui::Combo("Draw model:", &_pt.draw_mode, str_draw_model, en_gl_count);
  };
}
}