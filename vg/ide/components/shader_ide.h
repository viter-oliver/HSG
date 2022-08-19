#include "shader.h"
namespace vg {
namespace shader {
class shader_ide : public shader_base {
public:
  std::string _vs_code, _fs_code, _vs_name, _fs_name;
  bool _sel{false};
};
using sd_shader_ide=std::shared_ptr<shader_ide>;
} // namespace shader
void create_item(shader::mp_sd_shder_base& shader_list);
void select_item(shader::sd_shader_ide& sd_shd_i);
void edit_item(shader::sd_shader_ide& sd_shd_i);
} // namespace vg