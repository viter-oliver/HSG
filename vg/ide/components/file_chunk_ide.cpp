#include <iostream>
#include "file_chunk_ide.h"
#include "common_functions.h"
#include "dir_output.h"
#include "ide_common_functions.h"
#include "user_control_imgui.h"
namespace vg {
using namespace std;
using json=nlohmann::json;
extern string g_current_directory;
void file_chunk_ide::edit_item() { ImGui::Text("File size:%d", _fsize); }
void file_chunk_ide::to_json(nlohmann::json &jTar) {
    jTar["name"]=_name;
    jTar["fsize"]=_fsize;
}
bool file_chunk_ide::from_json(nlohmann::json &jSrc) {
    try{
        _name=jSrc["name"];
        auto file_path=g_current_directory + files_fold + _name;
        if(!get_file_data(file_path,[&](u32 file_sz)->char*{
            re_alloc(file_sz);
            return (char*)_pbin;
        })){
            vg_print("fail to get data from file:%s \n", file_path.c_str());
            return false;
        }
        
    } catch (json::exception &e){
        std::cerr << e.what() << '\n';
        return false;
    }
    
}
void create_item(mp_sd_file_chunk_ide &file_chunk_list) {
  static string file_name;
  static bool first_call = true;
  if (first_call) {
    file_name.clear();
    first_call = false;
  }
  ImGui::Text("File name:%s", file_name.c_str());
  ImGui::SameLine();
  if (ImGui::Button("...")) {
    open_file_to_folder(file_name, files_fold, "Loading new file");
  }
  bool be_enabled = !file_name.empty();
  if (!be_enabled) {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  }
  if (ImGui::Button("New file ...")) {
    auto file_full_path = g_current_directory + files_fold + file_name;
    get_file_data(file_full_path, [&](u32 file_size) -> char * {
      auto pnew_file = make_shared<file_chunk_ide>(file_size);
      file_chunk_list[file_name] = pnew_file;
      return (char *)pnew_file->_pbin;
    });
  }
  if (!be_enabled) {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
}
} // namespace vg