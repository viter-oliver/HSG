#ifdef WITHIN_IDE
#include "material_ide.h"
using sd_material_base = vg::shader::sd_material_ide;
#else
#include "material.h"
using sd_material_base = vg::shader::sd_material;
#endif
namespace vg {
namespace shader {
using mp_sd_material = std::map<std::string, sd_material_base>;
extern mp_sd_material _mp_sd_materials;
} // namespace shader
} // namespace vg