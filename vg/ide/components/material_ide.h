#include "material.h"
#include "shader_ide.h"
namespace vg {
class shader::material_ide;
using sd_material_ide = std::shared_ptr<shader::material_ide>;
void create_item(shader::mp_sd_material& material_list);
void edit_item(shader::sd_material_ide& sd_mtl_i);
void select_item(shader::sd_material_ide& sd_mtl_i);
namespace shader {
class material_ide : public material {
public:
  std::string _shader_name;
  friend void edit_item(shader::sd_material_ide& sd_mtl_i);
};
} // namespace shader
} // namespace vg