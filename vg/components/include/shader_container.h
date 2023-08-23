#ifdef WITHIN_IDE
#include "shader_ide.h"
using shader_def=vg::shader::shader_ide;
#else
#include "shader.h"
using shader_def=vg::shader::shader_base;
#endif
namespace vg{
    using sd_shader_def=std::shared<shader_def>
    using mp_sd_shder_def = std::map<std::string, sd_shader_def>;
    extern mp_sd_shder_def _mp_sd_shder_def;
  }
}