#pragma once
#include "macro_tool.h"
namespace vg{
  struct var_base {};
    using sd_var_base = std::shared_ptr<var_base>;

  struct bind_var{
    virtual void edit(){};
    virtual sd_var_base spawn() = 0;
    virtual bool assign(sd_var_base& sbase) = 0;
    virtual bool scale(sd_var_base from, sd_var_base to,double s) = 0;
  };
  DEFINE_SMART(bind_var)

  
}