#include "textblock.hpp"
#include "user_control_imgui.h"
#include "imgui_internal.h"
#include<algorithm>
#include<iostream>
namespace vg {
    void textblock::ex_init_fun() {
        control_base::ex_init_fun();
            
        // 获取当前的face_id并设置相应的控件值
        _user_properties_handlor[_face_id] = [&] {
            auto ifu = font_unit_map_.find(_txt_pt._font_key);
            if (ifu != font_unit_map_.end()) 
            {
                auto& fu = ifu->second;
                fu->face_count;
                static std::vector<std::string> items;

                // todo::实现功能并进行优化
                items.clear();
                // 获取父节点的属性
                for (int i = 0; i < fu->face_count; i++)
                {
                    std::string tmp = std::string(fu->_ft_face[i]->family_name) + std::string(fu->_ft_face[i]->style_name);
                    items.emplace_back(tmp);
                }
                // 显示combo控件中的选项
                static int current_item = -1;        // 当前选中的项目是
                static int init_flag = 1;
                if (init_flag)
                {
                    current_item = _txt_pt._face_id;
                    init_flag = 0;
                }
                    if (current_item > fu->face_count || current_item > max_face_count)return;

                if (ImGui::BeginCombo("##combo", current_item >= 0 ? items[current_item].c_str() : "Select an option"))
                {

                    for (int i = 0; i < items.size(); i++)
                    {
                        bool is_selected = (current_item == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                        {
                            current_item = i;  // 选择某个项目
                            _txt_pt._face_id = i;
                        }
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();  // 重置为默认值
                        }
                    }
                    ImGui::EndCombo();
                }

            }

            };

    }

    //gray@2024.09.23
    void textblock::draw_sel_anchor() {

        float delta_x = _txt_area.right_bottom.x() - _txt_area.left_top.x();
        float delta_y = _txt_area.right_bottom.y() - _txt_area.left_top.y();
        float line_height = _txt_pt._font_size * _txt_pt._font_scale;

        auto base_draw_pos = get_draw_pos();//控件左下角pos
        vg::vec2 draw_pos;
        draw_pos.x() = base_draw_pos.x() - delta_x * _txt_pt._txt_alignh_nml;
        draw_pos.y() = base_draw_pos.y() - delta_y * _txt_pt._txt_alignv_nml;


        ImVec2 pos1 = { draw_pos.x()       , draw_pos.y() - _txt_pt._font_line_offset };         // 左上角
        //ImVec2 pos1 = { draw_pos.x       , draw_pos.y - line_height };         // 左上角
        ImVec2 pos2 = { pos1.x           , pos1.y + delta_y };         // 左下角
        ImVec2 pos3 = { pos2.x + delta_x , pos2.y                   };         // 右下角
        ImVec2 pos4 = { pos3.x           , pos1.y                   };         // 右上角

        // 获取颜色
        ImU32 col = ImGui::GetColorU32(ImGuiCol_HeaderActive);

        // 定义编辑单位的大小
        ImVec2 editunit(edit_unit_len/2, edit_unit_len/2);

        // 对 pos1 (左上角) 进行渲染
        ImVec2 pos1a = pos1 - editunit;
        ImVec2 pos1b = pos1 + editunit;
        ImGui::RenderFrame(pos1a, pos1b, col);

        // 对 pos2 (左下角) 进行渲染
        ImVec2 pos2a = pos2 - editunit;
        ImVec2 pos2b = pos2 + editunit;
        ImGui::RenderFrame(pos2a, pos2b, col);

        // 对 pos3 (右下角) 进行渲染
        ImVec2 pos3a = pos3 - editunit;
        ImVec2 pos3b = pos3 + editunit;
        ImGui::RenderFrame(pos3a, pos3b, col);

        // 对 pos4 (右上角) 进行渲染
        ImVec2 pos4a = pos4 - editunit;
        ImVec2 pos4b = pos4 + editunit;
        ImGui::RenderFrame(pos4a, pos4b, col);

    }
}