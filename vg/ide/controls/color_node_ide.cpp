#include "color_node.h"
#include "user_control_imgui.h"
namespace vg {
void color_node::ex_init_fun() {
  reg_property_handle(&_pt, _pt._attached_obj, [this]() {
    if (_ps_prm) {
      ImGui::Text("Attached primitive:%s", _pt._attached_obj);
      ImGui::SameLine();
      if (ImGui::Button("Delink##primitive")) {
        _ps_prm = nullptr;
      }
    } else {
      ImGui::InputText("Attached primitive:", _pt._attached_obj, FILE_NAME_LEN);
      if (ImGui::Button("Import")) {
        auto itxt = vertex::_mp_vertex_base_units.find(_pt._attached_obj);
        if (itxt != vertex::_mp_vertex_base_units.end()) {
          _ps_prm = itxt->second;
        }
      }
    }
  });
  reg_property_handle(&_pt, _pt._trans_mat4x4, [this]() {
    ImGui::Text("transformation:");
    float *pf = _pt._trans_mat4x4;
    ImGui::InputFloat4("1st vec4:", pf);
    ImGui::InputFloat4("2nd vec4:", pf + 4);
    ImGui::InputFloat4("3rd vec4:", pf + 8);
    ImGui::InputFloat4("4th vec4:", pf + 12);
  });
}
} // namespace vg
