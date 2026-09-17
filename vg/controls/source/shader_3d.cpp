#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader_3d.h"
#include "user_control_imgui.h"
#include "imgui_internal.h""
namespace vg{
const float identity[16] = {
    1.f, 0, 0, 0, 
    0, 1.f, 0, 0, 
    0, 0, 1.f, 0,  
    0, 0, 0, 1.f, 
};
const auto identity_sz = sizeof(identity);
  shader_3d::shader_3d(){ 
    auto pfm=_sd3_pt.model.value;
    memcpy_s(pfm, identity_sz, identity, identity_sz);
    register_before_draw_handle( [&]{
      auto pfm = _sd3_pt.model.value;
      memcpy_s(pfm, identity_sz, identity, identity_sz);
    });
  }
}