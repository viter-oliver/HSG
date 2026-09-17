#pragma once
#include "shared_properties.h"
namespace vg {
enum en_trans_order {
  en_translate_scale_rotate,
  en_translate_rotate_scale,
  en_scale_translate_rotate,
  en_scale_rotate_translate,
  en_rotate_translate_scale,
  en_rotate_scale_translate,
  en_trans_order_cnt
};
enum en_rotate_order {
  en_rotate_x_y_z,
  en_rotate_x_z_y,
  en_rotate_y_x_z,
  en_rotate_y_z_x,
  en_rotate_z_x_y,
  en_rotate_z_y_x,
  en_rotate_order_cnt
};
class AFG_EXPORT ctrl_model : public shared_properties {
  DEF_STRUCT_WITH_INIT( _pt, 
      (int, _trans_order, {0}),
      (float, _trans_translation_x, {0.f}),
      (float, _trans_translation_y, {0.f}),
      (float, _trans_translation_z, {0.f}), 
      (float, _trans_scale_x, {1.f}),
      (float, _trans_scale_y, {1.f}), 
      (float, _trans_scale_z, {1.f}),
      (int, _rotate_order, {0}), 
      (float, _trans_rotation_x, {0.f}),
      (float, _trans_rotation_y, {0.f}), 
      (float, _trans_rotation_z, {0.f}))
public:
  DECLARE_EX_INT
  void calculate_properties(m_sd_bind_var &child_properties);
#define DEF_TRANS_SET(item, memb)                                              \
  ctrl_model &set_##item##_##memb(float fvalue) {                                \
    _pt._trans_##item##_##memb = fvalue;                                       \
    return *this;                                                              \
  }
  DEF_TRANS_SET(translation, x)
  DEF_TRANS_SET(translation, y)
  DEF_TRANS_SET(translation, z)
  DEF_TRANS_SET(scale, x)
  DEF_TRANS_SET(scale, y)
  DEF_TRANS_SET(scale, z)
  DEF_TRANS_SET(rotation, x)
  DEF_TRANS_SET(rotation, y)
  DEF_TRANS_SET(rotation, z)
#define DEF_TRANS_GET(item, memb)                                              \
  auto get_##item##_##memb() { return _pt._trans_##item##_##memb; }
  DEF_TRANS_GET(translation, x)
  DEF_TRANS_GET(translation, y)
  DEF_TRANS_GET(translation, z)
  DEF_TRANS_GET(scale, x)
  DEF_TRANS_GET(scale, y)
  DEF_TRANS_GET(scale, z)
  DEF_TRANS_GET(rotation, x)
  DEF_TRANS_GET(rotation, y)
  DEF_TRANS_GET(rotation, z)
};
REG_CTL_2_FAC(ctrl_model);
} // namespace vg
