#include "scene.hpp"
#include "user_control_imgui.h"
#include "imgui_internal.h"
namespace vg
{
  void scene::ex_init_fun(){
    control_base::ex_init_fun();
    _user_properties_handlor[_view_pos]=[&]{
        enum { ig_x, ig_y, ig_z, ig_count };
      static int ignore_current = ig_z;
      const char* ignore_names[] = {"X","Y","Z"};
      ImGui::Text("2D View");
      ImGui::SameLine();
      for(int ix=0;ix<ig_count;ix++){
        if(ignore_current == ix){
          ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.8f,0.2f,0.2f,1.0f));
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.9f,0.3f,0.3f,1.0f));
          ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.8f,0.2f,0.2f,1.0f));
        }
        if(ImGui::Button(ignore_names[ix])){
          ignore_current = ix;
        }
        if(ignore_current == ix){
          ImGui::PopStyleColor(3);
        }
      }



      ImGui::SliderFloat3("View Position", (float *)&_pt._view_pos, -2000.0f, 2000.0f, "%.3f");                          
    };
  }
}