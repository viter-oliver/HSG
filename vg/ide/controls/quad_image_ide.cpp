#include "quad_image.h"
#include "user_control_imgui.h"
namespace vg{
  using namespace std;
  void quad_image::ex_init_fun() {
     reg_property_handle(&_img_pt, _img_pt._imge_pos, [this]() {
    static int pt_id = 0;
    ImGui::SliderInt("Select a point:", &pt_id, 0, 2);
    if (pt_id < 3) {
      ImGui::SliderFloat2("Selected point", &_img_pt._imge_pos[pt_id].x, -1920,
                          1920);
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 20));
    ImVec2 chd_wd_sz=to_imvec2(size());
    ImGui::BeginChild("canvas", chd_wd_sz, true,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
    ImVec2 img_pos0 = { 0,0 };
    auto img_pos1 = to_imvec2(_img_pt._imge_pos[0]);
    auto img_pos2 = to_imvec2(_img_pt._imge_pos[1]);
    auto img_pos3 = to_imvec2(_img_pt._imge_pos[2]);
    draw_list->AddLine(origin + img_pos0,
                       origin + img_pos1, GRID_COLOR);
    draw_list->AddLine(origin + img_pos1,
                       origin + img_pos2, GRID_COLOR);
    draw_list->AddLine(origin + img_pos2,
                       origin + img_pos3, GRID_COLOR);
    draw_list->AddLine(origin + img_pos0,
                       origin + img_pos3, GRID_COLOR);
    ImU32 pt_col = IM_COL32(200, 150, 150, 250);
    const float pt_radius = 4.;
    string btn_cap = "btn";
    for (int ix = 0; ix < 3; ix++) {
      ImVec2 pos_d = origin + to_imvec2(_img_pt._imge_pos[ix]);
      draw_list->AddCircleFilled(pos_d, pt_radius, pt_col);
      ImVec2 btn_pos = pos_d - ImVec2(4., 4.);
      ImGui::SetCursorScreenPos(btn_pos);
      btn_cap += "_b";
      ImGui::InvisibleButton(btn_cap.c_str(), ImVec2(8., 8.));
      if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        _img_pt._imge_pos[ix] =
            _img_pt._imge_pos[ix] + to_vec2(ImGui::GetIO().MouseDelta);
      }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
  });
  }
  void quad_image::draw_sel_anchor() {
      ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
      auto abpos = absolute_coordinate_of_base_pos();
      ImVec2 winpos = ImGui::GetWindowPos();
      ImVec2 pos1 = winpos + abpos;
      ImVec2 pos2 = pos1 + _img_pt._imge_pos[0];
      ImVec2 pos3 = pos1 + _img_pt._imge_pos[1];
      ImVec2 pos4 = pos1 + _img_pt._imge_pos[2];
      ImVec2 editunit(edit_unit_len, edit_unit_len);
      ImVec2 pos1a = pos1 - editunit;
      ImVec2 pos1b = pos1 + editunit;
      ImGui::RenderFrame(pos1a, pos1b, col);
      ImVec2 pos2a = pos2 - editunit;
      ImVec2 pos2b = pos2 + editunit;
      ImGui::RenderFrame(pos2a, pos2b, col);
      ImVec2 pos3a = pos3 - editunit;
      ImVec2 pos3b = pos3 + editunit;
      ImGui::RenderFrame(pos3a, pos3b, col);
      ImVec2 pos4a = pos4 - editunit;
      ImVec2 pos4b = pos4 + editunit;
      ImGui::RenderFrame(pos4a, pos4b, col);
  }
}