#include "shared_properties.h"
namespace vg{
    void shared_properties::calculate_properties(m_sd_bind_var& child_properties){
      auto parent = get_parent();
      auto test_ptr = dynamic_cast<shared_properties *>(parent);
      if (test_ptr) {
        test_ptr->calculate_properties(child_properties);
      } 
      #if 0
      for (auto &s_bv : _v_bind_var) {
        auto ichd = child_properties.find(s_bv->var_name());
        if (ichd != child_properties.end()) {
          auto svalue = s_bv->spawn();
          ichd->second->assign(svalue);
          child_properties.erase(ichd);
        }
      }
      #endif
    }
}