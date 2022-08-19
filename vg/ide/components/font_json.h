#include "font.h"
#include <nlohmann/json.hpp>
namespace vg{
  void res_unit_2_json(nlohmann::json& jTar,font_unit& res_unit);
  bool json_2_res_unit(font_unit& res_unit,nlohmann::json& jsrc);
}