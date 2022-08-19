#pragma once
#include "control_fac.h"
namespace vg {
class AFG_EXPORT button : public control_base {
public:
  enum state { en_normal, en_pressed, en_selected, en_state_cnt };

private:
  DEF_STRUCT_WITH_INIT(intl_pt, _pt, (vi2, _image_normal_txt),
                       (vi2, _image_press_txt),
                       (vi2, _image_select_txt),
                       (vec4, _tint_clr),
                       (vec4, _border_clr)
                       )
  state _state = {en_normal};

public:
  button();
  // ctl_button(int tid) :control_common_def(tid){}
  void draw();
  void set_state(state st) { _state = st; }
  state get_state() { return _state; }
  vi2& get_cur_txt_id() {
    switch (_state) {
    case button::en_normal:
      return _pt._image_normal_txt;
    case button::en_pressed:
      return _pt._image_press_txt;
    case button::en_selected:
      return _pt._image_select_txt;
    }
    return {0, 0};
  }
};
REG_CTL_2_FAC(button);
} // namespace vg