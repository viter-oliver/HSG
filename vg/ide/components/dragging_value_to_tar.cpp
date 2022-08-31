#include <map>
#include "dragging_value_to_tar.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
namespace dragging_value_to_tar {
ptr_dragging_value pcur_dragging_value = nullptr;
using mp_accept_fun=std::map<int,accept_fun>;
mp_accept_fun accept_fun_list;
static bool dragging = false, hit_target = false;
static int _key=-1,_subkey=-1;
static int key_base=0;
void reg_a_value(ptr_dragging_value pvalue){
  pcur_dragging_value=pvalue;
}
void try_dragging_a_value() {
  if (ImGui::IsItemActive()) {
    // Draw a line between the button and the mouse cursor
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRectFullScreen();
    ImGuiIO &io = ImGui::GetIO();
    draw_list->AddLine(io.MouseClickedPos[0], io.MousePos,
                       ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
    draw_list->PopClipRect();
    dragging = true;
  }

  if (ImGui::IsMouseReleased(0)) {
    dragging = false;
    if (pcur_dragging_value && hit_target) {
      auto iacc_fun=accept_fun_list.find(_key);
      if(iacc_fun!=accept_fun_list.end()){
        iacc_fun->second(pcur_dragging_value,_subkey);
      }
    }
    pcur_dragging_value = nullptr;
    hit_target = false;
  }
}
int reg_accept_fun(accept_fun acc_fun) {
  accept_fun_list[key_base++]=acc_fun;
  return key_base;
}
void report_hit(int key,int subkey){
  _key=key;
  _subkey=subkey;
  hit_target = dragging && ImGui::IsItemHovered();
}
} // namespace dragging_value_to_tar
} // namespace vg