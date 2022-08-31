#pragma once
#include "control_fac.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
class AFG_EXPORT transform : public control_base {
  DEF_STRUCT_WITH_INIT(
      intl_pt, _pt, (int, _trans_order, {0}),
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
  transform();
  DECLARE_EX_INT
  void apply(glm::mat4 &model);
  transform &set_trans_scalex(float sx) {
    _pt._trans_scale_x = sx;
    return *this;
  }
  transform &set_trans_scaley(float sy) {
    _pt._trans_scale_y = sy;
    return *this;
  }
  transform &set_trans_scalez(float sz) {
    _pt._trans_scale_z = sz;
    return *this;
  }

  transform &set_trans_rtx(float rx) {
    _pt._trans_rotation_x = rx;
    return *this;
  }
  transform &set_trans_rty(float ry) {
    _pt._trans_rotation_y = ry;
    return *this;
  }
  transform &set_trans_rtz(float rz) {
    _pt._trans_rotation_z = rz;
    return *this;
  }

  transform &set_trans_tlx(float tx) {
    _pt._trans_translation_x = tx;
    return *this;
  }
  transform &set_trans_tly(float ty) {
    _pt._trans_translation_y = ty;
    return *this;
  }
  transform &set_trans_tlz(float tz) {
    _pt._trans_translation_z = tz;
    return *this;
  }
  float get_trans_tlx() { return _pt._trans_translation_x; }
  float get_trans_tly() { return _pt._trans_translation_y; }
  float get_trans_tlz() { return _pt._trans_translation_z; }
};
REG_CTL_2_FAC(transform);
} // namespace vg
