#pragma once
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "vg_type.h"
namespace vg {
inline ImVec2 to_imvec2(vec2 &vg_vev2) { return ImVec2(vg_vev2.x, vg_vev2.y); }
inline vec2 to_vec2(ImVec2& im_vec2){ return {im_vec2.x,im_vec2.y};}
inline ImVec2 operator+(const ImVec2 &imPt, const vec2 &afPt) {
  return ImVec2(imPt.x + afPt.x, imPt.y + afPt.y);
}
inline vec2 operator+(const const vec2&afPt, ImVec2 &imPt) {
  return {imPt.x + afPt.x, imPt.y + afPt.y};
}
extern bool IconTreeNode(const char *icon_name, const char *label,
                         ImGuiTreeNodeFlags flags,
                         const char *postfix_icon = NULL);
extern bool IconButton(const char *icon_name, ImVec2 &img_size = ImVec2(0, 0),
                       int flags = 0, int frame_padding = -1,
                       const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                       const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));

} // namespace vg