#pragma once
#include "control_fac.h"

namespace vg
{
     class AFG_EXPORT light_scene :
     public control_base
     {
          DEF_STRUCT_WITH_INIT( _pj_pt,
          (vec3,_view_pos ),
          (vec3,_center_of_prj),
          (vec3,_up),
          (float,_fovy),
          (float,_near),
          (float,_far),
          (vec3,_light_pos),
          (vec3, _light_ambient_clr),
          (vec3, _light_diffuse_clr),
          (vec3, _light_specular_clr),
          (bool,_test_depth),
          (vec4,_bk_clr))
          unsigned int _fboId = { 0 };
          unsigned int _colorTextId = { 0 };
          unsigned int _depthStencilTextId = { 0 };
          void release_resource();
     public:
          light_scene();
          ~light_scene();
          void link();
          void draw_frames();
         bool handle_mouse();
          vec3* get_view_pos()
          {
               return &_pj_pt._view_pos;
          }
          vec3* get_center_of_prj()
          {
               return &_pj_pt._center_of_prj;
          }
          vec3* get_up()
          {
               return &_pj_pt._up;
          }
          float get_fovy()
          {
               return _pj_pt._fovy;
          }
          float get_near()
          {
               return _pj_pt._near;
          }
          float get_far()
          {
               return _pj_pt._far;
          }
		  void set_fovy(float fovy)
		  {
			  _pj_pt._fovy=fovy;
		  }
		  void set_far(float ffar)
		  {
			  _pj_pt._far = ffar;
		  }
		  void set_near(float fnear)
		  {
			  _pj_pt._near=fnear;
		  }
		  vec3& get_light_pos(){
			  return _pj_pt._light_pos;
		  }
		  vec3& get_light_ambient(){
			  return _pj_pt._light_ambient_clr;
		  }
		  vec3& get_light_diffuse(){
			  return _pj_pt._light_diffuse_clr;
		  }
		  vec3& get_light_specular(){
			  return _pj_pt._light_specular_clr;
		  }
          DECLARE_EX_INT
     };
     REG_CTL_2_FAC( light_scene )
}

