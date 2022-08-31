#include "common_functions.h"
#include "hud_projector.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
	void hud_projector::ex_init_fun() {
        __super::ex_init_fun();
        reg_property_handle(&_pj_pt, &_pj_pt._bk_clr, [this]()
        {
            ImGui::ColorEdit4("Color of background", (float*)&_pj_pt._bk_clr, ImGuiColorEditFlags_RGB);
            if (!_fboId)
            {
                if (ImGui::Button("Create frame buffer"))
                {
                    prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh);
                }
            }
            else
            {
                if (ImGui::Button("Recreate frame buffer"))
                {
                    release_resource();
                    prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _in_p._sizew, _in_p._sizeh);
                }
            }
        });
	}
    void hud_projector::draw_sel_anchor() {
        draw_corners();
    }
}