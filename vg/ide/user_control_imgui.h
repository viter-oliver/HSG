#pragma once
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
extern bool IconTreeNode(const char *icon_name, const char *label,
                         ImGuiTreeNodeFlags flags,
                         const char *postfix_icon = NULL);
extern bool IconTreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags,
                                 const char *icon_name, const char *label,
                                 const char *postfix_icon);
extern bool IconButton(const char *icon_name,
                       ImVec2 &img_size = ImVec2(0, 0), int flags = 0,
                       int frame_padding = -1,
                       const ImVec4 &bg_col = ImVec4(0, 0, 0, 0),
                       const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));