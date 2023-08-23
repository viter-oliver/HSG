#pragma once
#include "macro_tool.h"
#ifdef WITHIN_IDE
#include "material_ide.h"
using material_def = vg::shader::material_ide;
#else
#include "material.h"
using material_def = vg::shader::material;
#endif
namespace vg {
DEFINE_SMART(material_def)
inline m_sd_material_def _mp_sd_materials;
} // namespace vg