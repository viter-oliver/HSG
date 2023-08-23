#include "bind_var.h"
#include "user_control_imgui.h"
namespace vg{
  void bind_var_t<s32>::edit(){
    ImGui::InputInt(_var_name.c_str(),&_value);
  }
  
  void bind_var_t<float>::edit(){
    ImGui::InputFloat(_var_name.c_str(),&_value);
  }

  void bind_var_t<bool>::edit(){
    ImGui::Checkbox(_var_name.c_str(),&_value);
  }  

  void bind_var_t<vec2>::edit(){
    //ImGui::SliderFloat2(_var_name.c_str(),(float*)&_value,);
  }

  void bind_var_t<vec3>::edit(){

  }

  void bind_var_t<vec4>::edit(){

  }
  void bind_var_t<char[50]>::edit(){
      ImGui::InputText(_var_name.c_str(), _value, FILE_NAME_LEN);
  }
}