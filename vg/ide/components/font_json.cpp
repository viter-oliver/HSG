#include "font_json.h"
#include "font_manager.h"
#include <iostream>
#include "dir_output.h"
namespace vg {
extern std::string g_current_directory;
void res_unit_2_json(nlohmann::json &jTar, font_unit &res_unit) {
  jTar["name"] = res_unit._name;
  jTar["cols"] = res_unit._char_count_c;
  jTar["rows"] = res_unit._char_count_r;
}
bool json_2_res_unit(font_unit &res_unit, nlohmann::json &jsrc) {
  try {

    res_unit._name = jsrc["name"];
    res_unit._char_count_c = jsrc["cols"];
    res_unit._char_count_r = jsrc["rows"];
    std::string font_path=g_current_directory+font_fold+res_unit._name;
    font_manager::load_font(res_unit, font_path);
  } catch (nlohmann::json::exception &e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}
} // namespace vg