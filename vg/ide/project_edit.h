#pragma once
#include "control_fac.h"
namespace vg {
class project_edit {
  control_base &_root;
  control_base *_pcurrent_object;
  control_base *_pcopy_object;
  control_base *_pcut_object;
  bool _trigger_focus_switch = {false};
  void view_object(control_base &fb);

public:
  project_edit(control_base &fb)
      : _root(fb), _pcurrent_object(nullptr), _pcopy_object(nullptr),
        _pcut_object(nullptr) {}
  void objects_view();

  void move_item_pre();
  void move_item_next();
  void copy_item();
  void cut_item();
  void past_item();
  void add_item();
  void insert_item();
  void add_sibling();
  void delete_item();
  void popup_context_menu();
  void clear_sel_item();
  void trigger_focus_switch() { _trigger_focus_switch = true; }
  void sel_ui_component(control_base *ui_target) {
    assert(ui_target);
    if (_pcurrent_object) {
      _pcurrent_object->set_selected(false);
    }
    ui_target->set_selected(true);
    _pcurrent_object = ui_target;
  }
  control_base *current_object() { return _pcurrent_object; }
};
} // namespace vg