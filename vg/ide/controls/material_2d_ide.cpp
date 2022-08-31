#include "material_2d.h"
#include "user_control_imgui.h"
namespace vg {
using namespace std;
void material_2d::ex_init_fun() {
  reg_property_handle(&_pt, &_pt._draw_mode, [this]() {
    ImGui::Combo("Draw mode:", &_pt._draw_mode, draw_mode, en_gl_count);
  });
  reg_property_handle(&_pt, _pt._primitive_name, [this]() {
    if (_ps_prm) {
      ImGui::Text("Primitive object:%s", _pt._primitive_name);
      ImGui::SameLine();
      if (ImGui::Button("Delink##prm_obj")) {
        _ps_prm = nullptr;
        _matched = false;
      }
    } else {
      ImGui::InputText("Primitive object:", _pt._primitive_name, FILE_NAME_LEN);
      if (ImGui::Button("Link##prm")) {
        auto &iprm = g_primitive_list.find(_pt._primitive_name);
        if (iprm != g_primitive_list.end()) {
          _ps_prm = iprm->second;
        } else {
          string str_prm_nm(_pt._primitive_name);
          if (ref_a_intenal_primitive(str_prm_nm)) {
            _ps_prm = g_primitive_list[str_prm_nm];
          }
        }
      }
    }
  });
  reg_property_handle(&_pt, _pt._material_name, [this]() {
    if (_ps_mtl) {
      ImGui::Text("Material name:%s", _pt._material_name);
      ImGui::SameLine();
      if (ImGui::Button("Delink##material")) {
        _ps_mtl = nullptr;
        _matched = false;
      }
    } else {
      ImGui::InputText("Material name:", _pt._material_name, FILE_NAME_LEN);
      if (ImGui::Button("Link##mtl")) {
        auto &imtl = g_material_list.find(_pt._material_name);
        if (imtl != g_material_list.end()) {
          _ps_mtl = imtl->second;
        }
      }
    }
    if (!_matched) {
      ImGui::Text("material is unmatched to primitive object");
    }
  });
}
} // namespace vg