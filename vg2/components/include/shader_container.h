#include "macro_tool.h"
#ifdef WITHIN_IDE
#include "shader_ide.h"
using shader_def=vg::shader::shader_ide;
#else
#include "shader.h"
using shader_def=vg::shader::shader_base;
#endif
namespace vg{
    DEFINE_SMART(shader_def)
    inline m_sd_shader_def _mp_sd_shder_def;
  
}