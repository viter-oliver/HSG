#include "user_control_imgui.h"
#include "imgui_internal.h"
#include "image.hpp"

namespace vg{

    image::image() { 
      _pt._tin_clr = {1.f, 1.f, 1.f};
    }
    void image::draw(){ 
      auto base_draw_pos = get_draw_pos();
      ImVec2 pos0 = to_imvec2(base_draw_pos);
      ImVec2 pos1 = {pos0.x, pos0.y + size().y()};
      ImVec2 pos2 = {pos1.x + size().x(), pos1.y};
      ImVec2 pos3 = {pos2.x, pos0.y};
      if (!_pt._normal_rectangle) {
        auto left_bottom = base_draw_pos + _pt._left_bottom_offset;
        pos1 = to_imvec2(left_bottom);
        auto right_bottom = base_draw_pos + _pt._right_bottom_offset;
        pos2 = to_imvec2(right_bottom);
        auto right_top = base_draw_pos + _pt._right_top_offset;
        pos3 = to_imvec2(right_top);
      }
      ImVec2 uv0 = {0, 0};
      ImVec2 uv1 = {0, 1};
      ImVec2 uv2 = {1, 1};
      ImVec2 uv3 = {1, 0};
      ImVec4 tin_clr = {_pt._tin_clr.x(), _pt._tin_clr.y(), _pt._tin_clr.z(), _aplpha_nml};
      auto itxt = texture_map_.find(_pt._texture_key);
      if (itxt != texture_map_.end()) {
        auto &txt_u = *itxt->second;
        if (_pt._sub_id < txt_u._v_sd_sub_txt.size()) {
          auto &sub_u = *txt_u._v_sd_sub_txt[_pt._sub_id];
          uv0 = {sub_u._x0/txt_u._width,sub_u._y0/txt_u._height};
          uv1 = {uv0.x, sub_u._y1 / txt_u._height};
          uv2 = {sub_u._x1 / txt_u._width, uv1.y};
          uv3 = {uv2.x, uv0.y};
        }
        imgui_ImageQuad((ImTextureID)txt_u._txt_id, pos0, pos1, pos2, pos3, uv0,
                        uv1, uv2, uv3, tin_clr);
      } else {
        internal_image("null", pos0, pos1, pos2, pos3);
      }
    }
}