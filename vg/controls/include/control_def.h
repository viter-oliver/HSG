#pragma once
#include "platform_def.h "
#include "property_utilities.h"
#include "vg_type.h"
#include <assert.h>
#include <functional>
#include <stdexcept>

namespace vg {
struct property_mem_range {
  void *phead_adree;
  int mem_len;
};
using v_property_mem_range = std::vector<property_mem_range>;
const int name_len = 50;
class control_def;
using sd_control_def = std::shared_ptr<control_def>;
using v_sd_control_def = std::vector<v_sd_control_def>;
class AFG_EXPORT control_def {
protected:
  vp_prop_ele _vprop_eles;
  DEF_STRUCT_WITH_INIT(base_prop, _in_p, (vec2, _pos, {0.f}),
                       (vec2, _size, {20.f}), (bool, _visible, {true}),
                       (char, _name[name_len]))
  v_sd_control_def _vchilds;
  /** the parent object, this member will NULL if current object is root */
  control_def *_parent = nullptr;

public:
  static prop_ele null_prop_ele;
  static field_ele null_field_ele;
  // static field_ele_with_value null_field_ele_with_value;
  prop_ele &get_prop_ele(int pgidx) {
    auto vpsz = _vprop_eles.size();
    if (pgidx >= vpsz) {
      return null_prop_ele;
    }
    return *_vprop_eles[pgidx];
  }
  field_ele &get_filed_ele(int pgidx, int field_idx) {
    auto &pele = get_prop_ele(pgidx);
    auto vfsz = pele._pro_page.size();
    if (field_idx >= vfsz) {
      return null_field_ele;
    }
    auto &vfiled = pele._pro_page;
    return *vfiled[field_idx];
  }
  virtual void ex_init_fun() {}
  control_def() { ex_init_fun(); }
  #ifdef WITHIN_IDE
  #define EX_INIT virtual void ex_init_fun();
  #else
  #define EX_INIT 
  #endif
  virtual ~control_def() {}
  void collect_property_range(v_property_mem_range &vplist) {
    for (auto &prop_ele : _vprop_eles) {
      vplist.emplace_back(prop_ele->_pro_address, prop_ele->_pro_sz);
    }
  }
  /*control_def *get_copy_of_object(){

  }*/
  std::function<void(void)> _before_draw_handle = nullptr;
  std::function<void(void)> _after_draw_handle = nullptr;
  virtual void draw() {
    if (!visiblity()) {
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
  vec2 &base_pos() { return _in_p._pos; }
  vec2 &size() { return _in_p._size; }
  bool &visiblity() { return _in_p._visible; }
  auto get_parent() { return _parent; }
  bool be_seen() {
    control_def *pparnt = this;
    do {
      if (!pparnt->visiblity()) {
        return false;
      }
    } while (pparnt = pparnt->get_parent());
    return true;
  }
  char *name() { return _in_p._name; }
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
    _vprop_eles.shrink_to_fit();
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
                                      control_def *pcur = NULL) {
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
        //DOUBT:could be better?
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
  void set_prop_fd_value(int pg_id, int fd_id, void *pvalue, int value_sz) {
    assert(pvalue && "invalid pvalue");
    auto pg_sz = _vprop_eles.size();
    assert(pg_id < pg_sz && "invalid pg_id");
    auto &pg_ele = _vprop_eles[pg_id];
    auto &vfd_ele = pg_ele->_pro_page;
    auto vfd_ele_sz = vfd_ele.size();
    assert(fd_id < vfd_ele_sz && "invalid fd_id");
    auto &fd_ele = *vfd_ele[fd_id];
    char *pdest = fd_ele._address;
    auto wsize = fd_ele._count * fd_ele._tpsz;
    assert(value_sz<=wsize);
    memcpy(pdest, pvalue, value_sz);
  }
};
} // namespace vg