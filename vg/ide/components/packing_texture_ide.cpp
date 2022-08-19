#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad.h"
#endif
#include "SOIL.h"
#include "texture.h"
#include "common_functions.h"
#include "dir_output.h"
#include "ide_common_functions.h"
#include "packing_texture_ide.h"
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
namespace vg {
using namespace std;
extern string g_cureent_directory;
static char packing_texture_name[FILE_NAME_LEN] = {0};
void create_item(packing_texture::v_sd_packing_texture &pk_texture_list) {
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
  bool be_enable = !texture_pack_file.empty() && !texture_data_file.empty();
  if (!be_enable) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }
  if (ImGui::Button("new packing texture")) {
    auto sd_new_pk_txt =
        make_shared<packing_texture::sd_packing_texture_unit_ide>();
    sd_new_pk_txt->texture_pack_file = texture_pack_file;
    sd_new_pk_txt->texture_data_file = texture_data_file;
    if (strlen(packing_texture_name) != 0) {
      sd_new_pk_txt->name = packing_texture_name;
    } else {
      sd_new_pk_txt->name = texture_pack_file;
    }
    string txt_full_name =
        g_cureent_directory + packing_texture_fold + texture_pack_file;

    sd_new_pk_txt->txt_id = TextureHelper::load2DTexture(
        txt_full_name.c_str(), sd_new_pk_txt->width, sd_new_pk_txt->height,
        GL_RGBA, GL_RGBA, SOIL_LOAD_RGBA);

    string data_full_name =
        g_cureent_directory + packing_texture_fold + texture_data_file;
    get_file_data(data_full_name, [&](char *pbuff, unsigned int buff_len) {
      auto &vtext_coor = sd_new_pk_txt->vtexture_coordinate;
      load_vector_texture_coordinate_ide_from_json(vtext_coor, pbuff, buff_len);
    });
    first_call = true;
  }
  if (!be_enable) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
}
void select_item(packing_texture::sd_packing_texture_unit_ide &sd_pkg_i) {
  strcpy(packing_texture_name,sd_pkg_i->name.c_str());
  packing_texture_name[sd_pkg_i->name.size()]=0;
}
void edit_item(packing_texture::sd_packing_texture_unit_ide &sd_pkg_i) {
  if(ImGui::InputText("Texture name",packing_texture_name,FILE_NAME_LEN)){
    sd_pkg_i->name=packing_texture_name;
  }
  ImGui::Checkbox("Separated",&sd_pkg_i->separated);
  ImGui::Checkbox("Mipmap",&sd_pkg_i->mipmap);
  
}

} // namespace vg