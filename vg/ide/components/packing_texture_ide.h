#include "packing_texture.h"
#include "dragging_value_to_tar.h"
#include "edit_interface.h"
namespace vg {
namespace packing_texture {
struct sub_texture_coordinate_ide : public sub_texture_coordinate {
  std::string filename;
};
using sd_sub_texture_coordinate_ide=std::shared_ptr<sub_texture_coordinate_ide>;
struct packing_texture_unit_ide : public packing_texture_unit,edit_interface{
  std::string texture_pack_file, texture_data_file;
  std::string name;
  bool _sel{false};
  void select_item(int index);
  void edit_item();
};
using sd_packing_texture_unit_ide = std::shared_ptr<packing_texture_unit_ide>;
using v_sd_packing_texture_unit_ide=std::vector<sd_packing_texture_unit_ide>;
} // namespace packing_texture
struct dragging_texture_item :public dragging_value_to_tar::dragging_value {
	int main_index{ -1 }, sub_index{ -1 };
	dragging_texture_item(int mi, int si) :main_index(mi), sub_index(si) {}
};
void create_item(packing_texture::v_sd_packing_texture_unit_ide& packing_texture_container);
} // namespace vg