#include "image.hpp"
#include "user_control_imgui.h"
#include "imgui_internal.h"


namespace vg{
    void image::ex_init_fun(){
      control_base::ex_init_fun();
      _user_properties_handlor[_normal_rectangle] = [&] {
        if (ImGui::Checkbox("normal rectangle", &_pt._normal_rectangle)) {
          if (!_pt._normal_rectangle) {
            _pt._right_top_offset = {size().x(),0};
            _pt._left_bottom_offset = {0, size().y()};
            _pt._right_bottom_offset = size();
          } else {
            size() = _pt._left_bottom_offset;
          }
        }
      };
      #if 0
      _user_properties_handlor[_texture_key] = [&] {
        internal_image("texture");
        ImGui::SameLine(0, 20);
        ImGui::InputText("texture key", _pt._texture_key.value, FILE_NAME_LEN);
        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("texture")) {
            if (ImGui::IsMouseReleased(0)) {
              memcpy_s(_pt._texture_key.value, payload->DataSize, payload->Data,
                       payload->DataSize);
            }
          }
          ImGui::EndDragDropTarget();
        }
        auto itxt = texture_map_.find(_pt._texture_key);
        if (itxt != texture_map_.end()) {
          auto &txt_u = *itxt->second;

          if (txt_u._v_sd_sub_txt.size() > 0) {
            
          }
        }
      };
      #endif
      _user_properties_handlor[_sub_id] = [&] { 
        auto itxt = texture_map_.find(_pt._texture_key);
        if (itxt != texture_map_.end()) {
          auto &txt_u = *itxt->second;
          if (txt_u._v_sd_sub_txt.size() > 0) {
            ImGui::BeginChild("##textture subid", ImVec2(0, 200),
                        ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
            for (int ix = 0; ix < txt_u._v_sd_sub_txt.size(); ix++) {
              auto &sub_u = *txt_u._v_sd_sub_txt[ix];
              if (ImGui::Selectable(sub_u.item_name.c_str(),
                                    _pt._sub_id == ix)) {
                _pt._sub_id = ix;
              }
            }
            ImGui::EndChild();
            auto &cur_sub_u = *txt_u._v_sd_sub_txt[_pt._sub_id];
            ImVec2 uv0 = {cur_sub_u._x0 / txt_u._width,
                          cur_sub_u._y0 / txt_u._height};
            ImVec2 uv1 = {cur_sub_u._x1 / txt_u._width,
                          cur_sub_u._y1 / txt_u._height};

            ImGui::SameLine();
            ImGui::Text("original size:%.2fx%.2f", cur_sub_u.width(),
                        cur_sub_u.height());
            ImGui::SameLine();
            if (ImGui::Button("restore to original size")) {
              size() = {cur_sub_u.width(), cur_sub_u.height()};
            }
            ImGui::Image((ImTextureID)txt_u._txt_id,
                         ImVec2(cur_sub_u.width(), cur_sub_u.height()), uv0,
                         uv1);
          }
        }
       
      };

    }
}