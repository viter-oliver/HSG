#pragma once
#include "vg_bind.h"
class common_value_edit
{
     ps_bs_prp _sl_bs_prp = { nullptr };
     string _key_name;
     string _popup_cap;
     bool _be_draging { false }, _hit_sm_window { false };
     int _pgidx { 0 }, _fdidx { 0 };
public:
     common_value_edit()
     {}
     ~common_value_edit() {}
     void set_dragging( bool be_dragging, control_common_def* pobj = nullptr, uint16_t page_idx = 0, uint16_t off_idx = 0 );
     void view_base_prp_list();
     void base_prp_item();
     void clear_sel()
     {
          _sl_bs_prp = nullptr;
          _be_draging = _hit_sm_window = false;
     }
};

