#include "material_ide.h"
#include "shader_container.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
using namespace std;  
namespace shader {
void edit_uniform_variable(sd_shader_variable puniform_variable) {
  auto punf_v = puniform_variable.get();
  if (typeid(shader_variable_T<float>) == typeid(*punf_v)) {
    auto punf_real = static_cast<shader_variable_T<float> *>(punf_v);
    float* pdata = (float*)punf_real->phead_data();
    auto buf_len = punf_real->get_buff_len();
    for (u32 ix = buf_len; ix > 0;) {
      if (ix >= 4) {
        ImGui::InputFloat4("##", pdata);
        ix -= 4;
        pdata += 4;
      } else {
        auto itrim = ix % 4;
        switch (itrim) {
        case 1:
          ImGui::InputFloat("##", pdata);
          return;
        case 2:
          ImGui::InputFloat2("##", pdata);
          return;
        case 3:
          ImGui::InputFloat3("##", pdata);
          return;
        }
      }
    }

  } else if (typeid(shader_variable_T<int>) == typeid(*punf_v) ||
             typeid(shader_variable_T<u32>) == typeid(*punf_v)) {
    auto punf_real = static_cast<shader_variable_T<int> *>(punf_v);
    int* pdata = (int*)punf_real->phead_data();
    auto buf_len = punf_real->get_buff_len();
    for (u32 ix = buf_len; ix > 0;) {
      if (ix >= 4) {
        ImGui::InputInt4("##", pdata);
        ix -= 4;
        pdata += 4;
      } else {
        auto itrim = ix % 4;
        switch (itrim) {
        case 1:
          ImGui::InputInt("##", pdata);
          return;
        case 2:
          ImGui::InputInt2("##", pdata);
          return;
        case 3:
          ImGui::InputInt3("##", pdata);
          return;
        }
      }
    }

  } else if (typeid(shader_variable_T<double>) == typeid(*punf_v)) {
    auto punf_real = static_cast<shader_variable_T<double> *>(punf_v);
  }
}

void material_ide::edit_item(){
  for(auto& sd_v:_uniform_variales){
    ImGui::Text("%s",sd_v.first.c_str());
    edit_uniform_variable(sd_v.second);
  }
}

}
void create_item(mp_sd_material_ide& material_list){
    static char ref_shader_name[FILE_NAME_LEN] = { 0 };
    static char material_name[FILE_NAME_LEN]={0};
    static shader::sd_shader_base pref_shader = nullptr;
    static bool first_call = true;
    if (first_call) {
        ref_shader_name[0] = 0;
        material_name[0]=0;
        pref_shader = nullptr;
        first_call = false;
    }
    ImGui::InputText("Name of shader referenced by the material", ref_shader_name, FILE_NAME_LEN);
    ImGui::SameLine();
    if (ImGui::Button("...##")) {
        auto i_shd = shader::_mp_sd_shder_def.find(ref_shader_name);
        if (i_shd != shader::_mp_sd_shder_def.end()) {
            pref_shader = i_shd->second;
        }
    }
    ImGui::InputText("Name of new material",material_name,FILE_NAME_LEN);
    bool be_enabled = pref_shader != nullptr ;
    if (!be_enabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    if (ImGui::Button("OK")) {
        string material_test_name = material_name;
        while (material_list.find(material_test_name) != material_list.end()) {
            material_test_name += "_";
        }
        auto new_mtl = make_shared<shader::material_ide>(pref_shader);
        new_mtl->_shader_name = ref_shader_name;
        material_list[material_test_name] = new_mtl;
        pref_shader = nullptr;
        first_call = true;
        ImGui::CloseCurrentPopup();
    }
    if (!be_enabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }
}
} // namespace vg