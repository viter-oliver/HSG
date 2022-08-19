#include "packing_texture_draw.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
namespace packing_texture {
void draw_quad(packing_text_id &id, vec2 &pos0, vec2 &pos1, vec2 &pos2,
               vec2 &pos3, vec4 &tint_clr,
               vec4 &bord_clr) {
  auto &cur_packing_texture = *_v_sd_packing_texture[id.x];
  auto cur_txt_id = cur_packing_texture.txt_id;
  auto &cur_vtexture_coordinate = cur_packing_texture.vtexture_coordinate;
  if (cur_vtexture_coordinate.size() == 0) {
    return;
  }
  auto texture_width = cur_packing_texture._width;
  auto texture_height = cur_packing_texture._height;
  ImVec2 winpos = ImGui::GetWindowPos();
  auto &cur_txt_coor = *cur_vtexture_coordinate[id.y];
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
  auto add_pos_2_impos = [](vec2 &pos, ImVec &impos) -> ImVec2 {
    ImVec2 pos_rtn = {pos.x + impos.x, pos.y + impos.y};
    return pos_rtn;
  } ImVec2 ipos0 = add_pos_2_impos(pos0, winpos);
  ImVec2 ipos1 = add_pos_2_impos(pos1, winpos);
  ImVec2 ipos2 = add_pos_2_impos(pos2, winpos);
  ImVec2 ipos3 = add_pos_2_impos(pos3, winpos);
  ImGui::ImageQuad((ImTextureID)cur_txt_id, ipos0, ipos1, ipos2, ipos3, uv0,
                   uv1, uv2, uv3, imtint_clr, imbrd_clr);
}
void draw_rect(packing_text_id &id, vec2 &pos, vec2 &rect_size,
               vec4 &tint_clr,
               vec4 &bord_clr) {
  auto &cur_packing_texture = *_v_sd_packing_texture[id.x];
  auto cur_txt_id = cur_packing_texture.txt_id;
  auto &cur_vtexture_coordinate = cur_packing_texture.vtexture_coordinate;
  if (cur_vtexture_coordinate.size() == 0) {
    return;
  }
  auto texture_width = cur_packing_texture._width;
  auto texture_height = cur_packing_texture._height;

  auto &cur_txt_coor = *cur_vtexture_coordinate[id.y];
  ImVec2 uv0 = ImVec2(cur_txt_coor._x0 / texture_width,
                      cur_txt_coor._y0 / texture_height);

  ImVec2 uv2 = ImVec2((cur_txt_coor._x1) / texture_width,
                      (cur_txt_coor._y1) / texture_height);
  ImVec4 imtint_clr(tint_clr.x, tint_clr.y, tint_clr.z, tint_clr.w);
  ImVec4 imbrd_clr(bord_clr.x, bord_clr.y, bord_clr.z, bord_clr.w);
  auto cur_cursor_pos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
  ImGui::Image((ImTextureID)cur_txt_id, ImVec2(rect_size.x, rect_size.y), uv0,
               uv2, imtint_clr, imbrd_clr);
  ImGui::SetCursorPos(cur_cursor_pos);
}
} // namespace packing_texture
} // namespace vg