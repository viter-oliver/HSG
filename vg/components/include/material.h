#include "shader.h"
namespace vg {
namespace shader {
class material {
protected:
  sd_shader_base _pshader;
  mp_sd_shader_variable _uniform_variales;

public:
  material(sd_shader_base pshader);
  void refresh_uniform_variables();
  bool be_same_shader(sd_shader_base& sd_shd_i){
    return sd_shd_i==this->_pshader;
  }
};
using sd_material = std::shared_ptr<material>;
using mp_sd_material = std::map<std::string, sd_material>;
extern mp_sd_material _mp_sd_materials;
void refresh_material(sd_shader_base &sd_shd_i);
} // namespace shader
} // namespace vg