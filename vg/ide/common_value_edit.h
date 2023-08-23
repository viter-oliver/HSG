#pragma once
#include "common_value.h"
namespace vg{
class common_value_edit
{
     sd_base_common_type _sl_bs_prp = { nullptr };
     std::string _key_name;
     std::string _popup_cap;
     bool _be_draging { false }, _hit_sm_window { false };
     int _pgidx { 0 }, _fdidx { 0 };
public:
     common_value_edit()
     {}
     ~common_value_edit() {}
     void set_dragging( bool be_dragging, sd_control_base  pobj = nullptr, uint16_t page_idx = 0, uint16_t off_idx = 0 );
     void view_base_prp_list();
     void base_prp_item();
     void clear_sel()
     {
          _sl_bs_prp = nullptr;
          _be_draging = _hit_sm_window = false;
     }
};
}
