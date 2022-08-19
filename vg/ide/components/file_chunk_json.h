
#include "file_chunk.h"
#include <nlohmann/json.hpp>
namespace vg{
  void res_unit_2_json(nlohmann::json& jTar,sd_file_chunk_unit pres_unit,std::string& res_unit_name);
  bool json_2_res_unit(sd_file_chunk_unit pres_unit,std::string& res_unit_name,nlohmann::json& jsrc);
}