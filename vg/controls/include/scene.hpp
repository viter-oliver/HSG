#pragma once
#include "shared_properties.h"
namespace vg
{
class AFG_EXPORT scene :public shared_properties {
     DEF_STRUCT_WITH_INIT(_pt,
     (vec3,_view_pos ),
     (vec3,_center_of_prj),
     (vec3,_up),
     (float,_fovy,{20}),
     (float,_near,{0.02}),
     (float,_far,{100.f}),
     (float,_aspect,{-1}),
     (vec3, _light_direction),
     (vec3, _light_position),
     (vec3, _light_ambient_clr),
     (vec3, _light_diffuse_clr),
     (vec3, _light_specular_clr),
     (bool,_test_depth,{true}),
     (vec4,_background_clr))
     unsigned int _FBO_id = { 0 };
     unsigned int _texture_id = { 0 };
     unsigned int _RBO_id = { 0 };
     void release_resource();
public:
     scene();
     ~scene();
     //void calculate_properties(m_sd_bind_var &child_properties);
     void calculate_properties(m_sd_bind_var &child_properties);
     void draw_frames();
     bool handle_mouse() { return true; }

     vec3* get_view_pos() {
          return &_pt._view_pos;
     }
     vec3* get_center_of_prj() {
          return &_pt._center_of_prj;
     }
     vec3* get_up() {
          return &_pt._up;
     }
     float get_fovy() {
          return _pt._fovy;
     }
     float get_near(){
          return _pt._near;
     }
     float get_far(){
          return _pt._far;
     }
     void set_fovy(float fovy) {
          _pt._fovy=fovy;
     }
     void set_far(float ffar){
          _pt._far = ffar;
     }
     void set_near(float fnear) {
          _pt._near=fnear;
     }
     void set_aspect(float aspect) {
          _pt._aspect = aspect;
     }
     float get_aspect() {
          if (_pt._aspect > 0.f) {
               return _pt._aspect;
          } else {
               float aspect = size().x() /size().y();
               return aspect;
          }
     }
     vec3& get_light_dir(){
          return _pt._light_direction;
     }
     vec3& get_light_position() {
          return _pt._light_position;
     }
     vec3& get_light_ambient(){
          return _pt._light_ambient_clr;
     }
     vec3& get_light_diffuse(){
          return _pt._light_diffuse_clr;
     }
     vec3& get_light_specular(){
          return _pt._light_specular_clr;
     }
      DECLARE_EX_INT
    };
    REG_CTL_2_FAC(scene);
}

