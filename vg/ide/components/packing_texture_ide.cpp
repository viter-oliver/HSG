#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include "SOIL.h"
#include "common_functions.h"
#include "dir_output.h"
#include "ide_common_functions.h"
#include "packing_texture_ide.h"
#include "texture.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
using namespace std;
static char packing_texture_name[FILE_NAME_LEN] = {0};
static int cur_index = -1;
extern string g_current_directory;
namespace packing_texture {

void packing_texture_unit_ide::select_item(int index) {
  strcpy(packing_texture_name, name.c_str());
  packing_texture_name[name.size()] = 0;
  cur_index = index;
}
void packing_texture_unit_ide::edit_item() {
  if (ImGui::InputText("Texture name", packing_texture_name, FILE_NAME_LEN)) {
    name = packing_texture_name;
  }
  ImGui::Checkbox("Separated", &separated);
  ImGui::Checkbox("Mipmap", &mipmap);
  auto txtW = _width;
  auto txtH = _height;
  auto &vtxt_coor = vtexture_coordinate;
  if (vtxt_coor.size() > 0) {
    for (int ix = 0; ix < vtxt_coor.size(); ++ix) {
      auto ptxt_coor_unit = vtxt_coor[ix];
      auto pidex_txt_coor_unit =
          static_pointer_cast<packing_texture::sd_sub_texture_coordinate_ide>(
              ptxt_coor_unit);
      auto &pide_txt_coor_u = (*pidex_txt_coor_unit);
      ImGui::Text("%s", pide_txt_coor_u->filename.c_str());
      ImVec2 uv0 = {pide_txt_coor_u->_x0 / txtW, pide_txt_coor_u->_y0 / txtH};
      ImVec2 uv1 = {pide_txt_coor_u->_x1 / txtW, pide_txt_coor_u->_y1 / txtH};
      ImVec2 img_sz = {pide_txt_coor_u->width(), pide_txt_coor_u->height()};
      if (ImGui::ImageButton((ImTextureID)txt_id, img_sz, uv0, uv1)) {
        dragging_value_to_tar::reg_a_value(
            make_shared<dragging_texture_item>(cur_index, ix));
      }
      dragging_value_to_tar::try_dragging();
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
    if (ImGui::ImageButton((ImTextureID)txt_id, img_sz, uv0, uv1)) {
      dragging_value_to_tar::reg_a_value(
          make_shared<dragging_texture_item>(cur_index, -1));
    }
    dragging_value_to_tar::try_dragging();
  }
}
} // namespace packing_texture
void create_item(
    packing_texture::v_sd_packing_texture_unit_ide &packing_texture_container) {
  static string texture_pack_file, texture_data_file; // png/json
  static bool first_call = true;
  if (first_call) {
    packing_texture_name[0] = 0;
    texture_pack_file.clear();
    texture_data_file.clear();
    first_call = false;
  }
  ImGui::InputText("Texture name", packing_texture_name, FILE_NAME_LEN);
  ImGui::Text("Texture pack file:%s", texture_pack_file.c_str());
  ImGui::SameLine();
  if (ImGui::Button("...")) {
    open_file_to_folder(
        texture_pack_file, packing_texture_fold,
        "Loading new packing texture...",
        "image "
        "file:\0*.png;*.bmp;*.jpg;*.jpeg;*.gif;*.dds;*.tga;*.psd;*.hdr\0\0");
  }
  ImGui::Text("Texture data file:%s", texture_data_file.c_str());
  ImGui::SameLine();
  if (ImGui::Button("...##_")) {
    open_file_to_folder(texture_data_file, packing_texture_fold,
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
        make_shared<packing_texture::packing_texture_unit_ide>();
    sd_new_pk_txt->texture_pack_file = texture_pack_file;
    if (strlen(packing_texture_name) != 0) {
      sd_new_pk_txt->name = packing_texture_name;
    } else {
      sd_new_pk_txt->name = texture_pack_file;
    }
    string txt_full_name =
        g_current_directory + packing_texture_fold + texture_pack_file;

    sd_new_pk_txt->txt_id = TextureHelper::load2DTexture(
        txt_full_name.c_str(), (int &)sd_new_pk_txt->_width,
        (int &)sd_new_pk_txt->_height, GL_RGBA, GL_RGBA, SOIL_LOAD_RGBA);
    if (!texture_data_file.empty()) {
      sd_new_pk_txt->texture_data_file = texture_data_file;
      string data_full_name =
          g_current_directory + packing_texture_fold + texture_data_file;
      get_file_data(data_full_name, [&](char *pbuff, unsigned int buff_len) {
        auto &vtext_coor = sd_new_pk_txt->vtexture_coordinate;
        load_vector_texture_coordinate_ide_from_json(vtext_coor, pbuff,
                                                     buff_len);
      });
    }
    packing_texture_container.emplace_back(sd_new_pk_txt);
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
}
} // namespace vg