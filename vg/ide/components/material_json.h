#include "material_ide.h"
#include <nlohmann/json.hpp>
namespace vg{
  void res_unit_2_json(nlohmann::json& jTar,shader::material_ide& res_unit);
  bool json_2_res_unit(shader::material_ide& res_unit,nlohmann::json& jsrc);
}