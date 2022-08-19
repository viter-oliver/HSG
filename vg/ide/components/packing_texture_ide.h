#include "packing_texture.h"
#include <string>
namespace vg {
namespace packing_texture {
struct sub_texture_coordinate_ide : public sub_texture_coordinate {
  std::string filename;
};
using sd_sub_texture_coordinate_ide=std::shared<sub_texture_coordinate_ide>;
struct packing_texture_unit_ide : public packing_texture_unit {
  std::string texture_pack_file, texture_data_file;
  std::string name;
};
using sd_packing_texture_unit_ide = std::shared_ptr<packing_texture_unit_ide>;
} // namespace packing_texture
void create_item(packing_texture::v_sd_packing_texture &pk_texture_list);
void select_item(packing_texture::sd_packing_texture_unit_ide &sd_pkg_i);
void edit_item(packing_texture::sd_packing_texture_unit_ide &sd_pkg_i);
} // namespace vg