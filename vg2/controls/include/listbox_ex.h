#pragma once
#include "control_fac.h"
namespace vg {
class AFG_EXPORT listbox_ex : public control_base {
  DEF_STRUCT_WITH_INIT(_pt, 
  (bool, _vertical, {true}),
  (float, _interval, {0.f}), 
  (float, _inner_left, {0.f}),
  (float, _inner_top, {0.f}), 
  (float, _inner_right, {0.f}),
  (float, _inner_bottom, {0.f}))
public:
  listbox_ex();
  void draw();
  DECLARE_EX_INT
  DECLARE_DRAW_SEL_ANCHOR
};
REG_CTL_2_FAC(listbox_ex);
} // namespace vg