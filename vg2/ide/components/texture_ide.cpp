#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include "SOIL.h"
#include "common_functions.h"
#include "dir_output.h"
#include "ide_common_functions.h"
#include "texture.h"
#include "texture_ide.h"
#include "user_control_imgui.h"
namespace vg {
using namespace std;
static int cur_index = -1;
extern string g_current_directory;
texture_ide::sd_texture_ide texture_ide::new_instance() {
  static string texture_pack_file, texture_data_file; // png/json
  static bool first_call = true;
  if (first_call) {
    texture_pack_file.clear();
    texture_data_file.clear();
    first_call = false;
  }
  sd_texture_ide sd_new_txt=nullptr;
  ImGui::Text("Texture pack file:%s", texture_pack_file.c_str());
  ImGui::SameLine();
  if (ImGui::Button("...")) {
    open_file_to_folder(
        texture_pack_file, texture_fold,
        "Loading new packing texture...",
        "image "
        "file:\0*.png;*.bmp;*.jpg;*.jpeg;*.gif;*.dds;*.tga;*.psd;*.hdr\0\0");
  }
  ImGui::Text("Texture data file:%s", texture_data_file.c_str());
  ImGui::SameLine();
  if (ImGui::Button("...##_")) {
    open_file_to_folder(texture_data_file, texture_fold,
                        "Loading new packing texture data...",
                        "data file:\0*.json\0\0");
  }
  bool be_enable = !texture_pack_file.empty();
  if (!be_enable) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }
  if (ImGui::Button("new packing texture")) {

    auto sd_new_pk_txt =
        make_shared<texture_ide>();
    sd_new_pk_txt->texture_pack_file = texture_pack_file;
    string txt_full_name =
        g_current_directory + texture_fold + texture_pack_file;

    sd_new_pk_txt->txt_id = TextureHelper::load2DTexture(
        txt_full_name.c_str(), (int &)sd_new_pk_txt->_width,
        (int &)sd_new_pk_txt->_height, GL_RGBA, GL_RGBA, SOIL_LOAD_RGBA);
    if (!texture_data_file.empty()) {
      sd_new_pk_txt->texture_data_file = texture_data_file;
      string data_full_name =
          g_current_directory + texture_fold + texture_data_file;
      get_file_data(data_full_name, [&](char *pbuff, unsigned int buff_len) {
        auto &vtext_coor = sd_new_pk_txt->_v_sd_sub_txt;
        load_vtex_from_json(vtext_coor, pbuff, buff_len);
      });
    }
    sd_new_txt=sd_new_pk_txt;
    first_call = true;
    ImGui::CloseCurrentPopup();
  }
  if (!be_enable) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return sd_new_txt;
}
void texture_ide::edit(texture_ide &tu) {
  ImGui::Checkbox("Separated", &tu.separated);
  ImGui::Checkbox("Mipmap", &tu.mipmap);
  auto txtW = tu._width;
  auto txtH = tu._height;
  auto &vtxt_coor = tu._v_sd_sub_txt;
  if (vtxt_coor.size() > 0) {
    for (int ix = 0; ix < vtxt_coor.size(); ++ix) {
      auto& ptxt_coor_unit = *vtxt_coor[ix];
      
      ImGui::Text("%s", ptxt_coor_unit.item_name.c_str());
      ImVec2 uv0 = { ptxt_coor_unit._x0 / txtW, ptxt_coor_unit._y0 / txtH};
      ImVec2 uv1 = { ptxt_coor_unit._x1 / txtW, ptxt_coor_unit._y1 / txtH};
      ImVec2 img_sz = { ptxt_coor_unit.width(), ptxt_coor_unit.height()};
      ImVec4 bk_col;
      bk_col = ix == tu.sub_id ? ImVec4(0, 0, 0, 0) : ImVec4(0.3, 0.3, 0.3, 1);
      if (ImGui::ImageButton((ImTextureID)tu.txt_id, img_sz, uv0, uv1,0,1,bk_col)) {
          tu.sub_id = ix;
      }
    }
  } else {
    ImVec2 uv0 = {0, 0};
    ImVec2 uv1 = {1, 1};
    ImVec2 img_sz = {txtW, txtH};
    if (img_sz.x > img_sz.y && img_sz.x > 300.f) {
      img_sz = {300, 300 * img_sz.y / img_sz.x};
    } else if (img_sz.y > 300) {
      img_sz = {300 * img_sz.y / img_sz.x, 300};
    }
    ImGui::ImageButton((ImTextureID)tu.txt_id, img_sz, uv0, uv1);
  }
}
} // namespace vg