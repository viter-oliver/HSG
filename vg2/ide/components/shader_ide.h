#include "shader.h"
namespace vg {

class shader_ide : public shader::shader_base {
public:
  std::string _vs_code, _fs_code, _vs_name, _fs_name;
  using sd_shader_ide = std::shared_ptr<shader_ide>;
  static sd_shader_ide new_instance();
  static void edit(shader_ide& sd_u);
};

} // namespace vg