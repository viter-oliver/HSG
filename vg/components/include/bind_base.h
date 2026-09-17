#pragma once
#include "control_fac.h"

namespace vg {
struct bind_base {
  sd_control_base _host{nullptr};
  int ix{ 0 };
  auto bind_member() {
	  assert(_host != nullptr);
	  return _host->get_member(ix);
  }
};
DEFINE_SMART(bind_base);
struct bind_pair {
  sd_bind_base _calcu_unit;
  v_sd_bind_base _var_list;
  std::string _expression;
};
DEFINE_SMART(bind_pair);
extern v_sd_bind_pair _vbinds;
void execute_binds();
} // namespace vg