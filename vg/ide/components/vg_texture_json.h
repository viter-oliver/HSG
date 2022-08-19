#include <vg_texture.h>
#include <nlohmann/json.hpp>
namespace vg{
  void res_unit_2_json(nlohmann::json& jTar,texture::sd_texture_unit pres_unit,std::string& res_unit_name);
  bool json_2_res_unit(texture::sd_texture_unit pres_unit,std::string& res_unit_name,nlohmann::json& jsrc);
}