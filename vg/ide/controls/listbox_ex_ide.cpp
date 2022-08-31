#include "bind.h"
#include "listbox_ex.h"
#include "user_control_imgui.h"
namespace vg {

void listbox_ex::ex_init_fun() {
  reg_property_handle(&_pt, &_pt._vertical, [this]() {
    if (ImGui::Checkbox("vertical", &_pt._vertical)) {
      if (_pt._vertical) {
        _pt._inner_top = 0;
      } else {
        _pt._inner_left = 0;
      }
    }
  });
}
void listbox_ex::draw_sel_anchor() { draw_corners(); }
} // namespace vg