#include "common_value.h"
namespace vg {
using namespace std;
mp_sd_base_common_type g_base_prp_dic;


void base_common_type::override_param_list()
{
     for (auto& itm_pm:_param_list)
     {
          auto& pgidx = itm_pm._page_index;
          auto& fdidx = itm_pm._field_index;
          auto& pobj = itm_pm._pobj;

          pobj->set_prop_fd_value( pgidx, fdidx, _sd_base_def->_address);

     }
}
bool prp_is_catched_by_base_prp_type(prop_ele_position &prp_pos,
                                     base_common_type &base_prp) {
  const auto &plist = base_prp._param_list;
  for (auto &iprp : plist) {
    if (iprp == prp_pos) {
      return true;
    }
  }
  return false;
}
bool cover_common_value(string name_of_common_value) {
  const auto &icmv = g_base_prp_dic.find(name_of_common_value);
  if (icmv != g_base_prp_dic.end()) {
    auto &btp = *icmv->second;
    btp.override_param_list();
    return true;
  }
  return false;
}
} // namespace vg