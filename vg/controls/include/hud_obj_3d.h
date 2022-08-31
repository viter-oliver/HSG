#pragma once
#include "control_fac.h"
#include "shader.h"
#include "vg_texture.h"
#include "primitive.h"
namespace vg
{
     class hud_obj_3d :
          public control_base
     {
          DEF_STRUCT_WITH_INIT( pty_page, _pt_tb,
                                ( vui2, _image_txt ),
                                ( float, _tanslation_x, {0.f} ),
                                ( float, _tanslation_y, {0.f} ),
                                ( float, _tanslation_z, {0.f} ) )
          static shader::sd_shader_base _phud_sd;
          static primitive::sd_primitive_unit _ps_prm; 
          texture::sd_texture_unit _pat_image;
          
     public:

          hud_obj_3d();
          ~hud_obj_3d();
          
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
     };
     REG_CTL_2_FAC( hud_obj_3d );
}