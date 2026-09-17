#pragma once
#include "bind_var.h"
namespace vg{
  struct computable_ele{
    virtual sd_bind_var get_sd_bind_var()=0;
  };
}