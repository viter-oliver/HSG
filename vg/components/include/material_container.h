#ifdef WITHIN_IDE
#include "material_ide.h"
using material_def = vg::shader::material_ide;
#else
#include "material.h"
using material_def = vg::shader::material;
#endif
namespace vg {
namespace shader {
  using sd_material_def=std::shared_ptr<material_def>;
using mp_sd_material = std::map<std::string, sd_material_def>;
extern mp_sd_material _mp_sd_materials;
} // namespace shader
} // namespace vg