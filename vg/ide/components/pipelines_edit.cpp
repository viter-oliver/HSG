
#include <windows.h>
#include <locale.h>  
#include <ShlObj.h>
#include <Commdlg.h>
#include <numeric>
#include <chrono>
#include <imgui_internal.h>
#include "user_control_imgui.h"
#include <imgui_node_editor.h>
#include "pipelines_edit.hpp"
#include "pipeline.h"
#include "ide_events.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/quaternion.hpp>
#include "TextureHelper.h"
#include "common_functions.h"
#include "shader_source_code.h"
#include "components_fun_declaration.h"
extern HINSTANCE g_cur_instance;
//#include "components.h"
//#include <imgui_node_editor.h>
namespace vg{
    inline ImVec2 operator+(const ImVec2& imPt, const ImVec2& afPt) {
        return ImVec2(imPt.x + afPt.x, imPt.y + afPt.y);
    }
    inline ImVec2 operator-(const ImVec2& imPt, const ImVec2& afPt) {
        return ImVec2(imPt.x - afPt.x, imPt.y - afPt.y);
    }

    

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}
namespace ed = ax::NodeEditor;
ed::EditorContext* m_Context = nullptr;
int selected_node_index = 0;
int get_node_id() {
static int _base_node_id = 1;     
return _base_node_id++;
}
struct node {
    int node_id{0};
    std::vector<int> input_link_list;
    std::vector<int> output_link_list;
};
std::vector<node> node_list;
int get_node_index_by_id(ed::NodeId node_id) {
    for (int ix = 0; ix < node_list.size(); ix++) {
		ed::NodeId current_node_id = node_list[ix].node_id;
        if (current_node_id == node_id) {
            return ix;
        }
    }
    return -1;
}
void init_node_list() {
    auto calcu_list_size = v_sd_calculate_unit_.size();
    node_list.resize(calcu_list_size);
    for(int ix=0;ix<calcu_list_size; ix++){
		auto& sd_calcu_unit_ = v_sd_calculate_unit_[ix];
          
        auto& calcu_unit=*sd_calcu_unit_;
        node_list[ix].node_id = get_node_id();
		  
        auto in_link_count=calcu_unit.input_count();
        for(int iy=0;iy<in_link_count;iy++){
            node_list[ix].input_link_list.push_back(get_node_id());
        }
        auto out_link_count = calcu_unit.output_ids.size();
        for (int iy = 0; iy < out_link_count; iy++) {
            node_list[ix].output_link_list.push_back(get_node_id());
        }
    }
    for (auto& [key, val] : links_ex_) {
	    val.link_id = get_node_id();
    }
}
pipepline_edit::pipepline_edit(std::string& name)
    :base_component_edit(name) {
        ed::Config config;
        config.SettingsFile = "Widgets.json";
        m_Context = ed::CreateEditor(&config);
        ed::SetCurrentEditor(m_Context);
        register_ide_envets(after_open_project, [&]() {
            node_list.clear();
			init_node_list();
			});
    }  

 pipepline_edit::~pipepline_edit(){
     ed::DestroyEditor(m_Context);
  }
 void ImGuiEx_BeginColumn()
 {
     ImGui::BeginGroup();
 }

 void ImGuiEx_NextColumn()
 {
     ImGui::EndGroup();
     ImGui::SameLine();
     ImGui::BeginGroup();
 }

 void ImGuiEx_EndColumn()
 {
     ImGui::EndGroup();
 }
  void calculate_units_box(float paneWidth){
    ImGui::BeginChild("Calculate unit box", ImVec2(paneWidth, 0));
    auto& style = ImGui::GetStyle();
    float spacing = style.ItemSpacing.x;

    // 用于记录当前行已用宽度
    float line_used = 0.0f;
    bool first_in_line = true;
    calculate_unit_factory::instance().iterate_keys([&](std::string key) {
        // 计算控件所需的理论宽度（可替换成固定宽度）
        ImVec2 size = ImGui::CalcTextSize(key.c_str());
        size.x += style.FramePadding.x * 2.0f;
        size.y += style.FramePadding.y * 2.0f;

        float next_item_w = size.x;
        float need_space = (first_in_line ? 0.0f : spacing) + next_item_w;

        // 如果本行放不下，就换行
        if (!first_in_line && (line_used + need_space > paneWidth)) {
            //ImGui::NewLine();
            line_used = 0.0f;
            first_in_line = true;
        }
        else if (!first_in_line) {
            ImGui::SameLine();
        }

        // 绘制控件
        bool pressed = IconButton(key.c_str());
        if (!pressed) {
            ImGui::Button(key.c_str());
        }
        // 获取实际大小（有时 IconButton() 比理论宽度略不同）
        ImVec2 actual_size = ImGui::GetItemRectSize();
        line_used += (first_in_line ? 0.0f : spacing) + actual_size.x;
        first_in_line = false;
      if (ImGui::BeginDragDropSource()) {
        if (internal_image(key.c_str())) {
            ImGui::SameLine(50);
        }
        ImGui::Text(key.c_str());
        ImGui::SetDragDropPayload("Calculate unit box",
                                  key.c_str(),
                                  key.size());
        ImGui::EndDragDropSource();
      } 

      if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
        ImGui::SetTooltip(key.c_str());
      
    });
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Separator();
    int count = ed::GetSelectedNodes(nullptr, 0);
    if (count == 1&& selected_node_index< v_sd_calculate_unit_.size()) {
        ImGui::Text("Selected Node index: %d", selected_node_index);
        auto& sd_calcu_unit = v_sd_calculate_unit_[selected_node_index];
        bool tail_node_is_canvas = sd_calcu_unit->output_ids.size() == 0;
        if (tail_node_is_canvas) {
            auto s_cs = std::dynamic_pointer_cast<canvas>(sd_calcu_unit);
            auto& canvas_ = *s_cs;
			ImGui::ColorEdit4("Canvas Background Color", (float*)&canvas_._pt.background_clr);
            if (IconButton("slider_thumb")) {
				canvas_.release_resource();
				canvas_.create_fbo();
            }
        }else  if (sd_calcu_unit->is_pipeline()) {
            auto s_pl = std::dynamic_pointer_cast<pipeline>(sd_calcu_unit);
			auto& pipeline_ = *s_pl;
            ImGui::Combo("Draw model:", &pipeline_._pt.draw_model_id, str_draw_model, en_gl_count);
        }
        else {
            auto input_link_list_size = sd_calcu_unit->_v_bind_var.size();
            for (int iy = 0; iy < input_link_list_size; iy++) {
                auto& bind_unit = sd_calcu_unit->_v_bind_var[iy];
                bind_unit->edit();
            }
        }
    }
    ImGui::EndChild();
  }

  static ImVec4 type_color[type_value_number] = {
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f),//u8
    ImVec4(0.0f, 1.0f, 0.0f, 1.0f),//u16
    ImVec4(0.0f, 0.0f, 1.0f, 1.0f),//u32
    ImVec4(1.0f, 1.0f, 0.0f, 1.0f),//s8
    ImVec4(0.0f, 1.0f, 1.0f, 1.0f),//s16
    ImVec4(1.0f, 0.0f, 1.0f, 1.0f),//s32
    ImVec4(0.5f, 0.0f, 0.0f, 1.0f),//f32
    ImVec4(0.0f, 0.5f, 0.0f, 1.0f),//f64
    ImVec4(1.0f, 0.0f, 0.5f, 1.0f),//bool
    ImVec4(0.8f, 0.3f, 0.8f, 1.0f),//texture_value
  };
  void show_bind_var_value(std::string& var_name,void* pvalue,int type,int count){
      char buff[FILE_NAME_LEN];
      std::string value_str;
      switch (type) {
          case u8_value: {
              u8* pval = (u8*)pvalue;
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%u", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
		  }case u16_value: {
              u16* pval = (u16*)pvalue;
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%u", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
          }case u32_value: {
              u32* pval = (u32*)pvalue;
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u,%u", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%u", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%u,%u,%u", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
          }case s8_value: {
              s8* pval = (s8*)pvalue;
              if (50 == count) {
                  ImGui::Text("%s:%s", var_name.c_str(), pval);
              }
              else {
                  for (int ix = count; ix > 0;) {
                      memset(buff, 0, FILE_NAME_LEN);
                      if (ix > 4) {
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d\n", pval[0], pval[1], pval[2], pval[3]);
                          ix -= 4;
                          pval += 4;
                          value_str += buff;
                      }
                      else {
                          switch (ix)
                          {
                          case 0:
                              std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d", pval[0], pval[1], pval[2], pval[3]);
                              break;
                          case 1:
                              std::snprintf(buff, FILE_NAME_LEN, "%d", pval[0]);
                              break;
                          case 2:
                              std::snprintf(buff, FILE_NAME_LEN, "%d,%d", pval[0], pval[1]);
                              break;
                          case 3:
                              std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d", pval[0], pval[1], pval[2]);
                              break;
                          default:
                              break;
                          }
                          value_str += buff;
                          break;
                      }
                  }
                  ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              }
              
              return;
          }case s16_value: {
              s16* pval = (s16*)pvalue;
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%d", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
          }case s32_value: {
              s32* pval = (s32*)pvalue;            
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d,%d", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%d", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%d,%d,%d", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
                  
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
          }case f32_value: {
              float* pval = (float*)pvalue;
              std::string tail_str;
              get_tailstr(var_name.c_str(), tail_str, 4);
              if(tail_str=="_clr"){

				  ImGui::Text("%s:", var_name.c_str());
				  ImGui::SameLine();
                  auto font_sz = ImGui::GetFontSize();
                  ImVec2 imsz = { font_sz + 6, font_sz + 6 };
                  auto screen_pos = ImGui::GetCursorScreenPos();
                  auto draw_list = ImGui::GetWindowDrawList();
                  draw_list->AddRectFilled(screen_pos, screen_pos + imsz, IM_COL32_WHITE);
                  auto convert_vec4_to_col32 = [](const vec4& clr) {
                      return IM_COL32((u8)(clr.x() * 255.0f), (u8)(clr.y() * 255.0f), (u8)(clr.z() * 255.0f), (u8)(clr.w() * 255.0f));
                      };
				  vec4 clr_value = { *pval,*(pval + 1),*(pval + 2),1 };
                  if (count == 4) {
                      clr_value.w() = *(pval + 3);
                  }
				  ImU32 back_clr = convert_vec4_to_col32(clr_value);
                  screen_pos.x += 3;
                  screen_pos.y += 3;
                  imsz.x -= 6;
                  imsz.y -= 6;

                  draw_list->AddRectFilled(screen_pos, screen_pos + imsz, back_clr);
              }
              else {
                  for (int ix = count; ix > 0;) {
                      memset(buff, 0, FILE_NAME_LEN);
                      if (ix > 4) {
                          std::snprintf(buff, FILE_NAME_LEN, "%.3f,%.3f,%.3f,%.3f\n", pval[0], pval[1], pval[2], pval[3]);
                          ix -= 4;
                          pval += 4;
                          value_str += buff;
                      }
                      else {
                          switch (ix)
                          {
                          case 0:
                              std::snprintf(buff, FILE_NAME_LEN, "%.3f,%.3f,%.3f,%.3f", pval[0], pval[1], pval[2], pval[3]);
                              break;
                          case 1:
                              std::snprintf(buff, FILE_NAME_LEN, "%.3f", pval[0]);
                              break;
                          case 2:
                              std::snprintf(buff, FILE_NAME_LEN, "%.3f,%.3f", pval[0], pval[1]);
                              break;
                          case 3:
                              std::snprintf(buff, FILE_NAME_LEN, "%.3f,%.3f,%.3f", pval[0], pval[1], pval[2]);
                              break;
                          default:
                              break;
                          }
                          value_str += buff;
                          break;
                      }
                      
                  }
                  ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              }
              return;
          } case f64_value: {
              double* pval = (double*)pvalue;
              for (int ix = count; ix > 0;) {
                  memset(buff, 0, FILE_NAME_LEN);
                  if (ix > 4) {
                      std::snprintf(buff, FILE_NAME_LEN, "%lf,%lf,%lf,%lf\n", pval[0], pval[1], pval[2], pval[3]);
                      ix -= 4;
                      pval += 4;
                      value_str += buff;
                  }
                  else {
                      switch (ix)
                      {
                      case 0:
                          std::snprintf(buff, FILE_NAME_LEN, "%lf,%lf,%lf,%lf", pval[0], pval[1], pval[2], pval[3]);
                          break;
                      case 1:
                          std::snprintf(buff, FILE_NAME_LEN, "%lf", pval[0]);
                          break;
                      case 2:
                          std::snprintf(buff, FILE_NAME_LEN, "%lf,%lf", pval[0], pval[1]);
                          break;
                      case 3:
                          std::snprintf(buff, FILE_NAME_LEN, "%lf,%lf,%lf", pval[0], pval[1], pval[2]);
                          break;
                      default:
                          break;
                      }
                      value_str += buff;
                      break;
                  }
              }
              ImGui::Text("%s:", var_name.c_str()); ImGui::SameLine(); ImGui::Text("%s", value_str.c_str());
              return;
          } case bool_value: {
              bool* pval = (bool*)pvalue;
			  value_str = (*pval) ? "true" : "false";
              ImGui::Text("%s:%s", var_name.c_str(), value_str.c_str()); 
              return;
          } case texture_value: {
              ImGui::Text("%s:%s", var_name.c_str(), *(s8*)pvalue);
              return;
          }
      }
  }
  void edit_calcu_node(int id) {
      auto& sd_calcu_unit = v_sd_calculate_unit_[id];
      auto& nd = node_list[id];
      ImGui::PushItemWidth(100);
      if(ed::IsNodeSelected(nd.node_id)){
          selected_node_index=id;
	  }
      if (sd_calcu_unit->is_pipeline()) {
          auto sd_pp = std::dynamic_pointer_cast<pipeline>(sd_calcu_unit);
          auto& pipeline_=*sd_pp;

          ed::BeginNode(nd.node_id);
          ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "unit%d:pipeline", id);
          std::string name_label = "name##" + std::to_string(id);
          ImGui::InputText(name_label.c_str(), (char*)pipeline_._var_name.value, FILE_NAME_LEN);
          ImGui::PushID(nd.node_id);
          if (IconButton("shader_1_txt")) {
              int execute_count = 0;
              calculate_node_ex(id, execute_count);
          }
          ImGui::PopID();
          pipeline_._v_bind_var[0]->edit();
          pipeline_._v_bind_var[1]->edit();
          ImGui::Text("Draw mode:%s",str_draw_model[pipeline_._pt.draw_model_id]);
          link_key key_low{ id, std::numeric_limits<int>::min() };
          link_key key_high{ id, std::numeric_limits<int>::max() };
          auto it_low = links_ex_.lower_bound(key_low);
          auto it_high = links_ex_.upper_bound(key_high);
          bool disabled = (it_low != it_high);
          ImGui::BeginDisabled(disabled);
          std::string update_str= "update shader & vertex##"+ std::to_string(id);
          if(ImGui::Button(update_str.c_str())){
              pipeline_.update_shader_vertex();
              auto in_link_count = pipeline_.data_inputs.size();
              nd.input_link_list.clear();
              for (int iy = 0; iy < in_link_count; iy++) {
                  nd.input_link_list.push_back(get_node_id());
              }
              //input_link_list_size = nd.input_link_list.size();
		  }
          ImGui::EndDisabled();

          ImGuiEx_BeginColumn();
          auto input_link_list_size = nd.input_link_list.size();
          for (int iy = 0; iy < input_link_list_size; iy++) {
              auto& link_id = nd.input_link_list[iy];
              auto& data_inut_put = pipeline_.data_inputs[iy];
              int bind_type_value = 0, bind_value_count = 0;
              data_inut_put.second->value_info(bind_type_value, bind_value_count);
              std::string icon_str = "left_in";
              icon_str += std::to_string(bind_value_count);     
              ed::BeginPin(link_id, ed::PinKind::Input);
              if (!internal_image(icon_str.c_str(), type_color[bind_type_value])) {
                  icon_str += "->";
                  ImGui::Text(icon_str.c_str());
              }
              ImGui::SameLine();
              ImGui::Text(data_inut_put.first.c_str());
              ed::EndPin();
          }
          ImGuiEx_NextColumn();
          ImGui::Dummy(ImVec2(100, 0));
          ImGuiEx_NextColumn();
          auto& link_id = nd.output_link_list[0];
          ed::BeginPin(link_id, ed::PinKind::Output);
          internal_image("left_paint_in", type_color[texture_value]);
          ed::EndPin();
          ImGuiEx_EndColumn();
          ed::EndNode();
      }
      else {
          bool tail_node_is_canvas = sd_calcu_unit->output_ids.size() == 0;
          if (tail_node_is_canvas) {
              auto s_cs= std::dynamic_pointer_cast<canvas>(sd_calcu_unit);
              auto& canvas_ = *s_cs;
              ed::BeginNode(nd.node_id);
              ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.8f, 1.0f), "unit%d:canvas", id);
              std::string name_label = "name##" + std::to_string(id);
              ImGui::InputText(name_label.c_str(), (char*)canvas_._var_name.value, FILE_NAME_LEN);
              /**std::string button_str = "▲##" + std::to_string(nd.node_id);
              if (ImGui::Button(button_str.c_str())) {
                  int execute_count = 0;
                  calculate_node_ex(id, execute_count);
              }*/
              ImGui::PushID(nd.node_id);
              if (IconButton("shader_1_txt")) {
                  int execute_count = 0;
                  calculate_node_ex(id, execute_count);
              }
			  static bool debug_draw = false;

			  ImGui::Checkbox("Debug Draw", &debug_draw);
              if (debug_draw) {
                  int execute_count = 0;
                  calculate_node_ex(id, execute_count);
              }
              ImGui::PopID();

			  canvas_._v_bind_var[0]->edit(); 
			  canvas_._v_bind_var[1]->edit(); 
			  ImGui::Text("Background Color:"); ImGui::SameLine();
              auto font_sz = ImGui::GetFontSize();
              ImVec2 imsz = { font_sz+6, font_sz+6 };
			  auto screen_pos = ImGui::GetCursorScreenPos();
			  auto draw_list = ImGui::GetWindowDrawList();
			  draw_list->AddRectFilled(screen_pos, screen_pos + imsz, IM_COL32_WHITE);
              auto convert_vec4_to_col32 = [](const vec4& clr) {
                  return IM_COL32((u8)(clr.x() * 255.0f), (u8)(clr.y() * 255.0f), (u8)(clr.z() * 255.0f), (u8)(clr.w() * 255.0f));
				  };
			  auto back_clr = convert_vec4_to_col32(canvas_._pt.background_clr);
			  screen_pos.x += 3;
			  screen_pos.y += 3;
              imsz.x -= 6;
			  imsz.y -= 6;

			  draw_list->AddRectFilled(screen_pos, screen_pos + imsz, back_clr);
			  ImGui::Dummy(imsz);
              ImGuiEx_BeginColumn();
              auto input_link_list_size = nd.input_link_list.size();
              for (int iy = 0; iy < input_link_list_size; iy++) {
                  auto& link_id = nd.input_link_list[iy];
                  ed::BeginPin(link_id, ed::PinKind::Input);
                  internal_image("left_paint_in", type_color[texture_value]);
                  ed::EndPin();
              }
              ImGuiEx_NextColumn();
              ImGui::Dummy(ImVec2(20,0));
              ImGuiEx_NextColumn();
              ImGui::Dummy(ImVec2(0, 10));
			  //ImGui::Text("Canvas Output:");
              if (canvas_.texture_id > 0) {
                  ImVec2 img_size = {(float)canvas_._pt.frame_size.x(), (float)canvas_._pt.frame_size.y()};
				  ImGui::Image((ImTextureID)canvas_.texture_id, img_size);
              }
              ImGuiEx_EndColumn();
              ed::EndNode();
          }
          else {
              auto& calcu_unit=*sd_calcu_unit;
   
		      //std::string type_name = typeid(calcu_unit).name();
              ed::BeginNode(nd.node_id);
		      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"unit%d:%s",
                  id,calcu_unit._type_name.value);
			  std::string name_label = "name##" + std::to_string(id);
              ImGui::InputText(name_label.c_str(), (char*)calcu_unit._var_name.value, FILE_NAME_LEN);
              ImGui::PushID(nd.node_id);
              if (IconButton("shader_1_txt")) {
                  int execute_count = 0;
                  calculate_node_ex(id, execute_count);
              }
              ImGui::PopID();

              ImGuiEx_BeginColumn();

              auto input_link_list_size = nd.input_link_list.size();
              for (int iy = 0; iy < input_link_list_size; iy++) {
                  auto& link_id = nd.input_link_list[iy];
                  auto& bind_unit = calcu_unit._v_bind_var[iy];
                  int bind_type_value = 0, bind_value_count = 0;
                  bind_unit->value_info(bind_type_value, bind_value_count);
				  void* pvalue = nullptr;
				  bind_unit->get_void_ptr(pvalue); 
				  std::string var_name = bind_unit->var_name();
				  
                  //bind_unit->edit();
                  std::string icon_str = "left_in";
                  icon_str += std::to_string(bind_value_count);
                  ed::BeginPin(link_id, ed::PinKind::Input);             
                  internal_image(icon_str.c_str(), type_color[bind_type_value]);
                  ed::EndPin();
				  ImGui::SameLine();
                  show_bind_var_value(var_name, pvalue, bind_type_value, bind_value_count);
              }
              ImGuiEx_NextColumn();
              ImGui::Dummy(ImVec2(100, 0));
              ImGuiEx_NextColumn();
              
              //float max_width = ImGui::CalcTextSize("这是最宽的文字或控件").x;
              auto output_link_list_size = nd.output_link_list.size();
              for (int iy = 0; iy < output_link_list_size; iy++) {
                  auto& value_id = calcu_unit.output_ids[iy];  
                  ImGui::Text(value_id->name.c_str()); 
              }
              ImGuiEx_NextColumn();
              for (int iy = 0; iy < output_link_list_size; iy++) {
                  auto& link_id = nd.output_link_list[iy];
                  auto& value_id = calcu_unit.output_ids[iy];
                  int type_value = 0, value_count = 0;
                  value_id->value_info(type_value, value_count);
                  ed::BeginPin(link_id, ed::PinKind::Output);
                  std::string icon_str = "left_in";
                  icon_str += std::to_string(value_count);
                  internal_image(icon_str.c_str(), type_color[type_value]);
                  //ImGui::SameLine();
                  ed::EndPin();
              }
              
              ImGuiEx_EndColumn();
              ed::EndNode();
          }
          
      }
      ImGui::PopItemWidth();
  }
  void before_node_view() {
      const ImGuiWindowFlags windowFlags=
              ImGuiWindowFlags_NoTitleBar |
              ImGuiWindowFlags_NoResize |
              ImGuiWindowFlags_NoMove |
              ImGuiWindowFlags_NoScrollbar |
              ImGuiWindowFlags_NoScrollWithMouse |
              ImGuiWindowFlags_NoSavedSettings |
              ImGuiWindowFlags_NoBringToFrontOnFocus;
      const auto windowBorderSize = ImGui::GetStyle().WindowBorderSize;
      const auto windowRounding = ImGui::GetStyle().WindowRounding;
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::Begin("Content", nullptr);//, windowFlags);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, windowBorderSize);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);
  }  
  void after_node_view() {
      ImGui::PopStyleVar(2);
      ImGui::End();
      ImGui::PopStyleVar(2);
  }
  void add_new_calcu_unit(sd_calculate_unit new_cal_u) {
      v_sd_calculate_unit_.push_back(new_cal_u);
      auto& ref_nd = node_list.emplace_back();
      ref_nd.node_id = get_node_id();

      auto in_link_count = new_cal_u->input_count();
      for (int iy = 0; iy < in_link_count; iy++) {
          ref_nd.input_link_list.push_back(get_node_id());
      }
      auto out_link_count = new_cal_u->output_ids.size();
      for (int iy = 0; iy < out_link_count; iy++) {
          ref_nd.output_link_list.push_back(get_node_id());
      }
      auto node_position=ImGui::GetMousePos();
      ed::SetNodePosition(ref_nd.node_id, node_position);
  }
  void pipepline_edit::show() {
    if (!_visible) {
      return;
	}
    before_node_view();
    
    static float leftPaneWidth  = 685.0f;
    static float rightPaneWidth = 800.0f;
    Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
    if (ImGui::Button("Show Flow"))
    {
        /*
        for (auto& link : links_) {
            ed::Flow(link.link_id);
        }*/
        for (auto& [key,value] : links_ex_) {
            ed::Flow(value.link_id);
        }
    }
   calculate_units_box(leftPaneWidth - 4.0f);
   //calculate_units_property(leftPaneWidth - 4.0f);
   ImGui::SameLine(0.0f, 12.0f);
    ed::Begin("Pipeline Canvas");
    {
      //auto cursorTopLeft = ImGui::GetCursorScreenPos();
        if (ImGui::BeginDragDropTarget()) {
        //cc_print("accept drag");
            if (const ImGuiPayload* payload =
                ImGui::AcceptDragDropPayload("Calculate unit box")) {
                std::string calcu_name;
                calcu_name.resize(payload->DataSize);
                memcpy_s(&calcu_name[0], payload->DataSize, payload->Data,
                    payload->DataSize);
                auto sd_cu = produce_sd_calculate_unit(calcu_name);
                if (sd_cu) {
					sd_cu->_type_name = calcu_name;
                    auto unit_id = v_sd_calculate_unit_.size();
					std::string unit_name = "unit" + std::to_string(unit_id);
                    int suffix = 1;
                    do {
                        unit_name = unit_name + "_(" + std::to_string(suffix) + ")";
                        suffix++;
                    } while (std::any_of(v_sd_calculate_unit_.begin(), v_sd_calculate_unit_.end(),
                        [&unit_name](const auto& cunit) {
							std::string tar_unit_name = cunit->unit_name();
                            return tar_unit_name == unit_name;
                        }));
                    sd_cu->set_name(unit_name);
                    add_new_calcu_unit(sd_cu);
                    
                }
            }
            ImGui::EndDragDropTarget();
        }
      auto calcu_list_size = v_sd_calculate_unit_.size();
      for(int ix=0;ix<calcu_list_size; ix++){
        
        edit_calcu_node(ix);
      }

	  static int frame_count = 0;
      frame_count++;
      for(auto& [key, val] : links_ex_) {
          auto link_id = val.link_id;
          auto pin_in = node_list[val.in_node_id].output_link_list[val.in_sub_id];
          auto pin_out = node_list[key.out_node_id].input_link_list[key.out_sub_id];
          auto& calcu_unit = *v_sd_calculate_unit_[val.in_node_id];
          auto& out_unit = calcu_unit.output_ids[val.in_sub_id];
          int bind_type_value = 0, bind_value_count = 0;
          out_unit->value_info(bind_type_value, bind_value_count);
          if (frame_count % 30 == 0) {
                if (val.thickness > 4) {
                    val.thickness--;
                }
          }
          
		  ed::Link(link_id, pin_in, pin_out, type_color[bind_type_value], val.thickness);
	  }
      if (frame_count % 50==0) {
         
      }
      frame_count %= 500;
      if (ed::BeginCreate()) {
          auto showLabel = [](const char* label, ImColor color){
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
            auto size = ImGui::CalcTextSize(label);

            auto padding = ImGui::GetStyle().FramePadding;
            auto spacing = ImGui::GetStyle().ItemSpacing;

            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

            auto rectMin = ImGui::GetCursorScreenPos() - padding;
            auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

            auto drawList = ImGui::GetWindowDrawList();
            drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
            ImGui::TextUnformatted(label);
          };
          ed::PinId inputPinId, outputPinId;
          if (ed::QueryNewLink(&inputPinId, &outputPinId))
          {
              if (inputPinId && outputPinId&& inputPinId!= outputPinId)
              {
                  uintptr_t input_value = static_cast<uintptr_t>(inputPinId);
                  uintptr_t output_value = static_cast<uintptr_t>(outputPinId);
                  //print input and output
				  //printf("input pin id:%llu, output pin id:%llu\n", input_value, output_value);
                  int in_node_id = -1,in_sub_id=-1,out_node_id=-1,out_sub_id=-1;
                  auto find_in_link_id=[&]()->bool {
                      int id = 0;
                      for (auto& nd : node_list) {
                          int s_id = 0;
                          for (auto& sb : nd.output_link_list) {
                              if (sb == input_value) {
                                  in_sub_id = s_id;
                                  in_node_id = id;
                                  return true;
                              }
                              s_id++;
                          }
                          id++;
                      }
                      return false;
					  };
                  auto find_out_link_id = [&]() -> bool {
                      int id = 0;
                      for (auto& nd : node_list) {
                          int s_id = 0;
                          for (auto& sb : nd.input_link_list) {
                              if (sb == output_value) {
                                  out_sub_id = s_id;
                                  out_node_id = id;
                                  return true;
                              }
                              s_id++;
                          }
                          id++;
                      }
					  return false;

                  };
                   if (find_in_link_id()&& find_out_link_id()) {
                       auto& calcu_u_in =* v_sd_calculate_unit_[in_node_id];
                       auto& value_u=*calcu_u_in.output_ids[in_sub_id];
                       int in_type_value=0, in_type_count = 0;
                       value_u.value_info(in_type_value, in_type_count);
                       auto& calcu_u_out = v_sd_calculate_unit_[out_node_id];
                       int out_type_value=0, out_type_count = 0;
                       calcu_u_out->input_info(out_sub_id,out_type_value, out_type_count);
                       if (in_type_value != out_type_value || in_type_count != out_type_count) {
                           showLabel("pin mimatch!", ImColor(45, 32, 32, 180));
                           ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                       }
                       else 
                       {
                           bool tail_node_is_canvas = calcu_u_out->output_ids.size() == 0;
                           if (tail_node_is_canvas) {
                               auto sd_canvas_ = std::dynamic_pointer_cast<canvas>(calcu_u_out);
							   auto is_connected= sd_canvas_->input_list[out_sub_id];
                               if (is_connected) {
                                   showLabel("canvas input pin already connected!", ImColor(45, 32, 32, 180));
                                   ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                               }
                               else {
								   showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                                   if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f)) {
                                       sd_canvas_->input_list[out_sub_id] = true;
                                       auto new_link_id = get_node_id();
                                       links_ex_.insert({ { out_node_id, out_sub_id },{ in_node_id,in_sub_id,new_link_id,4 } });
                                       ed::Link(new_link_id, inputPinId, outputPinId);
									   auto input_count = sd_canvas_->input_list.size();
                                       if (out_sub_id == input_count - 1) {
										   sd_canvas_->input_list.push_back(false);
										   node_list[out_node_id].input_link_list.push_back(get_node_id());
                                       }
                                   }
							   }


                           }
                           else {
                               showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                               if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f)) {
                                    auto new_link_id = get_node_id();
                                    links_ex_.insert({ { out_node_id, out_sub_id },{ in_node_id,in_sub_id,new_link_id,4 } });
                                    ed::Link(new_link_id, inputPinId, outputPinId); 
                               }
                           }

                           
                       }/*else {
                           showLabel("Link should from output pin to input pin!", ImColor(45, 32, 32, 180));
                           ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                       } */    
                   }         
              }
          }
      }
      ed::EndCreate();
      if (ed::BeginDelete()) {
          ed::LinkId deletedLinkId;
          while (ed::QueryDeletedLink(&deletedLinkId))
          {
              if (ed::AcceptDeletedItem())
              { 
                  uintptr_t udel_value = static_cast<uintptr_t>(deletedLinkId);
                  std::erase_if(links_ex_, [&](auto& lk) {
                      bool finded= lk.second.link_id == udel_value;
                      if (finded) {
						  auto out_node_id = lk.first.out_node_id;
                          auto out_sub_id = lk.first.out_sub_id;
						  auto calcu_u_out = v_sd_calculate_unit_[out_node_id];
                          bool tail_node_is_canvas = calcu_u_out->output_ids.size() == 0;
                          if (tail_node_is_canvas)
                          {
                              auto sd_canvas_ = std::dynamic_pointer_cast<canvas>(calcu_u_out);
                              auto& input_list = sd_canvas_->input_list;
                              input_list[out_sub_id]= false;
							  auto& output_link_list = node_list[out_node_id].input_link_list;
                              int i = static_cast<int>(input_list.size()) - 1;
                              while (i >= 0 && !input_list[i]) --i;  // 跳过所有尾部 false
                              int start_of_tail_falses = i + 1;

                              // 如果有连续 false，则保留最前面一个，删掉后面的
                              if (start_of_tail_falses < static_cast<int>(input_list.size())) {
                                  int keep_index = start_of_tail_falses;  // 要保留的 false
                                  for (int j = static_cast<int>(input_list.size()) - 1; j > keep_index; --j) {
                                      input_list.erase(input_list.begin() + j);
                                      output_link_list.erase(output_link_list.begin() + j);
                                  }
                              }
                          }
                      }
                      return finded;
					  });        
              }
          }
      }
      ed::EndDelete();
      static ed::NodeId node_id = 0;
      ed::Suspend();
      if(ed::ShowNodeContextMenu(&node_id)){
        ImGui::OpenPopup("Node Context Menu");
	  }
	  ed::Resume();
      ed::Suspend();
      if (ImGui::BeginPopup("Node Context Menu")) {
          auto idx = get_node_index_by_id(node_id);
		  auto& nd = node_list[idx];
		  //display node info
		  ImGui::Text("Node ID: %d", idx);
		  ImGui::Text("Input Links: %d", nd.input_link_list.size());
		  ImGui::Text("Output Links: %d", nd.output_link_list.size());
		  auto ilink = links_ex_.find({ idx,0 });
          auto be_outlink=ilink != links_ex_.end();
		  auto ilinked=std::find_if(links_ex_.begin(), links_ex_.end(), [&](auto& lk) {
              return lk.second.in_node_id == idx;
			  }) != links_ex_.end();
          if (!be_outlink && !ilinked) {
              ImGui::Separator();
              if (ImGui::MenuItem("Delete")) {
                  ed::DeleteNode(node_id);
                  node_list.erase(node_list.begin() + idx);
                  v_sd_calculate_unit_.erase(v_sd_calculate_unit_.begin() + idx);
              }
          }
          ImGui::EndPopup();
      }
      ed::Resume();
    }
    ed::End();
    after_node_view();
  }

  void pipepline_edit::clear(){
      v_sd_calculate_unit_.clear();
      node_list.clear();
      links_ex_.clear();
  }

  bool open_models::tryOpen(const std::string& path) {
      size_t pos = path.find_last_of('.');
      if (pos != std::string::npos) {
          std::string ext = path.substr(pos + 1);
          const char* supported_extensions[] = {
            "fbx", "dae", "blend", "3ds", "gltf", "glb", "ase","ifc","xgl","zgl","dxf","lwo",
            "lws","lxo","stl","x","ac","ms3d","cob","scn","bvh","csm",nullptr
          };
          //check supported extensions
          for (int ix = 0; supported_extensions[ix] != nullptr; ix++) {
              if (ext == supported_extensions[ix]) {
                  //std::string filename = path.substr(path.find_last_of("\\") + 1);
                  Assimp::Importer importer;
                  const aiScene* ascene = importer.ReadFile(
                      path, aiProcess_Triangulate | aiProcess_FlipUVs |
                      aiProcess_CalcTangentSpace |
                      aiProcess_GenNormals); // | aiProcess_SortByPType |
                  // aiProcess_FindDegenerates);

                  if (!ascene || ascene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
                      !ascene->mRootNode) {
                      printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
                      return false;
                  }
#define AVEC_2_VEC(cp) {cp.x,cp.y,cp.z}
#define ACOL_2_VEC(cp) {cp.r,cp.g,cp.b}
				  auto sd_canvas = std::make_shared<canvas>();
				  int canvas_id = v_sd_calculate_unit_.size();
  				  add_new_calcu_unit(sd_canvas);
                  auto sd_camera = std::make_shared<camera_ex>();
                  if (ascene->HasCameras()) {
                      auto& camera_ = ascene->mCameras[0];
                      auto& lookat = camera_->mLookAt;
                      auto& lookfrom = camera_->mPosition;
                      auto& up = camera_->mUp;
                      auto& fovy = camera_->mHorizontalFOV;
                      auto& aspect = camera_->mAspect;
                      auto& fnear = camera_->mClipPlaneNear;
                      auto& ffar = camera_->mClipPlaneFar;
					  std::string cmaera_name = camera_->mName.C_Str();
					  sd_camera->set_name(cmaera_name);
					  sd_camera->_pt.position = AVEC_2_VEC(lookfrom);
					  sd_camera->_pt.direction = AVEC_2_VEC(lookat);
					  sd_camera->_pt.up = AVEC_2_VEC(up);
                      sd_camera->_pt._near = fnear;
                      sd_camera->_pt._far = ffar;
                      sd_camera->_pt.fovy = fovy;
                      sd_camera->_pt.aspect = aspect;
                  }
				  int camera_id = v_sd_calculate_unit_.size();
                  add_new_calcu_unit(sd_camera);
				  std::string shader_features;
				  int light_id = -1;
                  if (ascene->HasLights()) {
                      shader_features += "#define USE_LIGHT\n";
                      auto& light_ = ascene->mLights[0];
                      auto& light_pos = light_->mPosition;
                      auto& color_diff = light_->mColorDiffuse;
					  auto& color_spec = light_->mColorSpecular;
					  auto& color_ambi = light_->mColorAmbient;
					  light_id = v_sd_calculate_unit_.size();
					  auto sd_light = std::make_shared<light>();
					  sd_light->_pt.position = AVEC_2_VEC(light_pos);
					  sd_light->_pt.diffuse_clr = ACOL_2_VEC(color_diff);
					  sd_light->_pt.specular_clr = ACOL_2_VEC(color_spec);
					  sd_light->_pt.ambient_clr = ACOL_2_VEC(color_ambi);
					  std::string light_name = light_->mName.C_Str();
					  sd_light->set_name(light_name);
					  sd_light->_type_name = "light";
					  add_new_calcu_unit(sd_light);
                  }
                  auto processMesh = [&](aiMesh* mesh, const aiScene* ascene,int transform_id) {
                      if (mesh->mNumFaces == 0) {
                          return;
                      }
                      std::vector<vertex_ele> vtx_eles;
                      if (mesh->HasPositions()) {
						  vtx_eles.push_back({3,(float*)mesh->mVertices});
                      }
                      if (mesh->HasNormals()) {
						  vtx_eles.push_back({ 3,(float*)mesh->mNormals });
                      }
                      if (mesh->HasTextureCoords(0)) {
						  shader_features += "#define USE_TEXTURE\n";
						  vtx_eles.push_back({ 2,(float*)mesh->mTextureCoords[0] });
                      }
                      aiFace face = mesh->mFaces[0];
                      u32 num_indices = face.mNumIndices;
                      u32 face_len = mesh->mNumFaces * num_indices;
                      u32* pface_idx = new u32[face_len];
                      for (unsigned int ix = 0; ix < mesh->mNumFaces; ix++) {
                          aiFace face = mesh->mFaces[ix];
                          auto id = ix * 3;
                          for (int idx = 0; idx < num_indices; idx++) {
                              pface_idx[id + idx] = face.mIndices[idx];
                          }
                      }
					  auto sd_vtx = std::make_shared<vertex>(vtx_eles, mesh->mNumVertices, 
                          GL_STATIC_DRAW, pface_idx, face_len);
                      delete[] pface_idx;
                      int  pipeline_id= v_sd_calculate_unit_.size();
                      std::string mesh_name = mesh->mName.C_Str();
                      if (mesh_name.empty()) {
                          mesh_name = "mesh_" + std::to_string(pipeline_id);
                      }
                      auto sd_pip = std::make_shared<pipeline>();
                      std::string str_pip(mesh_name);
                      sd_pip->set_name(str_pip);
                      sd_pip->_type_name = "pipeline";
                      add_new_calcu_unit(sd_pip);

                      std::string vertex_sd_cd = shader_features + modeling_vs;
					  std::string fragment_sd_cd = shader_features + modeling_fs;
					  auto sd_shader = std::make_shared<shader_base>();
					  sd_shader->build_vs_code(vertex_sd_cd.c_str());
					  sd_shader->build_fs_code(fragment_sd_cd.c_str());
                      sd_shader->link();
					  
					  vertex_key vtx_key;
                      vtx_key.get_str_value(mesh_name);
					  shader_base_key shd_key;
					  shd_key.get_str_value(mesh_name);
                      add_vertex_by_key( vtx_key,sd_vtx );
                      add_shader_base_by_key(shd_key, sd_shader);
					  //setup pipeline
                      sd_pip->_pt.vkey=vtx_key;
                      sd_pip->_pt.skey= shd_key;
                      sd_pip->update_shader_vertex();
                      if (light_id > 0) {//has light
						  std::string light_position = "light_position";
                          std::string light_ambient_clr = "light_ambient_clr";
						  std::string light_diffuse_clr = "light_diffuse_clr";
                          std::string light_specular_clr = "light_specular_clr";
						  auto light_position_id = sd_pip->get_input_id(light_position);
						  auto light_ambient_clr_id = sd_pip->get_input_id(light_ambient_clr);
						  auto light_diffuse_clr_id = sd_pip->get_input_id(light_diffuse_clr);
                          auto light_specular_clr_id = sd_pip->get_input_id(light_specular_clr);
                          auto new_link_id = get_node_id();
                          links_ex_.insert({ { pipeline_id, light_position_id },
                              { light_id, light::o_pos,new_link_id,4 } });
						  new_link_id = get_node_id();
                          links_ex_.insert({ { pipeline_id, light_ambient_clr_id },
                              { light_id, light::o_ambient,new_link_id,4 } });
						  new_link_id = get_node_id();
                          links_ex_.insert({ { pipeline_id, light_diffuse_clr_id },
							  { light_id, light::o_diffuse,new_link_id,4 } });
                          new_link_id = get_node_id();
                          links_ex_.insert({ { pipeline_id, light_specular_clr_id },
							  { light_id, light::o_specular,new_link_id,4 } });
                      }
                      std::string camera_str = "camera";
					  std::string viewPos = "viewPos";
					  auto camera_id_in = sd_pip->get_input_id(camera_str);
                      auto new_link_id = get_node_id();
                      links_ex_.insert({ { pipeline_id, camera_id_in },
                          { camera_id, camera_ex::op,new_link_id,4 } });
					  auto viewPos_id = sd_pip->get_input_id(viewPos);
					  new_link_id = get_node_id();
                      links_ex_.insert({ { pipeline_id, viewPos_id },
						  { camera_id, camera_ex::camPos,new_link_id,4 } });
					  //link transform
					  std::string model = "model";
					  new_link_id = get_node_id();
                      links_ex_.insert({ { pipeline_id, sd_pip->get_input_id(model)  },
						  { transform_id, model_transform::op,new_link_id,4 } });
                      auto amtl = ascene->mMaterials[mesh->mMaterialIndex];
                      if (mesh->HasTextureCoords(0)) {
                          auto amtl = ascene->mMaterials[mesh->mMaterialIndex];
                          auto load_texture = [&](aiMaterial* mtl, aiTextureType txtType,
                              sd_texture& sd_txt, texture_key& tx_k) {
                                  auto txtCnt = mtl->GetTextureCount(txtType);
                                  if (txtCnt > 0) {
                                      aiString aipath;
                                      mtl->Get(AI_MATKEY_TEXTURE(txtType, 0), aipath);
                                      if (!aipath.length) {
                                          return;
                                      }
                                      std::string aistr(aipath.C_Str());
                                      aistr = aistr.substr(aistr.find_last_of('\\') + 1);
                                      tx_k.get_str_value(aistr);
                                      
                                      sd_txt = std::make_shared<texture>();
                                      // 判断是否是嵌入fbx的图片 
                                      const aiTexture* aitexture = ascene->GetEmbeddedTexture(aipath.C_Str());
                                      if (aitexture) {
                                          // 纹理图片是内嵌的
                                          unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
                                          u32 widthIn = aitexture->mWidth; // 通常情况下（png、jpg），代表了整张图片大小
                                          u32 heightIn = aitexture->mHeight;
                                          u32 dataInSize =
                                              heightIn == 0 ? widthIn : widthIn * heightIn * 4;
                                          sd_txt->_txt_id = TextureHelper::transferMemory2Texture(
                                              dataIn, dataInSize, (s32&)widthIn, (s32&)heightIn,
                                              GL_RGBA);
                                          sd_txt->_width = widthIn;
                                          sd_txt->_height = heightIn;
                                      }
                                      else {
                                          // 纹理图片在硬盘上
                                          std::string fullPath = path + aipath.C_Str();
                                          if (fileExist(fullPath.c_str())) {
                                              sd_txt->_txt_id = TextureHelper::load2DTexture(
                                                  fullPath.c_str(), (s32&)sd_txt->_width,
                                                  (s32&)sd_txt->_height);
                                          }
                                          else {
                                              std::string str_txt_name = aipath.C_Str();
                                              std::string dlg_title = "pick up " + str_txt_name + " please!";
                                              OPENFILENAME ofn = { sizeof(OPENFILENAME) };
                                              ofn.hwndOwner = GetForegroundWindow();
                                              ofn.hInstance = g_cur_instance;//GetModuleHandle(NULL);
                                              //ofn.lpfnHook = ofnHookProc;
                                              ofn.lpstrFilter = "image "
                                                  "file:\0*.png;*.bmp;*.jpg;*.jpeg;*.gif;*."
                                                  "dds;*.tga;*.psd;*.hdr\0\0";
                                              char strFileName[MAX_PATH] = { 0 };
                                              ofn.nFilterIndex = 1;
                                              ofn.lpstrFile = strFileName;
                                              ofn.nMaxFile = sizeof(strFileName);
                                              ofn.lpstrTitle = dlg_title.c_str();
                                              ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                                              if (GetOpenFileName(&ofn)) {
                                                  printf("open file:%s\n", strFileName);
                                                  sd_txt->_txt_id = TextureHelper::load2DTexture(
                                                      strFileName, (s32&)sd_txt->_width,
                                                      (s32&)sd_txt->_height);
                                              }
                                          }
                                      }
                                  }
                              };
						  texture_key tx_k;
						  sd_texture sd_txt;
                          load_texture(amtl, aiTextureType_DIFFUSE, sd_txt, tx_k);
                          if (sd_txt && sd_txt->_txt_id > 0) {
                              add_texture_by_key(tx_k, sd_txt);
                              int tex_id_in = sd_pip->get_input_id("texture_diffuse");
                              auto new_link_id = get_node_id();
                              // add texture unit
                              auto sd_texture_unit_ = std::make_shared<texture_unit>();
                              sd_texture_unit_->_pt.tkey = tx_k;
                              sd_texture_unit_->sd_txt = sd_txt;
                              int texture_unit_id = v_sd_calculate_unit_.size();
                              add_new_calcu_unit(sd_texture_unit_);
                              links_ex_.insert({ { pipeline_id, tex_id_in },
                                  { texture_unit_id, texture_unit::op,new_link_id,4 } });
						  }

                      }
                      else {
						  std::string diffuse_clr = "diffuse_clr";
						  auto diffuse_clr_sub_id = sd_pip->get_input_id(diffuse_clr);
						  auto sd_diffuse_clr = std::make_shared<color_rgb>();
                          sd_diffuse_clr->_pt.data_clr = { 0.8f,0.8f,0.8f };
                          aiColor3D a_color_diffuse(0.f, 0.f, 0.f);
                          if (amtl->Get(AI_MATKEY_COLOR_DIFFUSE, a_color_diffuse) ==
                              AI_SUCCESS) {
                              sd_diffuse_clr->_pt.data_clr = ACOL_2_VEC(a_color_diffuse);
						  }
						  int diffuse_clr_id = v_sd_calculate_unit_.size();
                          add_new_calcu_unit(sd_diffuse_clr);
                          new_link_id = get_node_id();
                          links_ex_.insert({ { pipeline_id, diffuse_clr_sub_id },
							  { diffuse_clr_id, color_rgb::op,new_link_id,4 } });

						  std::string specular_clr = "specular_clr";
						  auto specular_clr_sub_id = sd_pip->get_input_id(specular_clr);
                          auto sd_specular_clr = std::make_shared<color_rgb>();
                          sd_specular_clr->_pt.data_clr = { 1.0f,1.0f,1.0f };
                          aiColor3D a_color_specular(0.f, 0.f, 0.f);
                          if (amtl->Get(AI_MATKEY_COLOR_SPECULAR, a_color_specular) ==
                              AI_SUCCESS) {
                              sd_specular_clr->_pt.data_clr = ACOL_2_VEC(a_color_specular);
                          }
						  int specular_clr_id = v_sd_calculate_unit_.size();
                          add_new_calcu_unit(sd_specular_clr);
                          new_link_id = get_node_id();
						  links_ex_.insert({ { pipeline_id, specular_clr_sub_id },
							  { specular_clr_id, color_rgb::op,new_link_id,4 } });
                      }
					  //link to canvas
                      auto canvas_pipeline_id = sd_canvas->input_count();
					  sd_canvas->input_list.push_back(false);
                      new_link_id = get_node_id();
                      links_ex_.insert({ { canvas_id, canvas_pipeline_id },
						  { pipeline_id, model_transform::op,new_link_id,4 } });
					  

                  };
                  std::function<void(aiNode*, const aiScene*,int)> processNode
                      =[&](aiNode* node, const aiScene* scene, int parent_model_index)->void {
					  //pick up node's transformation
                      aiVector3D scaling, position;
                      aiQuaternion rotation;
                      aiDecomposeMatrix(&node->mTransformation, &scaling, &rotation, &position);

					  auto sd_mt = std::make_shared<model_transform>();
                      auto& mt=*sd_mt;
                      mt._pt.translate = { position.x, position.y, position.z };
                      mt._pt.scale = { scaling.x, scaling.y, scaling.z };

                      auto aiQuatToGlm = [](const aiQuaternion& q, glm::quat& out){
                              // aiQuaternion: (w, x, y, z)
                              out = glm::quat((float)q.w, (float)q.x, (float)q.y, (float)q.z);
                              out = glm::normalize(out);
                          };
                      glm::quat gq;
                      aiQuatToGlm(rotation, gq);
                      glm::vec3 axis = glm::axis(gq);
                      float angle = glm::angle(gq); 
                      mt._pt.roatation_axis = { axis.x, axis.y, axis.z };
                      mt._pt.angle = angle; // 弧度

                      int transform_id = v_sd_calculate_unit_.size();
					  add_new_calcu_unit(sd_mt);
                      if (parent_model_index > 0) {
						  auto new_link_id = get_node_id();
						  links_ex_.insert({ { transform_id, model_transform::base_value },
                              { parent_model_index, model_transform::op,new_link_id,4 } });
                      }

                      for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                          aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                          processMesh(mesh, scene,transform_id);
                      }
                      // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
                      for (unsigned int i = 0; i < node->mNumChildren; i++) {
                          processNode(node->mChildren[i], scene, transform_id);
                      }
			      };
				  processNode(ascene->mRootNode, ascene,-1);
                  //auto node_position = ImGui::GetMousePos();
                  //ed::SetNodePosition(ref_nd.node_id, node_position);
				  
              }
          }
      }
      return false;
  }

}