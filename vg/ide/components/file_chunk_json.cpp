#include "file_chunk_json.h"
#include "common_functions.h"
#include "dir_output.h"
namespace vg {
using namespace std;
using json=nlohmann::json;
extern string g_current_directory;

void res_unit_2_json(nlohmann::json &jTar, sd_file_chunk_unit pres_unit,
                     std::string &res_unit_name) {
  jTar["name"] = res_unit_name;
  jTar["fsize"]=pres_unit->_fsize;
}
bool json_2_res_unit(sd_file_chunk_unit pres_unit, std::string &res_unit_name,
                     nlohmann::json &jsrc) {
  try {
    res_unit_name = jsrc["name"];
    string file_path = g_current_directory + files_fold + res_unit_name;
    if (!get_file_data(file_path, [&](u32 file_size) {
          pres_unit = make_shared<file_chunk_unit>(file_size);
          return file_fata->_pbin;
        })) {
      printf("fail to get data from file:%s \n", file_name.c_str());
      return false;
    }
  } catch (json::exception &e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  return true;
}
} // namespace vg