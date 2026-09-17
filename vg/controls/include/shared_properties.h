#pragma once
#include "control_fac.h"
namespace vg{
#define FIND_ITEM(x)                                                           \
  auto i##x = child_properties.find(#x); 
    class AFG_EXPORT shared_properties:public control_base{
public:
    virtual void calculate_properties(m_sd_bind_var& child_properties);
    DECLARE_DRAW_PROPERTY
    };
}