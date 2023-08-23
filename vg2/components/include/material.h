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
  bool be_same_shader(sd_shader_base &sd_shd_i) {
    return sd_shd_i == this->_pshader;
  }
  void use();
  bool valid() {
      return _pshader->valid();
  }
  sd_shader_variable get_sd_shader_variable(std::string& sv_name) {
      auto iv = _uniform_variales.find(sv_name);
      if (iv != _uniform_variales.end()) {
          return iv->second;
      }
      return nullptr;
  }
  template<typename T>
  void set_value(std::string uniform_name,T* value,u32 count){
    auto iuniform_v=_uniform_variales.find(uniform_name);
    if(iuniform_v==_uniform_variales.end()){
      vg_print("invalid uniform name:%s\n",uniform_name.c_str());
      return;
    }
    auto punf_v=iuniform_v->second.get();
    if(typeid(shader_variable_T<T>)==typeid(*punf_v)){
      auto punf_real=static_cast<shader_variable_T<T>*>(punf_v);
      auto pdata=punf_real->phead_data();
      auto buf_len=punf_real->get_buff_len();
      auto data_len=buf_len>count?count:buf_len;
      memcpy(pdata,value,sizeof(T)*data_len);
    }

  }
};
using sd_material = std::shared_ptr<material>;

void refresh_material(sd_shader_base &sd_shd_i);
} // namespace shader
} // namespace vg