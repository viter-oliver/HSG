#pragma once
#include "bind.h"
namespace vg {
struct base_common_type {
  std::string _type;
  void *_pbase;
  int _size;
  vprop_pos _param_list;
  base_common_type(std::string bty);
  ~base_common_type() { free(_pbase); }
  void override_param_list();
};
using sd_base_common_type = std::shared_ptr<base_common_type>;
using mp_sd_base_common_type = std::map<std::string, sd_base_common_type>;
extern mp_sd_base_common_type g_base_prp_dic;
extern bool AFG_EXPORT cover_common_value( std::string name_of_common_value );
extern bool AFG_EXPORT prp_is_catched_by_base_prp_type( prop_ele_position& prp_pos, base_common_type& base_prp );

} // namespace vg
