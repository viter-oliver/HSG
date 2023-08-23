#pragma once
#include <assert.h>
#include <functional>
#include <stdexcept>
#include "platform_def.h "
#include "macro_tool.h"
#include "vg_type.h"
#include "property_utilities.h"
#include "def_member.h"

namespace vg {


class control_def;
DEFINE_SMART(control_def)
class AFG_EXPORT control_def {
protected:
  //vp_prop_ele _vprop_eles;
  v_mem_page _mem_page;//
  DEFINE_MEMBER_CONTAINER

  DEF_STRUCT_WITH_INIT(_in_p, 
  (vec2, _pos, {0.f}),
  (vec2, _size, {20.f}), 
  (bool, _visible, {true}),
  (char_50, _name))
  v_sd_control_def _vchilds;
  /** the parent object, this member will NULL if current object is root */
  control_def *_parent = nullptr;

public:
  DEFINE_GET_MEMBER
  virtual void ex_init_fun() {}
  control_def() { ex_init_fun(); }

#define DECLARE_EX_INT
#define DECLARE_PROVIDE_DRAGGING_VALUE
#define DECLARE_DRAW_OUTLINE
#define DECLARE_DRAW_SEL_ANCHOR


  virtual ~control_def() {}
  v_mem_page& get_mem_page() {
    return _mem_page;
  }
  /*control_def *get_copy_of_object(){

  }*/
  std::function<void(void)> _before_draw_handle = nullptr;
  std::function<void(void)> _after_draw_handle = nullptr;
  void draw_frames() {
    if (!visibility()) {
      return;
    }
    if (_before_draw_handle) {
      _before_draw_handle();
    }
    for (auto &pchild : _vchilds) {
      if (pchild->visibility()) {
        pchild->draw();
      }
    }
    if (_after_draw_handle) {
      _after_draw_handle();
    }
  }
  virtual void draw() {}
  virtual void draw_outline() {}
  vec2 &base_pos() { return _in_p._pos; }
  vec2 &size() { return _in_p._size; }
  bool &visibility() { return _in_p._visible; }
  auto get_parent() { return _parent; }
  bool be_seen() {
    control_def *pparnt = this;
    do {
      if (!pparnt->visibility()) {
        return false;
      }
    } while (pparnt = pparnt->get_parent());
    return true;
  }
  char *name() { return _in_p._name.value; }
  void add_child(sd_control_def pchild) {
    pchild->_parent = this;
    _vchilds.emplace_back(pchild);
  }
  void insert_child(sd_control_def pinsert_node, sd_control_def pchild) {
    pchild->_parent = this;
    int index = 0;
    for (auto &item : _vchilds) {
      if (item == pinsert_node) {
        break;
      }
      index++;
    }
    _vchilds.emplace(_vchilds.begin() + index, pchild);
  }
  void clear_rebundent_memory() {
    _vchilds.shrink_to_fit();
  }
  virtual void remove_child(sd_control_def pchild) {
    auto it = find(_vchilds.begin(), _vchilds.end(), pchild);
    if (it != _vchilds.end()) {
      _vchilds.erase(it);
    }
  }
  bool move_pre(sd_control_def pchild) {
    auto it = find(_vchilds.begin(), _vchilds.end(), pchild);
    if (it == _vchilds.end() || it == _vchilds.begin()) {
      return false;
    }
    auto itp = it - 1;
    std::iter_swap(it, itp);
    return true;
  }
  bool move_next(sd_control_def pchild) {
    auto it = find(_vchilds.begin(), _vchilds.end(), pchild);
    if (it == _vchilds.end() || it == (_vchilds.end() - 1)) {
      return false;
    }
    auto inext = it + 1;
    std::iter_swap(it, inext);
    return true;
  }
  sd_control_def get_child(int index) { return _vchilds[index]; }
  std::string try_create_a_child_name(std::string attempt_name,
                                      sd_control_def pcur = nullptr) {
    for (auto &ichd : _vchilds) {
      if (ichd != pcur && ichd->name() == attempt_name) {
        attempt_name += '_';
      }
    }
    return attempt_name;
  }
  size_t child_count() { return _vchilds.size(); }

  vec2 absolute_coordinate_of_base_pos() {

    vec2 base_pos_ = base_pos();
    control_def *parent = get_parent();
    while (parent /*&&!parent->_be_window*/) {
      base_pos_ += parent->base_pos();
      parent = parent->get_parent();
    }
    return base_pos_;
  }
  virtual bool contain(vec2 &tar_pos) {
    vec2 right_bottom = base_pos() + size();
    area_f control_area = {base_pos(), right_bottom};
    return control_area.contain(tar_pos);
  }
  sd_control_def get_hit_obj(vec2 &tar_pos) {
    for (auto it = _vchilds.rbegin(); it != _vchilds.rend(); it++) {
      if ((*it)->visibility()) {
        // DOUBT:could be better?
        auto hit_obj = (*it)->get_hit_obj(tar_pos);
        if (hit_obj) {
          return hit_obj;
        }
      }
    }
    if (contain(tar_pos)) {
      return sd_control_def(this);
    } else {
      return nullptr;
    }
  }
  bool set_prop_fd_value(int pg_id, int fd_id, void *pvalue,
                         u32 value_sz = -1) {
    if (!pvalue) {
      vg_print("pvalue is 0");
      return false;
    }
    
    return true;
  }
};
} // namespace vg