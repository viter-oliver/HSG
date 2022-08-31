#pragma once
#include "control_fac.h"
namespace vg
{
     class AFG_EXPORT hud_projector :
     public control_base
     {
          DEF_STRUCT_WITH_INIT( intl_pt, _pj_pt,
                                (vec3,_view_pos ),
                                (vec3,_center_of_prj),
                                (vec3,_up),
                                (float,_fovy),
                                (float,_near),
                                (float,_far),
                                (bool,_test_depth,false),
                                (vec4,_bk_clr))
          u32 _fboId = { 0 };
          u32 _colorTextId = { 0 };
          u32 _depthStencilTextId = { 0 };
          void release_resource();
     public:
          hud_projector();
          ~hud_projector();
          DECLARE_EX_INT
          DECLARE_DRAW_SEL_ANCHOR
          void link();
          void draw();
          vec3& get_view_pos()
          {
               return _pj_pt._view_pos;
          }
          vec3& get_center_of_prj()
          {
               return _pj_pt._center_of_prj;
          }
          vec3& get_up()
          {
               return _pj_pt._up;
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
     };
     REG_CTL_2_FAC( hud_projector );
}

