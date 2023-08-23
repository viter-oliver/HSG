#include "ide_common_functions.h"
#include "common_functions.h"
#include <Commdlg.h>
#include <ShlObj.h>
#include <nlohmann/json.hpp>
#include <windows.h>
#include <iostream>
namespace vg {
using namespace std;
using json = nlohmann::json;
extern string g_current_directory;
void open_file_to_folder(std::string &file_name, const char *folder,
                         const char *dlg_title, const char *str_filter) {
  OPENFILENAME ofn = {sizeof(OPENFILENAME)};
  ofn.hwndOwner = GetForegroundWindow();
  ofn.lpstrFilter = str_filter;
  char strFileName[MAX_PATH] = {0};
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = strFileName;
  ofn.nMaxFile = sizeof(strFileName);
  ofn.lpstrTitle = dlg_title;
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
  if (GetOpenFileName(&ofn)) {
    string dest_dir = g_current_directory + folder;
    string src_file_path = strFileName;
    string src_file_name =
        src_file_path.substr(src_file_path.find_last_of('\\') + 1);
    string test_full_name = dest_dir + src_file_name;
    getUniqueFileName(test_full_name);
    string str_cmd = "copy " + src_file_path + " " + test_full_name;
    system(str_cmd.c_str());
    file_name = test_full_name.substr(dest_dir.size());
  }
}
void load_vtex_from_json(v_sd_sub_txt& vcoor, char* json_buff, int buff_len){
  try {
      
    json jdata_format = json::parse(json_buff);
    json frames = jdata_format["frames"];
    int isz = frames.size();
    for (int ix = 0; ix < isz; ++ix) {
      auto &frame = frames[ix];
      bool rotated = frame["rotated"];
      auto txt_coor_u = make_shared<sub_txt>();
      txt_coor_u->item_name = frame["filename"];
      txt_coor_u->_x0 = frame["x"];
      txt_coor_u->_y0 = frame["y"];
      txt_coor_u->_x1 = frame["x"] + frame["w"].get<float>();
      txt_coor_u->_y1 = frame["y"] + frame["h"].get<float>();
      if (rotated) {
        txt_coor_u->_x1 = frame["x"] + frame["h"].get<float>();
        txt_coor_u->_y1 = frame["y"] + frame["w"].get<float>();
      }
      vcoor.emplace_back(txt_coor_u);
    }
    if (vcoor.size() > 0) {
      return;
    }
    for (auto &ju : frames.items()) {
        auto& ju_value = ju.value();
      auto &frame = ju_value["frame"];
      auto txt_coor_u = make_shared<sub_txt>();
      txt_coor_u->item_name = ju_value["filename"];
      bool rotated = frame["rotated"];
      txt_coor_u->_x0 = frame["x"];
      txt_coor_u->_y0 = frame["y"];
      txt_coor_u->_x1 = frame["x"] + frame["w"].get<float>();
      txt_coor_u->_y1 = frame["y"] + frame["h"].get<float>();
      if (rotated) {
        txt_coor_u->_x1 = frame["x"] + frame["h"].get<float>();
        txt_coor_u->_y1 = frame["y"] + frame["w"].get<float>();
      }
      vcoor.emplace_back(txt_coor_u);
    }

  } catch (json::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
} // namespace vg