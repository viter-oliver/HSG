#include "texture_ex.h"
namespace vg {
  struct texture_ide:public texture_ex{
    std::string texture_pack_file, texture_data_file;
    int sub_id{ -1 };
    using sd_texture_ide=std::shared_ptr<texture_ide>;
    static sd_texture_ide new_instance();
    static void edit(texture_ide& tu);
  };
} // namespace vg