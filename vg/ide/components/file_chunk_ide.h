#include "file_chunk.h"
#include "edit_interface.h"
#include <nlohmann/json.hpp>
namespace vg{
  struct file_chunk_ide:public file_chunk_unit,edit_interface{
    std::string _name;
    void edit_item();
    void to_json(nlohmann::json& jTar);
    bool from_json(nlohmann::json& jSrc);
  };
  using sd_file_chunk_ide=std::shared_ptr<file_chunk_ide>;
  using mp_sd_file_chunk_ide=std::map<std::string,sd_file_chunk_ide>;
  void create_item(mp_sd_file_chunk_ide& file_chunk_list);
}