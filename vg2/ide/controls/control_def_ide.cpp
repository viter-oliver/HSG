#include "control_def_ide.h"
#include "user_control_imgui.h"
#include <regex>
namespace vg {
using namespace std;


void control_def_ide::ex_init_fun() {
 
  _after_draw_handle = [&]() {
    draw_sel_anchor();
    
  };
}

void control_def_ide::draw_propertys() {
  int pgidx = 0;
  string type_name = typeid(*this).name();
  string type_show = type_name.substr(sizeof("class vg::"));
  ImGui::Text("Type name:%s", type_show.c_str());
  for (auto &s_bv : _v_bind_var) {
    s_bv->edit();
  }
}

void control_def_ide::draw_outline() {
  auto winpos = ImGui::GetWindowPos();
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 pos[4];
  string cur_cname = typeid(*this).name();
  cur_cname = cur_cname.substr(sizeof("class autofuture::"));
  ImVec2 asz = to_imvec2(size());

  pos[0] = winpos + ImVec2(abpos.x, abpos.y);
  pos[1] = pos[0] + ImVec2(asz.x, 0.f);
  pos[2] = pos[0] + asz;
  pos[3] = pos[0] + ImVec2(0.f, asz.y);
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  float alpha = 1.f;
  if (!be_seen()) {
    alpha = 0.4f;
  }

  ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.7, 0.7, 0.7, alpha));
  float thickness = 0.8f;
  col = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 0, 0, alpha));

  draw_list->AddPolyline(pos, 4, col, true, thickness);
  for (auto &bc : _vchilds) {
    bc->draw_outline();
  }
}
void control_def_ide::draw_sel_anchor() {}
void control_def_ide::draw_corners() {
  ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 winpos = ImGui::GetWindowPos();
  ImVec2 pos1 = winpos + ImVec2(abpos.x, abpos.y);
  ImVec2 pos2 = {pos1.x, pos1.y + size().y};
  ImVec2 pos3 = {pos1.x + size().x, pos1.y + size().y};
  ImVec2 pos4 = {pos1.x + size().x, pos1.y};

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
} // namespace vg