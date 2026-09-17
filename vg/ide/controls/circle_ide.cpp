#include "circle.hpp"
#include "user_control_imgui.h"
#include "imgui_internal.h"
#include <regex>
namespace vg {
using namespace std;
void circle::ex_init_fun() {
  _draw_focus_handle = [&]() {
    auto base_draw_pos = get_draw_pos();
    base_draw_pos.x() -= _pt._radius_f1s;
    base_draw_pos.y() -= _pt._radius_f1s;
    auto delta = _pt._radius_f1s * 2;
    ImVec2 pos1 = to_imvec2(base_draw_pos);
    ImVec2 pos2 = {pos1.x, pos1.y + delta};
    ImVec2 pos3 = {pos1.x + delta, pos1.y + delta};
    ImVec2 pos4 = {pos1.x + delta, pos1.y};
    ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);

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
  };
}
} // namespace vg