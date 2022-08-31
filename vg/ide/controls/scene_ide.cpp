#include "scene.h"
#include "user_control_imgui.h"
#include "common_functions.h"
namespace vg{
  void scene::ex_init_fun(){
    reg_property_handle(&_sn_pt, &_sn_pt._bk_clr, [this]() {
    ImGui::ColorEdit4("Color of background", (float *)&_sn_pt._bk_clr,
                      ImGuiColorEditFlags_RGB);
    if (!_fboId) {
      if (ImGui::Button("Create frame buffer")) {
        prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, size().x,
                    size().y);
      }
    } else {
      if (ImGui::Button("Recreate frame buffer")) {
        release_resource();
        prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, size().x,
                    size().y);
      }
    }
  });
  }
}