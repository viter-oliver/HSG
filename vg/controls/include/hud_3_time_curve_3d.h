#pragma once
#include "control_fac.h"
#include "shader.h"
#include "vg_texture.h"
#include "vertex.h"

namespace vg
{
     class hud_3_time_curve_3d :
          public control_base
     {
          DEF_STRUCT_WITH_INIT( pty_page, _pt_tb,
                                ( vui2, _image_txt ),
                                ( float, _coeff_hac[4] ),
                                ( bool, _lectl_border, {true} ),
                                ( float, _width, { 100.f } ),
                                ( float, _tanslation_x, { 0.f } ),
                                ( float, _tanslation_y, { 0.f } ),
                                ( float, _tanslation_z, { 0.f } ) )
          static shader::sd_shader_base _phud_sd;
          static vertex::sd_vertex_unit _ps_prm;
     public:
          hud_3_time_curve_3d();
          ~hud_3_time_curve_3d();
          void link();
          void draw();
          void set_transx( float transx )
          {
               _pt_tb._tanslation_x = transx;
          }
          void set_transy( float transy )
          {
               _pt_tb._tanslation_y = transy;
          }
          void set_transz( float transz )
          {
               _pt_tb._tanslation_z = transz;
          }
          void set_coeff( float cf0, float cf1, float cf2, float cf3 )
          {
               _pt_tb._coeff_hac[ 0 ] = cf0;
               _pt_tb._coeff_hac[ 1 ] = cf1;
               _pt_tb._coeff_hac[ 2 ] = cf2;
               _pt_tb._coeff_hac[ 3 ] = cf3;
        }
     };
     REG_CTL_2_FAC(hud_3_time_curve_3d);
}


