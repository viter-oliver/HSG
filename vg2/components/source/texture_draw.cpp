#include "texture_draw.h"
#include "user_control_imgui.h"
#include "texture_ex.h"
#include <assert.h>
namespace vg {
void draw_quad(texture_key &id, vec2 &pos0, vec2 &pos1, vec2 &pos2, vec2 &pos3,
               vec4 &&tint_clr, vec4 &&bord_clr) {
  u32 cur_txt_id, texture_width, texture_height;

  sub_txt cur_txt_coor = {0, 0, 1.f, 1.f};
  
  auto& itxt = g_texture_dic.find(id.value);
  if(itxt==g_texture_dic.end()){
    vg_print("invalid texture key:%s\n",id.value);
    return;
  }
  auto& txt=*itxt->second;
  texture_width = txt._width;
  texture_height = txt._height;
  auto& vsub=txt._v_sd_sub_txt;
  if (id.sub_id < vsub.size()) {
    cur_txt_coor = *vsub[id.sub_id];
  } 

  ImVec2 winpos = ImGui::GetWindowPos();

  ImVec2 uv0 = ImVec2(cur_txt_coor._x0 / texture_width,
                      cur_txt_coor._y0 / texture_height);
  ImVec2 uv1 = ImVec2(cur_txt_coor._x0 / texture_width,
                      (cur_txt_coor._y1) / texture_height);
  ImVec2 uv2 = ImVec2((cur_txt_coor._x1) / texture_width,
                      (cur_txt_coor._y1) / texture_height);
  ImVec2 uv3 = ImVec2((cur_txt_coor._x1) / texture_width,
                      (cur_txt_coor._y0) / texture_height);
  ImVec4 imtint_clr(tint_clr.x, tint_clr.y, tint_clr.z, tint_clr.w);
  ImVec4 imbrd_clr(bord_clr.x, bord_clr.y, bord_clr.z, bord_clr.w);
  auto add_pos_2_impos = [](vec2 &pos, ImVec2 &impos) -> ImVec2 {
    ImVec2 pos_rtn = {pos.x + impos.x, pos.y + impos.y};
    return pos_rtn;
  };
  ImVec2 ipos0 = add_pos_2_impos(pos0, winpos);
  ImVec2 ipos1 = add_pos_2_impos(pos1, winpos);
  ImVec2 ipos2 = add_pos_2_impos(pos2, winpos);
  ImVec2 ipos3 = add_pos_2_impos(pos3, winpos);
  ImGui::ImageQuad((ImTextureID)cur_txt_id, ipos0, ipos1, ipos2, ipos3, uv0,
                   uv1, uv2, uv3, imtint_clr, imbrd_clr);
}
void draw_rect(texture_key &id, vec2 &pos, vec2 &rect_size, vec4 &&tint_clr,
               vec4 &&bord_clr) {
  u32 cur_txt_id, texture_width, texture_height;
  sub_txt cur_txt_coor = {0, 0, 1.f, 1.f};
  auto& itxt = g_texture_dic.find(id.value);
  if (itxt == g_texture_dic.end()) {
      vg_print("invalid texture key:%s\n", id.value);
      return;
  }
  auto& txt = *itxt->second;
  texture_width = txt._width;
  texture_height = txt._height;
  auto& vsub = txt._v_sd_sub_txt;
  if (id.sub_id < vsub.size()) {
      cur_txt_coor = *vsub[id.sub_id];
  }

  ImVec2 uv0 = ImVec2(cur_txt_coor._x0 / texture_width,
                      cur_txt_coor._y0 / texture_height);

  ImVec2 uv2 = ImVec2((cur_txt_coor._x1) / texture_width,
                      (cur_txt_coor._y1) / texture_height);
  ImVec4 imtint_clr(tint_clr.x, tint_clr.y, tint_clr.z, tint_clr.w);
  ImVec4 imbrd_clr(bord_clr.x, bord_clr.y, bord_clr.z, bord_clr.w);
  auto cur_cursor_pos = ImGui::GetCursorPos();
  if (pos.x > 0.f)
    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
  ImGui::Image((ImTextureID)cur_txt_id, ImVec2(rect_size.x, rect_size.y), uv0,
               uv2, imtint_clr, imbrd_clr);
  ImGui::SetCursorPos(cur_cursor_pos);
}
} // namespace vg