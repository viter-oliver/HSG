#include "packing_texture_ide.h"
#include <nlohmann/json.hpp>
namespace vg{
  void res_unit_2_json(nlohmann::json& jTar,packing_texture::packing_texture_unit_ide& res_unit);
  bool json_2_res_unit(packing_texture::packing_texture_unit_ide& res_unit,nlohmann::json& jsrc);
}