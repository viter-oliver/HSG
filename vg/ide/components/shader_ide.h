#include "shader.h"
#include "edit_interface.h"
namespace vg {
namespace shader {
class shader_ide : public shader_base,edit_interface {
public:
  std::string _vs_code, _fs_code, _vs_name, _fs_name;
};
using sd_shader_ide=std::shared_ptr<shader_ide>;
using mp_sd_shader_ide = std::map<std::string, sd_shader_ide>;
} // namespace shader
void create_item(shader::mp_sd_shader_ide& shader_list);
} // namespace vg