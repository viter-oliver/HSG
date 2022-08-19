#pragma once
#include "control_fac.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace vg
{
     class AFG_EXPORT trans :
          public control_base
     {
          DEF_STRUCT_WITH_INIT( intl_pt, _pt,
               ( int, _trans_order, { 0 } ),
               ( float, _trans_translation_x, { 0.f } ),
               ( float, _trans_translation_y, { 0.f } ),
               ( float, _trans_translation_z, { 0.f } ),
               ( float, _trans_scale_x, { 1.f } ),
               ( float, _trans_scale_y, { 1.f } ),
               ( float, _trans_scale_z, { 1.f } ),
               ( int, _rotate_order, { 0 } ),
               ( float, _trans_rotation_x, { 0.f } ),
               ( float, _trans_rotation_y, { 0.f } ),
               ( float, _trans_rotation_z, { 0.f } ) )
     public:
          trans();
          void transform( glm::mat4& model );
     };
     REG_CTL_2_FAC( trans );
}
