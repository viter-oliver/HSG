#ifdef WITHIN_IDE
#include "shader_ide.h"
using sd_shader_def=vg::shader::sd_shader_ide;
#else
#include "shader.h"
using sd_shader_def=vg::shader::sd_shader_base;
#endif
namespace vg{
  namespace shader{
    using mp_sd_shder_def = std::map<std::string, sd_shader_def>;
    extern mp_sd_shder_def _mp_sd_shder_def;
  }
}